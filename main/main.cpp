#include <cstdio>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2s_tdm.h"
#include "tinyusb.h"
#include "tinyusb_default_config.h"
#include "tusb.h"
#include "faust_dsp_core.hpp"
#include "phi_dsp_math.h"

static const char *TAG = "P4_FAUST_MAIN";

// 32-byte SIMD vector-aligned audio buffers
alignas(32) static float dsp_out_left[DSP_BLOCK_SIZE];
alignas(32) static float dsp_out_right[DSP_BLOCK_SIZE];
alignas(32) static uint32_t tdm_tx_buffer[DSP_BLOCK_SIZE * 8];

static i2s_chan_handle_t tx_chan = nullptr;
static FaustDspCore dsp_engine;

static esp_err_t init_tdm_hardware() {
    ESP_LOGI(TAG, "Initializing 8-Slot TDM Hardware for Faust + UAC2 Pipeline");
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 8;
    chan_cfg.dma_frame_num = 128;
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, nullptr));

    const auto slot_mask = static_cast<i2s_tdm_slot_mask_t>(
        I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3 |
        I2S_TDM_SLOT4 | I2S_TDM_SLOT5 | I2S_TDM_SLOT6 | I2S_TDM_SLOT7
    );

    i2s_tdm_config_t tdm_cfg = {
        .clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(DSP_SAMPLE_RATE),
        .slot_cfg = I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO, slot_mask),
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
    ESP_LOGI(TAG, "Faust Real-Time DSP Engine locked on Core 1");

    dsp_engine.init(DSP_SAMPLE_RATE);
    float* outputs[2] = { dsp_out_left, dsp_out_right };

    while (true) {
        // 1. Process block inside Faust DSP engine
        dsp_engine.compute(DSP_BLOCK_SIZE, nullptr, outputs);

        // 2. Vectorized interleave into 8-channel TDM slot buffer using SIMD helpers
        phi_simd_interleave_to_q31(dsp_out_left, dsp_out_right, tdm_tx_buffer, DSP_BLOCK_SIZE);

        // 3. Push to hardware DMA
        size_t bytes_written = 0;
        i2s_channel_write(tx_chan, tdm_tx_buffer, sizeof(tdm_tx_buffer), &bytes_written, portMAX_DELAY);
    }
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "=== Starting ESP32-P4 Faust DSP + UAC2 Audio Hybrid Platform ===");
    ESP_LOGI(TAG, "Golden Ratio Damping Active (Q = %.4f, Watermarks = %.1f%% / %.1f%%)",
             PHI_OPTIMAL_Q, PHI_INV_SQ * 100.0f, PHI_INV * 100.0f);

    // 1. Initialize High-Speed TDM Hardware
    ESP_ERROR_CHECK(init_tdm_hardware());

    // 2. Initialize TinyUSB Device Stack (UAC2 480 Mbps Core)
    tinyusb_config_t const tusb_cfg = TINYUSB_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "TinyUSB UAC2 High-Speed Subsystem online");

    // 3. Pin Faust DSP real-time calculation to dedicated Core 1 (Priority 10)
    xTaskCreatePinnedToCore(dsp_audio_task, "faust_dsp_core1", 8192, nullptr, 10, nullptr, 1);
}
