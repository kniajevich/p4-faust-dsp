#pragma once

#include "phi_dsp_math.h"
#include <cstdint>

class FaustDspCore {
public:
    FaustDspCore();
    ~FaustDspCore() = default;

    void init(uint32_t sample_rate);
    void compute(size_t count, float** inputs, float** outputs);
    
    // MIDI Integration and Dynamic Modulation Interface
    void handleMidiNoteOn(uint8_t note, uint8_t velocity);
    void handleMidiNoteOff(uint8_t note);
    void handleMidiControlChange(uint8_t cc, uint8_t value);
    void handleMidiPitchBend(int16_t bend_val);

private:
    uint32_t m_sample_rate;
    float m_phase;
    float m_phase_step;
    float m_q_factor;
    float m_feedback_state;
    float m_target_gain;
    float m_current_gain;
    float m_filter_cutoff_norm;
    bool  m_gate_active;
};
