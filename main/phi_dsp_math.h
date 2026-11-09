#pragma once

#include <cstddef>
#include <cstdint>

// Golden Ratio Fundamental Constants
constexpr float PHI_RATIO            = 1.618033988749895f;
constexpr float PHI_INV              = 0.618033988749895f;   // 1 / phi
constexpr float PHI_INV_SQ           = 0.381966011250105f;   // 1 / phi^2
constexpr float PHI_OPTIMAL_Q        = 0.786151377757423f;   // 1 / sqrt(phi) - non-modal resonant damping

// DSP Engine Configuration
constexpr size_t DSP_SAMPLE_RATE     = 48000;
constexpr size_t DSP_BLOCK_SIZE      = 64;
constexpr size_t DSP_NUM_CHANNELS    = 2;
constexpr size_t RING_BUFFER_FRAMES  = 256;

// Golden Ratio Watermark Boundaries for Elastic Queue Damping
constexpr size_t QUEUE_LOW_WATERMARK  = static_cast<size_t>(RING_BUFFER_FRAMES * PHI_INV_SQ); // ~38.2%
constexpr size_t QUEUE_HIGH_WATERMARK = static_cast<size_t>(RING_BUFFER_FRAMES * PHI_INV);    // ~61.8%
