# Pure Data (Heavy Compiler) Integration

This platform supports real-time Pure Data (`.pd`) patches running directly on the ESP32-P4 RISC-V core via the **Heavy (hvcc)** framework.

Unlike interpreted `libpd`, Heavy compiles vanilla Pd patches into clean, deterministic, zero-allocation C/C++ code optimized for embedded SIMD pipelines.

---

## Quickstart: Compiling a Patch

1. Install `hvcc`:
   ```bash
   pip install hvcc
   ```

2. Compile `pd/phi_drone.pd` into native C source:
   ```bash
   python3 tools/compile_pd.py pd/phi_drone.pd
   ```

3. Build and flash the firmware:
   ```bash
   idf.py build flash
   ```
