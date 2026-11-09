# p4-faust-dsp: Unified Faust DSP, UAC2 & SIMD Platform for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/kniajevich/p4-faust-dsp?color=blue)](https://github.com/kniajevich/p4-faust-dsp/releases/latest)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Target: ESP32-P4](https://img.shields.io/badge/Target-ESP32--P4-red.svg)
![Architecture: RISC--V](https://img.shields.io/badge/Architecture-RISC--V%20SIMD-orange.svg)

An all-in-one embedded audio processing platform combining **Faust & Pure Data real-time DSP**, **USB Audio Class 2.0 (480 Mbps High-Speed)**, **MIDI CC/Pitch modulation**, **I2C Multichannel Codec control**, and **8-channel TDM hardware output**, designed exclusively for the dual-core **ESP32-P4 RISC-V SoC** (400 MHz).

---

## Unified System Architecture

```text
      +-------------------------------------------------------------+
      |                         ESP32-P4                            |
      |                                                             |
      |  [Core 0 - Control & Telemetry]                             |
      |   +-------------------+        +-------------------------+  |
USB <====>| TinyUSB UAC2 &    |<======>| Phi RingBuffer Queue    |  |
(480M)|   | MIDI Subsystem    |        | (Watermarks: 38% / 62%) |  |
      |   +-------------------+        +-------------------------+  |
      |   +-------------------+        +-------------------------+  |
      |   | Codec I2C Master  |        | RISC-V Cycle Telemetry  |  |
      |   | (GPIO 7 / 8)      |        | (% DSP Load Monitoring) |  |
      |   +-------------------+        +-------------------------+  |
      |                                             ^               |
      |  [Core 1 - Real-Time Audio Engine]          |               |
      |   +-----------------------------------------v------------+  |
      |   | Faust / Pure Data C++ DSP Core (32-byte SIMD align)  |  |
      |   | Dynamic MIDI CC & Pitch Modulation Interface         |  |
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

## Hardware Documentation & Bring-Up

See [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md) for full schematic wiring, power isolation guidelines, and flashing via `esptool.py`.

---

## MIDI CC Default Mapping

| Controller | Function | Range / Mapping |
| :--- | :--- | :--- |
| **Note On / Off** | Pitch Tracking | $f = 440 \\cdot 2^{(note - 69) / 12}$ Hz |
| **Pitch Bend** | Microtonal Detune | $\\pm 2$ Semitones |
| **CC 74** | Cutoff Frequency | Lowpass Filter Cutoff ($50\\text{ Hz} - 12\\text{ kHz}$) |
| **CC 71** | Resonance / Q | Scaled around Golden Ratio ($Q = 1 / \\sqrt{\\phi} \\approx 0.7861$) |
| **CC 7** | Master Gain | $0.0 - 1.0$ (S-curve smoothed) |

---

## Compiling Faust & Pure Data Algorithms

### Faust (.dsp)
```bash
python3 tools/compile_faust.py dsp/phi_synth.dsp
```

### Pure Data (.pd via Heavy)
See [pd/README.md](pd/README.md):
```bash
python3 tools/compile_pd.py pd/phi_drone.pd
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
