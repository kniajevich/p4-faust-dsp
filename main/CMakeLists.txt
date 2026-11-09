idf_component_register(
    SRCS "main.cpp" "faust_dsp_core.cpp" "codec_i2c.cpp" "cv_gate_driver.cpp" "usb_descriptors.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_driver_i2s esp_timer freertos esp_tinyusb esp_adc
)
