#pragma once

#include <cstddef>
#include <cstdint>

// Direct Form II Transposed Biquad Filter optimized for RISC-V
struct alignas(16) RiscvBiquadState {
    float s1;
    float s2;
};

struct alignas(16) RiscvBiquadCoeffs {
    float b0, b1, b2;
    float a1, a2;
};

// Dual-channel vectorized biquad step with unrolled FMA operations
static inline void riscv_biquad_process_block(
    const float* __restrict__ in_l,
    const float* __restrict__ in_r,
    float* __restrict__ out_l,
    float* __restrict__ out_r,
    size_t count,
    const RiscvBiquadCoeffs& c,
    RiscvBiquadState& state_l,
    RiscvBiquadState& state_r
) {
    float s1_l = state_l.s1;
    float s2_l = state_l.s2;
    float s1_r = state_r.s1;
    float s2_r = state_r.s2;

    #pragma GCC unroll 4
    for (size_t i = 0; i < count; ++i) {
        float x_l = in_l[i];
        float y_l = c.b0 * x_l + s1_l;
        s1_l = c.b1 * x_l - c.a1 * y_l + s2_l;
        s2_l = c.b2 * x_l - c.a2 * y_l;
        out_l[i] = y_l;

        float x_r = in_r[i];
        float y_r = c.b0 * x_r + s1_r;
        s1_r = c.b1 * x_r - c.a1 * y_r + s2_r;
        s2_r = c.b2 * x_r - c.a2 * y_r;
        out_r[i] = y_r;
    }

    state_l.s1 = s1_l; state_l.s2 = s2_l;
    state_r.s1 = s1_r; state_r.s2 = s2_r;
}
