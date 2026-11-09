#include "codec_i2c.hpp"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "P4_CODEC_I2C";

esp_err_t p4_codec_i2c_init(void) {
    ESP_LOGI(TAG, "Initializing I2C Master on SDA: GPIO %d, SCL: GPIO %d", CODEC_I2C_SDA_PIN, CODEC_I2C_SCL_PIN);
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CODEC_I2C_SDA_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = CODEC_I2C_SCL_PIN;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = CODEC_I2C_FREQ_HZ;
    
    ESP_ERROR_CHECK(i2c_param_config(CODEC_I2C_PORT, &conf));
    return i2c_driver_install(CODEC_I2C_PORT, conf.mode, 0, 0, 0);
}

esp_err_t p4_codec_write_reg(uint8_t chip_addr, uint8_t reg, uint8_t val) {
    uint8_t write_buf[2] = { reg, val };
    esp_err_t ret = i2c_master_write_to_device(CODEC_I2C_PORT, chip_addr, write_buf, sizeof(write_buf), pdMS_TO_TICKS(100));
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Codec write failed (addr: 0x%02X, reg: 0x%02X, err: %s)", chip_addr, reg, esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t p4_codec_configure_tdm8(uint8_t chip_addr) {
    ESP_LOGI(TAG, "Configuring Multichannel Codec at 0x%02X for 8-channel TDM 24-bit 48kHz", chip_addr);
    // Generic un-mute and standard TDM frame sync setup sequence
    p4_codec_write_reg(chip_addr, 0x01, 0x00); // Power on / active mode
    p4_codec_write_reg(chip_addr, 0x02, 0x10); // Set TDM format mode
    p4_codec_write_reg(chip_addr, 0x03, 0x00); // Master unmute all DACs
    return ESP_OK;
}
