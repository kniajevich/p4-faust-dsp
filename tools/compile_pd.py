#!/usr/bin/env python3
"""
Pure Data (.pd) to ESP32-P4 C/C++ Transpilation Tool
Leverages Heavy (hvcc) for Zero-Allocation Real-Time Synthesis
Author: Alexandr Ovchinnikov (@kniajevich)
"""

import sys
import os
import subprocess

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 compile_pd.py <path_to_patch.pd>")
        print("Example: python3 compile_pd.py pd/phi_drone.pd")
        sys.exit(1)

    pd_file = sys.argv[1]
    if not os.path.isfile(pd_file):
        print(f"\033[91mError: Pure Data file '{pd_file}' not found.\033[0m")
        sys.exit(1)

    out_dir = "main/pd_generated"
    os.makedirs(out_dir, exist_ok=True)

    cmd = [
        "hvcc",
        pd_file,
        "-o", out_dir,
        "-n", "Heavy_P4",
        "-g", "c"
    ]

    print(f"Compiling Pure Data patch {pd_file} -> {out_dir} via Heavy...")
    try:
        subprocess.run(cmd, check=True)
        print("\033[92mSuccess: Pure Data C core generated in main/pd_generated!\033[0m")
    except FileNotFoundError:
        print("\033[91mError: 'hvcc' not found. Install it via: pip install hvcc\033[0m")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"\033[91mHeavy compilation failed with exit code {e.returncode}.\033[0m")
        sys.exit(e.returncode)

if __name__ == "__main__":
    main()
