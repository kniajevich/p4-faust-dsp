# ESP32-P4 Hardware Bring-Up & Flashing Guide

This guide details how to flash the compiled firmware binaries and wire external multichannel TDM audio codecs to the **ESP32-P4** SoC.

---

## 1. Hardware Pinout & Wiring

### TDM Multichannel Audio Bus (Zero-Jitter DMA)
| ESP32-P4 GPIO | Codec Signal | Function |
| :--- | :--- | :--- |
| **GPIO 12** | **BCLK** | Bit Clock ($48000 \\times 8 \\times 32 = 12.288\\text{ MHz}$) |
| **GPIO 13** | **WS / FS** | Frame Sync ($48\\text{ kHz}$) |
| **GPIO 11** | **DOUT** | Serial Data OUT (8 TDM Time Slots to DAC) |
| **GPIO 10** | **DIN** | Serial Data IN (8 TDM Time Slots from ADC) |

### Codec I2C Master Control Bus
| ESP32-P4 GPIO | Codec Signal | Notes |
| :--- | :--- | :--- |
| **GPIO 7** | **SDA** | Codec Register Data ($100\\text{ kHz}$, $4.7\\text{k}\\Omega$ pullup) |
| **GPIO 8** | **SCL** | Codec Register Clock ($4.7\\text{k}\\Omega$ pullup) |

### High-Speed USB Interface
* Connect the native USB-C connector directly to the ESP32-P4 High-Speed PHY (D+ / D- pins). No external USB PHY is required.

---

## 2. Flashing the Firmware

### Option A: Using ESP-IDF (Developer Mode)
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

### Option B: Standalone Flashing via `esptool.py` (Release Binaries)
Extract `esp32p4-faust-dsp-binaries.tar.gz` from the GitHub Release and flash:

```bash
esptool.py --chip esp32p4 -p /dev/ttyUSB0 -b 460800 \
  --before default_reset --after hard_reset write_flash \
  --flash_mode dio --flash_size 16MB --flash_freq 80m \
  0x0000 bootloader.bin \
  0x8000 partition-table.bin \
  0x10000 p4-faust-dsp.bin
```

---

## 3. Real-Time Telemetry & Boot Verification

Upon booting, open the serial terminal at **115200 baud** (`idf.py monitor` or `minicom -D /dev/ttyUSB0 -b 115200`). You should observe:

```text
I (120) P4_CODEC_I2C: Initializing I2C Master on SDA: GPIO 7, SCL: GPIO 8
I (135) P4_CODEC_I2C: Configuring Multichannel Codec at 0x44 for 8-channel TDM 24-bit 48kHz
I (150) P4_UNIFIED_MAIN: Initializing 8-Slot TDM Hardware Subsystem
I (162) P4_UNIFIED_MAIN: TinyUSB UAC2 High-Speed Subsystem online
I (180) P4_UNIFIED_MAIN: Faust DSP Core running isolated on Core 1 (Priority 10)
I (2180) P4_UNIFIED_MAIN: [DSP Telemetry] Core 1 Load: 1.84% | Cycles: 9812 / Peak: 10420 | Blocks: 750
```
