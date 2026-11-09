#!/usr/bin/env python3
"""
Faust DSP to ESP32-P4 C++ Compilation Tool
Author: Alexandr Ovchinnikov (@kniajevich)
"""

import sys
import os
import subprocess

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 compile_faust.py <path_to_dsp_file.dsp>")
        print("Example: python3 compile_faust.py dsp/phi_synth.dsp")
        sys.exit(1)

    dsp_file = sys.argv[1]
    if not os.path.isfile(dsp_file):
        print(f"Error: File '{dsp_file}' not found.")
        sys.exit(1)

    output_file = "main/faust_dsp_core.cpp"
    arch_file = "faust/architecture/esp32p4.cpp"

    cmd = [
        "faust",
        "-a", arch_file,
        "-vec", "-vs", "64",
        "-double", "0",
        dsp_file,
        "-o", output_file
    ]

    print(f"Compiling {dsp_file} -> {output_file} using ESP32-P4 architecture...")
    try:
        subprocess.run(cmd, check=True)
        print("\033[92mSuccess: Faust core generated and ready for ESP-IDF build!\033[0m")
    except FileNotFoundError:
        print("\033[91mError: 'faust' compiler executable not found in PATH.\033[0m")
        print("Install Faust via: apt install faust / brew install faust")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"\033[91mCompilation error (code {e.returncode}).\033[0m")
        sys.exit(e.returncode)

if __name__ == "__main__":
    main()
