#pragma once

#include "esp_err.h"
#include <cstdint>

#define EURORACK_GATE_PIN   GPIO_NUM_9
#define EURORACK_CV_ADC_CH  ADC_CHANNEL_0 // GPIO 1 on ESP32-P4

// Initializes ADC oneshot unit and Gate GPIO interrupts
esp_err_t cv_gate_init(void);

// Read 1V/Oct Pitch Voltage with Golden Ratio anti-aliasing filter
float cv_read_volt_octave_frequency(void);

// Check current Gate state (High = Note On, Low = Note Off)
bool gate_is_active(void);
