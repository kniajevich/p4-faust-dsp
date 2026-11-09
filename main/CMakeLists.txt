idf_component_register(
    SRCS "main.cpp" "faust_dsp_core.cpp" "usb_descriptors.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_i2s esp_timer freertos esp_tinyusb
)
