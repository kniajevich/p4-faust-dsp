# p4-faust-dsp: Unified Faust DSP, UAC2 & SIMD Platform for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Target: ESP32-P4](https://img.shields.io/badge/Target-ESP32--P4-red.svg)
![Architecture: RISC--V](https://img.shields.io/badge/Architecture-RISC--V%20SIMD-orange.svg)

An all-in-one embedded audio processing platform combining **Faust / Pure Data real-time DSP**, **USB Audio Class 2.0 (480 Mbps High-Speed)**, and **8-channel TDM hardware output**, designed exclusively for the dual-core **ESP32-P4 RISC-V SoC** (400 MHz).

---

## Unified System Architecture

```text
      +-------------------------------------------------------------+
      |                         ESP32-P4                            |
      |                                                             |
      |  [Core 0]                                                   |
      |   +-------------------+        +-------------------------+  |
USB <====>| TinyUSB UAC2 High |<======>| Phi RingBuffer Queue    |  |
(480M)|   | Speed Subsystem   |        | (Watermarks: 38% / 62%) |  |
      |   +-------------------+        +-------------------------+  |
      |                                             ^               |
      |  [Core 1]                                   |               |
      |   +-----------------------------------------v------------+  |
      |   | Faust C++ DSP Core (32-byte SIMD vector alignment)   |  |
      |   +------------------------------------------------------+  |
      |                                 |                           |
      |                                 v                           |
      |   +------------------------------------------------------+  |
      |   | Hardware I2S TDM 8-Slot Streamer (DMA to Multichannel)|  |
      +---|------------------------------------------------------|--+
                                        | (GPIO 10, 11, 12, 13)
                                        v
                           External Multichannel DAC/ADC
```

---

## Key Architectural Features

* **Monolithic Zero-Copy Audio Pipeline:** Direct bridge between UAC2 USB endpoints, Faust DSP registers, and hardware TDM DMA channels without CPU copy overhead.
* **Core Isolation:** Core 0 manages USB enumeration, descriptors, and I/O interrupts. Core 1 runs the Faust DSP compute loop with zero jitter.
* **RISC-V SIMD Optimization:** Audio scratchbuffers strictly aligned to 32-byte boundaries (`alignas(32)`) with vector dot-product and saturation inlines in `phi_dsp_math.h`.
* **Golden Ratio ($\\phi \\approx 1.618$) Dynamics:**
  * Filter damping: Non-modal $Q = 1 / \\sqrt{\\phi} \\approx 0.7861$.
  * Asymmetric queue watermarks: $38.2\\%$ and $61.8\\%$ preventing buffer oscillation.

---

## Compiling Faust `.dsp` Files

To compile any Faust DSP algorithm directly for this repository:

```bash
faust -a faust/architecture/esp32p4.cpp my_synth.dsp -o main/faust_dsp_core.cpp
```

---

## Dual Licensing

Distributed under the **GNU General Public License v3.0 (GPL-3.0)** for open-source and research use. Commercial, proprietary hardware and closed-source licenses are available upon inquiry: [@kniajevich](https://github.com/kniajevich).

---

## Support & Sponsorship

| Network / Asset | Address |
| :--- | :--- |
| **EVM** *(ETH, Arbitrum, Base, BSC, Polygon)* | `0xfd8029b4295d6f4d8AB77Bfc061BBaff4f143C44` |
| **USDT / Tron** *(TRC-20)* | `TQMmeTMudPqafdNqsRnwRGeupBNinLbLdt` |
| **Bitcoin** *(Native SegWit)* | `bc1q9rwm4p5vplqt5a0mdat3vmegm2kgk38zfzlfd7` |
| **Solana** *(SOL, SPL)* | `GKbakfAXmaEXZ4DPTgDJCiZTCtXe9a1qgT5q1Y1hXm35` |
| **Stellar** *(XLM)* | `GACXOMM26U4GL2JT3TEGS2MV5IF7O43Y7X2RGYRUZT2RWQODIOERRITD` |
