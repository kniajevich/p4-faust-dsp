# p4-faust-dsp: Unified Faust DSP, UAC2 & SIMD Platform for ESP32-P4

[![ESP32-P4 Faust DSP CI](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml/badge.svg)](https://github.com/kniajevich/p4-faust-dsp/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/kniajevich/p4-faust-dsp?color=blue)](https://github.com/kniajevich/p4-faust-dsp/releases/latest)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Target: ESP32-P4](https://img.shields.io/badge/Target-ESP32--P4-red.svg)
![Architecture: RISC--V](https://img.shields.io/badge/Architecture-RISC--V%20SIMD-orange.svg)

An all-in-one embedded audio processing platform combining **Faust & Pure Data real-time DSP**, **USB Audio Class 2.0 (480 Mbps High-Speed)**, **Eurorack 1V/Oct CV/Gate**, **I2C Multichannel Codec control**, and **8-channel TDM hardware output**, designed exclusively for the dual-core **ESP32-P4 RISC-V SoC** (400 MHz).

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

## Hardware Pinout (ESP32-P4)

| Signal | ESP32-P4 GPIO | Description |
| :--- | :--- | :--- |
| **TDM BCLK** | GPIO 12 | Bit Clock ($48000 \\times 8 \\times 32 = 12.288\\text{ MHz}$) |
| **TDM WS / FS** | GPIO 13 | Frame Sync ($48\\text{ kHz}$) |
| **TDM DOUT** | GPIO 11 | Serial Audio Data Output (8 Time Slots to DAC) |
| **TDM DIN** | GPIO 10 | Serial Audio Data Input (8 Time Slots from ADC) |
| **I2C SDA** | GPIO 7 | Multichannel Codec Control Data (PCM3168A / CS42448) |
| **I2C SCL** | GPIO 8 | Multichannel Codec Control Clock |
| **Eurorack CV In** | GPIO 1 (ADC1_CH0) | 1V/Oct Analog Pitch Control ($0-3.3\\text{V}$) |
| **Eurorack Gate** | GPIO 9 | Note Trigger Interrupt (Active High) |
| **USB D+ / D-** | On-chip HS PHY | Direct connection to USB-C port (480 Mbps) |

---

## Verification & Control Utilities

* **Host Audio Verification Suite:** `python3 tools/audio_test_suite.py` measures Round-Trip Latency (RTL) and channel crosstalk.
* **Web Control & Flasher Interface:** Open `web/index.html` in Chrome or Edge to control parameters via Web-MIDI and flash binaries via WebSerial.

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
