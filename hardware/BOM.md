# Complete Bill of Materials (BOM): Eurorack 3U Shield for ESP32-P4

Reference component list for professional SMT assembly (JLCPCB / PCBWay) and manual prototyping.

---

## 1. Integrated Circuits (IC)

| Designator | Description | Package | Part Number | LCSC / Mouser |
| :--- | :--- | :--- | :--- | :--- |
| **U1** | 6-in / 8-out 24-bit 192 kHz TDM Audio Codec | HTSSOP-48 | **PCM3168APTR** | LCSC: C126815 |
| **U2, U3, U4** | Ultra-low Distortion Audio Operational Amplifiers | SOIC-8 | **OPA1612AIDR** | LCSC: C80970 |
| **U5** | Ultra-Low Noise +5V Positive LDO (1A) | QFN-20 | **TPS7A4700QRGWRQ1** | Mouser: 595-TPS7A4700QRGWRQ1 |
| **U6** | High-Accuracy -5V Negative LDO (1A) | QFN-20 | **TPS7A3301RGWR** | Mouser: 595-TPS7A3301RGWR |
| **U7** | High-Efficiency Step-Down Buck Regulator (+3.3V) | SOIC-8 | **LMR14030SDDAR** | LCSC: C83637 |
| **U8** | Dual Fast Voltage Comparator (Gate Threshold) | SOIC-8 | **LM393DR** | LCSC: C6230 |

---

## 2. Protection & Diodes

| Designator | Description | Package | Part Number |
| :--- | :--- | :--- | :--- |
| **D1, D2** | Reverse Polarity P-Ch Protection MOSFET | PowerPAK SO-8 | **DMP3010LPS-13** |
| **D3..D10** | Low Leakage Schottky Clamping Diodes (CV/Gate) | SOT-23 | **BAT54S** |
| **D11, D12** | Bidirectional TVS Diodes (USB D+/D- lines) | SOT-23 | **USBLC6-2SC6** |

---

## 3. Connectors & Mechanical

| Designator | Description | Manufacturer Part | Sourcing |
| :--- | :--- | :--- | :--- |
| **J1..J16** | 3.5mm Mono Switched Jack Sockets (Eurorack Audio/CV) | **WQX PJ-301M / Thonkiconn** | Thonk / LCSC |
| **J17** | Standard 10-pin / 16-pin Shrouded Eurorack Power Header | 2.54mm 2x5 / 2x8 Pin Box Header | Wurth / Multicomp |
| **J18** | 2x20 Pin Dual Row Female Header (ESP32-P4 Socket) | 2.54mm Pitch Socket Strip | Standard SMT/THT |
| **USB1** | USB-C 16-pin Receptacle (High-Speed UAC2 Interface) | **TYPE-C-31-M-12** | LCSC: C165948 |

---

## 4. Passive Network Guidelines

* **Resistors (Audio & CV dividers):** Thin film $0.1\%$ 0603 resistors for precision $1\text{V/Oct}$ tracking ($100\text{k}\Omega, 30\text{k}\Omega, 1.62\text{k}\Omega$).
* **Decoupling Capacitors:** Murata 10µF X7R 25V 0805 on power rails; 100nF C0G/NPO 0603 on audio op-amp local pins.
* **Power Chokes / Ferrite Beads:** Murata **BLM18PG121SN1D** rated for 2A saturation current.
