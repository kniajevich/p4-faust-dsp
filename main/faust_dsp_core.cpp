#include "faust_dsp_core.hpp"
#include <cmath>

FaustDspCore::FaustDspCore()
    : m_sample_rate(DSP_SAMPLE_RATE),
      m_phase(0.0f),
      m_phase_step(0.0f),
      m_q_factor(PHI_OPTIMAL_Q),
      m_feedback_state(0.0f) {}

void FaustDspCore::init(uint32_t sample_rate) {
    m_sample_rate = sample_rate;
    m_phase = 0.0f;
    setFrequency(440.0f);
}

void FaustDspCore::setFrequency(float freq_hz) {
    m_phase_step = (2.0f * static_cast<float>(M_PI) * freq_hz) / static_cast<float>(m_sample_rate);
}

void FaustDspCore::compute(size_t count, float** inputs, float** outputs) {
    float* out_l = outputs[0];
    float* out_r = outputs[1];
    (void)inputs;

    // Block computation with phi-tuned harmonic resonance damping
    for (size_t i = 0; i < count; ++i) {
        m_phase += m_phase_step;
        if (m_phase >= 2.0f * static_cast<float>(M_PI)) {
            m_phase -= 2.0f * static_cast<float>(M_PI);
        }

        float raw_sample = sinf(m_phase);
        // Golden Ratio aperiodic resonant smoothing filter
        m_feedback_state = (m_feedback_state * PHI_INV) + (raw_sample * (1.0f - PHI_INV));

        out_l[i] = m_feedback_state;
        out_r[i] = m_feedback_state;
    }
}
