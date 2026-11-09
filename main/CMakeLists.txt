idf_component_register(
    SRCS "main.cpp" "faust_dsp_core.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_i2s esp_timer freertos
)
