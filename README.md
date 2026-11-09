# p4-faust-dsp: Faust & Pure Data DSP Runtime for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Architecture: RISC--V P4](https://img.shields.io/badge/Architecture-RISC--V%20P4-orange.svg)

High-performance real-time Faust and Pure Data DSP synthesis engine optimized exclusively for the **ESP32-P4** dual-core RISC-V SoC (400 MHz).

---

## Key Architectural Features

* **RISC-V Vector-Aligned Buffering:** Audio scratchbuffers strictly aligned to 32-byte boundaries (`alignas(32)`) unlocking SIMD vectorization paths on ESP32-P4.
* **Phi-Resonant Mathematical Tuning ($\\phi \\approx 1.618$):** Non-modal filter damping ($Q = 1 / \\sqrt{\\phi} \\approx 0.7861$) and golden-ratio watermark ring buffering ($38.2\\% / 61.8\\%$) to eliminate standing resonant waves and buffer clicks.
* **Hardware TDM Audio Streamer:** Zero-copy DMA transmission of processed multi-channel blocks directly into multichannel DAC codecs.
* **Dedicated Core 1 Execution:** Audio calculation isolated to Core 1 at priority 10, keeping Core 0 free for UI, networking, or USB tasks.

---

## Dual Licensing & Commercial Inquiries

This project is distributed under a Dual-Licensing model:

1. **Open Source (GPLv3):** Free for personal experimentation, Eurorack DIY modules, academic research, and non-commercial open-source synthesis.
2. **Commercial / Closed-Source License:** Required if embedding this engine into proprietary synthesizers, commercial guitar processors, or closed-source DSP products.

For commercial licensing terms or custom DSP development: [@kniajevich](https://github.com/kniajevich).

---

## Support & Sponsorship

| Network / Asset | Address |
| :--- | :--- |
| **EVM** *(ETH, Arbitrum, Base, BSC, Polygon)* | `0xfd8029b4295d6f4d8AB77Bfc061BBaff4f143C44` |
| **USDT / Tron** *(TRC-20)* | `TQMmeTMudPqafdNqsRnwRGeupBNinLbLdt` |
| **Bitcoin** *(Native SegWit)* | `bc1q9rwm4p5vplqt5a0mdat3vmegm2kgk38zfzlfd7` |
| **Solana** *(SOL, SPL)* | `GKbakfAXmaEXZ4DPTgDJCiZTCtXe9a1qgT5q1Y1hXm35` |
| **Stellar** *(XLM)* | `GACXOMM26U4GL2JT3TEGS2MV5IF7O43Y7X2RGYRUZT2RWQODIOERRITD` |
