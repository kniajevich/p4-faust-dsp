/*
 * Faust Architecture file for ESP32-P4 RISC-V Engine
 * Embeds compiled DSP into Kniajevich DSP Core
 */

#include <cmath>
#include <cstring>
#include "faust_dsp_core.hpp"

// Minimal Faust Abstract UI & Glue
struct UI {
    virtual ~UI() = default;
    virtual void addButton(const char* label, float* zone) {}
    virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) {}
    virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) {}
    virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) {}
};

struct Meta {
    virtual ~Meta() = default;
    virtual void declare(const char* key, const char* value) {}
};

<<includeIntrinsic>>
<<includeclass>>

static mydsp FAUST_GLOBAL_INSTANCE;

void FaustDspCore::init(uint32_t sample_rate) {
    m_sample_rate = sample_rate;
    FAUST_GLOBAL_INSTANCE.init(sample_rate);
}

void FaustDspCore::compute(size_t count, float** inputs, float** outputs) {
    FAUST_GLOBAL_INSTANCE.compute(count, inputs, outputs);
}

void FaustDspCore::setFrequency(float freq_hz) {
    (void)freq_hz;
}
