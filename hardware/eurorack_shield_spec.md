# Eurorack 3U / 10HP Hardware Shield Specification

This specification details the reference hardware implementation for connecting the **ESP32-P4** SoC to modular synthesizer racks using the standard **Eurorack 10-pin/16-pin power bus**.

---

## 1. Module Overview

* **Form Factor:** Eurorack 3U, 10HP width (50.5 mm), maximum depth 35 mm.
* **Digital Audio Core:** ESP32-P4 RISC-V Dual-Core running at 400 MHz.
* **Codec Subsystem:** Texas Instruments **PCM3168A** (6-in / 8-out 24-bit 192 kHz audio codec connected over 8-slot TDM and I2C Master).
* **Control Connectivity:** High-Speed USB 2.0 Type-C (UAC2 + Web-MIDI / WebSerial), 4x Analog CV Inputs (1V/Oct calibrated), 2x Gate/Trigger Inputs, 8x DC-coupled Audio I/O jacks.

---

## 2. Power Supply Architecture

Eurorack standard supplies $+12\text{V}$, $-12\text{V}$, and optionally $+5\text{V}$. The shield generates clean, ultra-low-noise local rails:

```text
 +12V Eurorack ---> [ TPS7A4700 LDO ] ------------> +5V Analog Rail (Op-Amps)
 -12V Eurorack ---> [ TPS7A3301 Negative LDO ] ---> -5V Analog Rail (Op-Amps)
 +12V Eurorack ---> [ Buck Regulator LMR14030 ] --> +3.3V Digital (ESP32-P4 & Codec)
```

* **Reverse Polarity Protection:** Low $V_f$ P-Channel MOSFETs (e.g., DMP3010LPS) on $\pm 12\text{V}$ rails.
* **Ferrite Beads:** Murata BLM18PG series on analog power rails before codec decoupling networks.

---

## 3. Analog Conditioning & CV/Gate Circuitry

### 1V/Oct CV Input Conditioning (0V to 10V Eurorack -> 0V to 3.0V ADC)
To interface standard Eurorack modular signals with the ESP32-P4 SAR ADC (`GPIO 1`):
1. Precision op-amp inverting summer with $0.1\%$ tolerance resistors ($R_1 = 100\text{k}\Omega, R_f = 30\text{k}\Omega$).
2. Dual Schottky clamp diodes (BAT54S) connected to $+3.3\text{V}$ and $\text{GND}$ to prevent overvoltage damage during hot-plugging.
3. $\phi$-tuned anti-aliasing RC lowpass filter ($R = 1.62\text{k}\Omega, C = 10\text{nF}$, $f_c \approx 9.8\text{ kHz}$).

### Gate / Trigger Detect (0V to +10V Eurorack -> 3.3V Logic Level)
1. Input series resistor: $47\text{k}\Omega$.
2. NPN transistor (MMBT3904) or comparator (LM393) configured with a $+1.5\text{V}$ Schmitt trigger threshold.
3. Open-collector output pulled up to $+3.3\text{V}$ routed directly to `GPIO 9` for instant hardware edge interrupts.

---

## 4. Audio I/O Conditioning (PCM3168A)

* **Inputs (6 Channels):** Inverting unity-gain buffers utilizing **OPA1612** or **TL074** op-amps, converting Eurorack $\pm 5\text{V}$ ($10\text{V}_{pp}$) signals to the differential $2.8\text{V}_{pp}$ input range of the PCM3168A.
* **Outputs (8 Channels):** Differential-to-single-ended reconstruction filters with DC-coupling capability, allowing the module to act as an audio synthesizer, LFO bank, or modulation source.

---

## 5. Front Panel Pin Mapping

| Jack Label | Signal Type | Target Pin | Voltage Range |
| :--- | :--- | :--- | :--- |
| **CV 1 (Pitch)** | Analog Input | GPIO 1 (ADC1_CH0) | $0\text{V}$ to $+10\text{V}$ ($1\text{V/Oct}$) |
| **CV 2 (Cutoff)** | Analog Input | GPIO 2 (ADC1_CH1) | $-5\text{V}$ to $+5\text{V}$ |
| **GATE 1** | Digital Input | GPIO 9 (ISR) | $0\text{V}$ to $+10\text{V}$ (Active High) |
| **TRIG 2** | Digital Input | GPIO 4 (ISR) | $0\text{V}$ to $+10\text{V}$ (Active High) |
| **AUDIO OUT 1..8** | Analog Output | PCM3168A DAC 1..8 | $\pm 5\text{V}$ ($10\text{V}_{pp}$) |
| **USB-C** | Digital I/O | On-Chip HS D+/D- | USB 2.0 High-Speed |
