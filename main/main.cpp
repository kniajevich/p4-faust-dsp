#include <cstdio>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2s_tdm.h"
#include "faust_dsp_core.hpp"
#include "phi_dsp_math.h"

static const char *TAG = "P4_FAUST_DSP";

// 32-byte aligned DSP scratchbuffers for RISC-V SIMD block vectorization
alignas(32) static float dsp_out_left[DSP_BLOCK_SIZE];
alignas(32) static float dsp_out_right[DSP_BLOCK_SIZE];
alignas(32) static uint32_t tdm_tx_buffer[DSP_BLOCK_SIZE * 8];

static i2s_chan_handle_t tx_chan = nullptr;
static FaustDspCore dsp_engine;

static esp_err_t init_tdm_hardware() {
    ESP_LOGI(TAG, "Initializing 8-Slot TDM Hardware for Faust Runtime");
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 8;
    chan_cfg.dma_frame_num = 128;
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, nullptr));

    i2s_tdm_config_t tdm_cfg = {
        .clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(DSP_SAMPLE_RATE),
        .slot_cfg = I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO, I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3 | I2S_TDM_SLOT4 | I2S_TDM_SLOT5 | I2S_TDM_SLOT6 | I2S_TDM_SLOT7),
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = GPIO_NUM_12,
            .ws   = GPIO_NUM_13,
            .dout = GPIO_NUM_11,
            .din  = GPIO_NUM_NC,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };
    tdm_cfg.slot_cfg.total_slot = 8;

    ESP_ERROR_CHECK(i2s_channel_init_tdm_mode(tx_chan, &tdm_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
    return ESP_OK;
}

static void dsp_audio_task(void *pvParameters) {
    (void)pvParameters;
    ESP_LOGI(TAG, "Audio DSP Core Task running on High-Speed RISC-V Engine");

    dsp_engine.init(DSP_SAMPLE_RATE);
    float* outputs[2] = { dsp_out_left, dsp_out_right };

    while (true) {
        // 1. Compute DSP block
        dsp_engine.compute(DSP_BLOCK_SIZE, nullptr, outputs);

        // 2. Interleave stereo float into 8-channel 32-bit TDM slots
        for (size_t i = 0; i < DSP_BLOCK_SIZE; ++i) {
            int32_t val_l = static_cast<int32_t>(dsp_out_left[i] * 2147483647.0f);
            int32_t val_r = static_cast<int32_t>(dsp_out_right[i] * 2147483647.0f);

            tdm_tx_buffer[i * 8 + 0] = static_cast<uint32_t>(val_l);
            tdm_tx_buffer[i * 8 + 1] = static_cast<uint32_t>(val_r);
            tdm_tx_buffer[i * 8 + 2] = 0;
            tdm_tx_buffer[i * 8 + 3] = 0;
            tdm_tx_buffer[i * 8 + 4] = 0;
            tdm_tx_buffer[i * 8 + 5] = 0;
            tdm_tx_buffer[i * 8 + 6] = 0;
            tdm_tx_buffer[i * 8 + 7] = 0;
        }

        // 3. Direct DMA push
        size_t bytes_written = 0;
        i2s_channel_write(tx_chan, tdm_tx_buffer, sizeof(tdm_tx_buffer), &bytes_written, portMAX_DELAY);
    }
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "=== Initializing ESP32-P4 Faust DSP Runtime Engine ===");
    ESP_LOGI(TAG, "Golden Ratio Damping Active (Q = %.4f)", PHI_OPTIMAL_Q);

    ESP_ERROR_CHECK(init_tdm_hardware());

    // Pin DSP engine to Core 1 for dedicated real-time processing
    xTaskCreatePinnedToCore(dsp_audio_task, "faust_dsp_task", 8192, nullptr, 10, nullptr, 1);
}
