#pragma once

#include <cstdint>
#include <cstddef>
#include "esp_err.h"
#include "esp_heap_caps.h"

// Multi-sample voice descriptor allocated in high-speed AXI PSRAM (16-32 MB)
struct PsramSample {
    const int16_t* data;
    size_t length_samples;
    uint32_t sample_rate;
    float base_frequency;
    bool loop_enabled;
    size_t loop_start;
    size_t loop_end;
};

class PsramRomplerEngine {
public:
    PsramRomplerEngine() : m_phase_accum(0), m_phase_inc(0), m_active(false) {}

    void setSample(const PsramSample& sample) {
        m_sample = sample;
        m_phase_accum = 0.0f;
        m_active = true;
    }

    void setPlaybackFrequency(float target_freq, uint32_t output_sample_rate) {
        if (m_sample.base_frequency > 0.0f) {
            m_phase_inc = (target_freq / m_sample.base_frequency) * (static_cast<float>(m_sample.sample_rate) / output_sample_rate);
        }
    }

    // Vectorized linear interpolation render into 32-bit DMA block
    void renderBlock(float* out_buffer, size_t num_samples) {
        if (!m_active || !m_sample.data) {
            for (size_t i = 0; i < num_samples; ++i) out_buffer[i] = 0.0f;
            return;
        }

        for (size_t i = 0; i < num_samples; ++i) {
            size_t idx0 = static_cast<size_t>(m_phase_accum);
            size_t idx1 = idx0 + 1;
            float frac = m_phase_accum - idx0;

            if (m_sample.loop_enabled && idx0 >= m_sample.loop_end) {
                m_phase_accum = m_sample.loop_start + (m_phase_accum - m_sample.loop_end);
                idx0 = static_cast<size_t>(m_phase_accum);
                idx1 = idx0 + 1;
            } else if (idx1 >= m_sample.length_samples) {
                m_active = false;
                for (; i < num_samples; ++i) out_buffer[i] = 0.0f;
                break;
            }

            float s0 = static_cast<float>(m_sample.data[idx0]) / 32768.0f;
            float s1 = static_cast<float>(m_sample.data[idx1]) / 32768.0f;
            out_buffer[i] = s0 + frac * (s1 - s0);
            m_phase_accum += m_phase_inc;
        }
    }

private:
    PsramSample m_sample = {};
    float m_phase_accum;
    float m_phase_inc;
    bool m_active;
};
