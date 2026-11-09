# p4-faust-dsp: Unified Faust DSP, UAC2 & SIMD Platform for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/kniajevich/p4-faust-dsp?color=blue)](https://github.com/kniajevich/p4-faust-dsp/releases/latest)
[![Web Console](https://img.shields.io/badge/Web%20Console-GitHub%20Pages-brightgreen.svg)](https://kniajevich.github.io/p4-faust-dsp/)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Target: ESP32-P4](https://img.shields.io/badge/Target-ESP32--P4-red.svg)
![Architecture: RISC--V](https://img.shields.io/badge/Architecture-RISC--V%20SIMD-orange.svg)

An all-in-one embedded audio processing platform combining **Faust & Pure Data real-time DSP**, **USB Audio Class 2.0 (480 Mbps High-Speed)**, **Eurorack 1V/Oct CV/Gate**, **I2C Multichannel Codec control**, and **8-channel TDM hardware output**, designed exclusively for the dual-core **ESP32-P4 RISC-V SoC** (400 MHz).

---

## Live Web Console & Flasher

Access the browser-based control surface and flasher directly at:  
👉 **[https://kniajevich.github.io/p4-faust-dsp/](https://kniajevich.github.io/p4-faust-dsp/)**  
*(Requires Chrome or Edge for WebSerial flashing and Web-MIDI parameter control)*.

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

## Eurorack Hardware Modular Integration

* Complete schematic and PCB design guide: [hardware/eurorack_shield_spec.md](hardware/eurorack_shield_spec.md)
* Direct hardware bring-up and flashing steps: [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md)

---

## Included Faust DSP Presets

| Preset File | Description | Controls |
| :--- | :--- | :--- |
| `dsp/phi_synth.dsp` | Polyphonic dual-oscillator synthesizer with anti-standing filter | Pitch, Cutoff (CC 74), Resonance (CC 71) |
| `dsp/phi_wavetable.dsp` | Dual morphing wavetable synth with $\phi$-overtone balance | Morph (CC 1), Cutoff (CC 74), Gain (CC 7) |
| `dsp/phi_compressor.dsp` | Golden ratio stereo dynamics mastering compressor | Threshold (CC 70), Ratio (CC 71), Gain (CC 7) |
| `dsp/fibonacci_fdn.dsp` | 8-channel spatial Feedback Delay Network reverb | Feedback (CC 1), Damping (CC 74) |

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
