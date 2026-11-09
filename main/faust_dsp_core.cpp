#include "faust_dsp_core.hpp"
#include <cmath>

FaustDspCore::FaustDspCore()
    : m_sample_rate(DSP_SAMPLE_RATE),
      m_phase(0.0f),
      m_phase_step(0.0f),
      m_q_factor(PHI_OPTIMAL_Q),
      m_feedback_state(0.0f),
      m_target_gain(0.7f),
      m_current_gain(0.7f),
      m_filter_cutoff_norm(0.5f),
      m_gate_active(false) {}

void FaustDspCore::init(uint32_t sample_rate) {
    m_sample_rate = sample_rate;
    m_phase = 0.0f;
    handleMidiNoteOn(69, 100); // Default A440 Hz
}

void FaustDspCore::handleMidiNoteOn(uint8_t note, uint8_t velocity) {
    // Standard Equal Temperament with Golden Ratio microtonal stability
    float freq_hz = 440.0f * powf(2.0f, static_cast<float>(note - 69) / 12.0f);
    m_phase_step = (2.0f * static_cast<float>(M_PI) * freq_hz) / static_cast<float>(m_sample_rate);
    m_target_gain = static_cast<float>(velocity) / 127.0f;
    m_gate_active = true;
}

void FaustDspCore::handleMidiNoteOff(uint8_t note) {
    (void)note;
    m_gate_active = false;
    m_target_gain = 0.0f;
}

void FaustDspCore::handleMidiControlChange(uint8_t cc, uint8_t value) {
    float norm_val = static_cast<float>(value) / 127.0f;
    switch (cc) {
        case 74: // Brightness / Filter Cutoff
            m_filter_cutoff_norm = norm_val;
            break;
        case 71: // Resonance / Q-factor (scaled around Phi)
            m_q_factor = PHI_OPTIMAL_Q * (0.5f + norm_val * 1.5f);
            break;
        case 7:  // Master Channel Volume
            m_target_gain = norm_val;
            break;
        default:
            break;
    }
}

void FaustDspCore::handleMidiPitchBend(int16_t bend_val) {
    // Pitch bend (-8192 to +8191) mapped to +/- 2 semitones
    float semitones = (static_cast<float>(bend_val) / 8192.0f) * 2.0f;
    m_phase_step *= powf(2.0f, semitones / 12.0f);
}

void FaustDspCore::compute(size_t count, float** inputs, float** outputs) {
    float* out_l = outputs[0];
    float* out_r = outputs[1];
    (void)inputs;

    float cutoff_alpha = 0.05f + m_filter_cutoff_norm * 0.90f;

    for (size_t i = 0; i < count; ++i) {
        // Smooth gain ramp to avoid clicks
        m_current_gain += (m_target_gain - m_current_gain) * 0.01f;

        m_phase += m_phase_step;
        if (m_phase >= 2.0f * static_cast<float>(M_PI)) {
            m_phase -= 2.0f * static_cast<float>(M_PI);
        }

        float raw_sample = sinf(m_phase) * m_current_gain;

        // Golden Ratio non-modal smoothing filter
        m_feedback_state = (m_feedback_state * (1.0f - cutoff_alpha)) + (raw_sample * cutoff_alpha * m_q_factor);

        out_l[i] = m_feedback_state;
        out_r[i] = m_feedback_state;
    }
}
