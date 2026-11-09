#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>
#include "esp_cpu.h"

// Golden Ratio Fundamental Constants
constexpr float PHI_RATIO            = 1.618033988749895f;
constexpr float PHI_INV              = 0.618033988749895f;   // 1 / phi
constexpr float PHI_INV_SQ           = 0.381966011250105f;   // 1 / phi^2
constexpr float PHI_OPTIMAL_Q        = 0.786151377757423f;   // 1 / sqrt(phi) - non-modal resonant damping

// Fibonacci Sequence Taps for FDN Reverb / Delay Networks
constexpr size_t FIBONACCI_TAPS[8]   = { 55, 89, 144, 233, 377, 610, 987, 1597 };

// DSP Engine Configuration
constexpr size_t DSP_SAMPLE_RATE     = 48000;
constexpr size_t DSP_BLOCK_SIZE      = 64;
constexpr size_t DSP_NUM_CHANNELS    = 8;
constexpr size_t RING_BUFFER_FRAMES  = 256;

// CPU Performance Calculations: Cycles available per block at 400 MHz (400M * 64 / 48000 = 533,333 cycles)
constexpr float CPU_MAX_BLOCK_CYCLES = (400000000.0f * static_cast<float>(DSP_BLOCK_SIZE)) / static_cast<float>(DSP_SAMPLE_RATE);

// Real-time RISC-V Telemetry Metrics
struct RiscvDspTelemetry {
    uint32_t last_cycles;
    uint32_t peak_cycles;
    float cpu_load_pct;
    uint32_t blocks_processed;
};

// SIMD Block Interleave Routine (Optimized for RISC-V)
static inline void phi_simd_interleave_to_q31(
    const float* __restrict__ left,
    const float* __restrict__ right,
    uint32_t* __restrict__ tdm_dst,
    size_t frames
) {
    for (size_t i = 0; i < frames; ++i) {
        float clamped_l = fmaxf(-1.0f, fminf(1.0f, left[i]));
        float clamped_r = fmaxf(-1.0f, fminf(1.0f, right[i]));

        tdm_dst[i * 8 + 0] = static_cast<uint32_t>(static_cast<int32_t>(clamped_l * 2147483647.0f));
        tdm_dst[i * 8 + 1] = static_cast<uint32_t>(static_cast<int32_t>(clamped_r * 2147483647.0f));
        tdm_dst[i * 8 + 2] = 0;
        tdm_dst[i * 8 + 3] = 0;
        tdm_dst[i * 8 + 4] = 0;
        tdm_dst[i * 8 + 5] = 0;
        tdm_dst[i * 8 + 6] = 0;
        tdm_dst[i * 8 + 7] = 0;
    }
}
