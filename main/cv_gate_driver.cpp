#include "cv_gate_driver.hpp"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "phi_dsp_math.h"
#include <cmath>

static const char *TAG = "P4_CV_GATE";
static adc_oneshot_unit_handle_t adc1_handle = nullptr;
static volatile bool s_gate_active = false;
static float s_smoothed_adc_norm = 0.0f;

static void IRAM_ATTR gate_isr_handler(void* arg) {
    (void)arg;
    s_gate_active = gpio_get_level(EURORACK_GATE_PIN) == 1;
}

esp_err_t cv_gate_init(void) {
    ESP_LOGI(TAG, "Initializing Eurorack CV/Gate Interfaces");

    // 1. Configure Gate Interrupt Pin
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = (1ULL << EURORACK_GATE_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(EURORACK_GATE_PIN, gate_isr_handler, nullptr);
    s_gate_active = gpio_get_level(EURORACK_GATE_PIN) == 1;

    // 2. Configure 1V/Oct ADC Channel
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EURORACK_CV_ADC_CH, &config));

    return ESP_OK;
}

float cv_read_volt_octave_frequency(void) {
    int raw_adc = 0;
    if (adc1_handle && adc_oneshot_read(adc1_handle, EURORACK_CV_ADC_CH, &raw_adc) == ESP_OK) {
        float raw_norm = static_cast<float>(raw_adc) / 4095.0f;
        // Golden Ratio aperiodic EMA filter for ADC noise cancellation
        s_smoothed_adc_norm = (s_smoothed_adc_norm * PHI_INV) + (raw_norm * (1.0f - PHI_INV));
    }
    // 1V/Oct scaling: 0.0V to 3.3V spanning 5 Octaves around C3 (130.81 Hz)
    float octaves = s_smoothed_adc_norm * 5.0f;
    return 130.81f * powf(2.0f, octaves);
}

bool gate_is_active(void) {
    return s_gate_active;
}
