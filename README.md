# p4-faust-dsp: Unified Faust DSP, UAC2 & SIMD Platform for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
[![Deploy GitHub Pages Web Console](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/pages.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/pages.yml)
[![Release](https://img.shields.io/github/v/release/kniajevich/p4-faust-dsp?color=blue)](https://github.com/kniajevich/p4-faust-dsp/releases/latest)
[![Web Studio](https://img.shields.io/badge/Live%20Studio-GitHub%20Pages-brightgreen.svg)](https://kniajevich.github.io/p4-faust-dsp/)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Target: ESP32-P4](https://img.shields.io/badge/Target-ESP32--P4-red.svg)
![Architecture: RISC--V](https://img.shields.io/badge/Architecture-RISC--V%20SIMD-orange.svg)

An all-in-one embedded audio processing platform combining **Faust & Pure Data real-time DSP**, **USB Audio Class 2.0 (480 Mbps High-Speed)**, **Eurorack 1V/Oct CV/Gate**, **I2C Multichannel Codec control**, and **8-channel TDM hardware output**, designed exclusively for the dual-core **ESP32-P4 RISC-V SoC** (400 MHz).

---

## 🎛️ Live Web Studio & Controller

Direct browser control with zero software installation:  
👉 **[https://kniajevich.github.io/p4-faust-dsp/](https://kniajevich.github.io/p4-faust-dsp/)**

* **Touch XY-Pad (Kaoss Style):** Real-time dual-axis parameter modulation (Cutoff CC 74 + Resonance CC 71).
* **Polyphonic Touch Keyboard:** 17-key touch surface sending Note On/Off with velocity.
* **Real-Time Oscilloscope:** 60 FPS HTML5 Canvas waveform visualizer.
* **WebSerial Flasher:** Direct firmware flashing via Chrome or Edge.

---

## Unified System Architecture

```text
      +-------------------------------------------------------------+
      |                         ESP32-P4                            |
      |                                                             |
      |  [Core 0 - Control, USB & Telemetry]                        |
      |   +-------------------+        +-------------------------+  |
USB <====>| TinyUSB UAC2 &    |<======>| Phi RingBuffer Queue    |  |
(480M)|   | Web-MIDI Engine   |        | (Watermarks: 38% / 62%) |  |
      |   +-------------------+        +-------------------------+  |
      |   +-------------------+        +-------------------------+  |
      |   | Codec I2C Master  |        | Eurorack 1V/Oct CV &    |  |
      |   | (GPIO 7 / 8)      |        | Gate ISR (GPIO 1 / 9)   |  |
      |   +-------------------+        +-------------------------+  |
      |                                             ^               |
      |  [Core 1 - Real-Time Audio Engine]          |               |
      |   +-----------------------------------------v------------+  |
      |   | Faust / Pure Data C++ DSP Core (32-byte SIMD align)  |  |
      |   | RISC-V Vectorized Biquad & FMA Acceleration Engine   |  |
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

## Hardware Modular Integration & Manufacturing

* **Complete Bill of Materials (BOM):** [hardware/BOM.md](hardware/BOM.md) with LCSC & Mouser part numbers.
* **Eurorack 3U Shield Electrical Spec:** [hardware/eurorack_shield_spec.md](hardware/eurorack_shield_spec.md).
* **Flashing & Hardware Bring-Up:** [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md).

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
