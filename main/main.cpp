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
#include "codec_i2c.hpp"

static const char *TAG = "P4_UNIFIED_MAIN";

// 32-byte SIMD vector-aligned audio buffers for RISC-V
alignas(32) static float dsp_out_left[DSP_BLOCK_SIZE];
alignas(32) static float dsp_out_right[DSP_BLOCK_SIZE];
alignas(32) static uint32_t tdm_tx_buffer[DSP_BLOCK_SIZE * 8];

static i2s_chan_handle_t tx_chan = nullptr;
static FaustDspCore dsp_engine;
static RiscvDspTelemetry g_telemetry = {};

static esp_err_t init_tdm_hardware() {
    ESP_LOGI(TAG, "Initializing 8-Slot TDM Hardware Subsystem");
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

// Core 1: Dedicated Real-Time DSP Audio Loop with Cycle Profiling
static void dsp_audio_task(void *pvParameters) {
    (void)pvParameters;
    ESP_LOGI(TAG, "Faust DSP Core running isolated on Core 1 (Priority 10)");

    dsp_engine.init(DSP_SAMPLE_RATE);
    float* outputs[2] = { dsp_out_left, dsp_out_right };

    while (true) {
        uint32_t start_cycles = esp_cpu_get_cycle_count();

        // 1. Execute DSP compute block
        dsp_engine.compute(DSP_BLOCK_SIZE, nullptr, outputs);

        // 2. Measure execution cycles and CPU load
        uint32_t elapsed_cycles = esp_cpu_get_cycle_count() - start_cycles;
        g_telemetry.last_cycles = elapsed_cycles;
        if (elapsed_cycles > g_telemetry.peak_cycles) g_telemetry.peak_cycles = elapsed_cycles;
        g_telemetry.cpu_load_pct = (static_cast<float>(elapsed_cycles) / CPU_MAX_BLOCK_CYCLES) * 100.0f;
        g_telemetry.blocks_processed++;

        // 3. Vectorized interleave into 8-channel TDM frame buffer
        phi_simd_interleave_to_q31(dsp_out_left, dsp_out_right, tdm_tx_buffer, DSP_BLOCK_SIZE);

        // 4. Zero-copy DMA transmission
        size_t bytes_written = 0;
        i2s_channel_write(tx_chan, tdm_tx_buffer, sizeof(tdm_tx_buffer), &bytes_written, portMAX_DELAY);
    }
}

// Core 0: Telemetry Logger & Watchdog Task
static void telemetry_task(void *pvParameters) {
    (void)pvParameters;
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP_LOGI(TAG, "[DSP Telemetry] Core 1 Load: %.2f%% | Cycles: %lu / Peak: %lu | Blocks: %lu",
                 g_telemetry.cpu_load_pct,
                 g_telemetry.last_cycles,
                 g_telemetry.peak_cycles,
                 g_telemetry.blocks_processed);
    }
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "=== Starting ESP32-P4 Faust DSP, UAC2 & RISC-V Audio Engine ===");
    ESP_LOGI(TAG, "Golden Ratio Tuning Active (Q = %.4f)", PHI_OPTIMAL_Q);

    // 1. Initialize Hardware I2C Master for External Codecs
    p4_codec_i2c_init();
    p4_codec_configure_tdm8(PCM3168A_I2C_ADDR_A);

    // 2. Initialize 8-Channel Hardware TDM Streamer
    ESP_ERROR_CHECK(init_tdm_hardware());

    // 3. Initialize TinyUSB Device Stack (UAC2 480 Mbps Core)
    tinyusb_config_t const tusb_cfg = TINYUSB_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    // 4. Launch isolated Real-Time DSP Audio Task on Core 1
    xTaskCreatePinnedToCore(dsp_audio_task, "dsp_core1_task", 8192, nullptr, 10, nullptr, 1);

    // 5. Launch Telemetry Logger on Core 0
    xTaskCreatePinnedToCore(telemetry_task, "telemetry_task", 3072, nullptr, 1, nullptr, 0);
}
