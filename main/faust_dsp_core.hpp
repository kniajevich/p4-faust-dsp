#pragma once

#include "phi_dsp_math.h"
#include <cstdint>

class FaustDspCore {
public:
    FaustDspCore();
    ~FaustDspCore() = default;

    void init(uint32_t sample_rate);
    void compute(size_t count, float** inputs, float** outputs);
    void setFrequency(float freq_hz);

private:
    uint32_t m_sample_rate;
    float m_phase;
    float m_phase_step;
    float m_q_factor;
    float m_feedback_state;
};
