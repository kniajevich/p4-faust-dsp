#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>

// Golden Ratio Fundamental Constants
constexpr float PHI_RATIO            = 1.618033988749895f;
constexpr float PHI_INV              = 0.618033988749895f;   // 1 / phi
constexpr float PHI_INV_SQ           = 0.381966011250105f;   // 1 / phi^2
constexpr float PHI_OPTIMAL_Q        = 0.786151377757423f;   // 1 / sqrt(phi) - non-modal resonant damping

// DSP Engine Configuration
constexpr size_t DSP_SAMPLE_RATE     = 48000;
constexpr size_t DSP_BLOCK_SIZE      = 64;
constexpr size_t DSP_NUM_CHANNELS    = 8;
constexpr size_t RING_BUFFER_FRAMES  = 256;

// Golden Ratio Watermark Boundaries for Elastic Queue Damping
constexpr size_t QUEUE_LOW_WATERMARK  = static_cast<size_t>(RING_BUFFER_FRAMES * PHI_INV_SQ); // ~38.2%
constexpr size_t QUEUE_HIGH_WATERMARK = static_cast<size_t>(RING_BUFFER_FRAMES * PHI_INV);    // ~61.8%

// RISC-V Vector-Friendly Inlined DSP Routines
static inline void phi_simd_vector_scale_add(
    const float* __restrict__ src,
    float* __restrict__ dst,
    float scale,
    size_t count
) {
    // Auto-vectorized loop leveraging RISC-V SIMD pipeline via 32-byte alignment
    for (size_t i = 0; i < count; ++i) {
        dst[i] += src[i] * scale;
    }
}

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
