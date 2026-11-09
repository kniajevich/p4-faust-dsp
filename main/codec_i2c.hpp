#pragma once

#include "esp_err.h"
#include <cstdint>

#define CODEC_I2C_PORT         I2C_NUM_0
#define CODEC_I2C_SDA_PIN      GPIO_NUM_7
#define CODEC_I2C_SCL_PIN      GPIO_NUM_8
#define CODEC_I2C_FREQ_HZ      100000

// Default I2C addresses for popular multichannel audio codecs
#define PCM3168A_I2C_ADDR_A    0x44
#define CS42448_I2C_ADDR       0x48
#define ES8388_I2C_ADDR        0x10

// Initialize ESP32-P4 I2C Master bus
esp_err_t p4_codec_i2c_init(void);

// Generic register write helper
esp_err_t p4_codec_write_reg(uint8_t chip_addr, uint8_t reg, uint8_t val);

// Preset initializer for 8-channel TDM Mode
esp_err_t p4_codec_configure_tdm8(uint8_t chip_addr);
