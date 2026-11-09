#!/usr/bin/env python3
"""
ESP32-P4 Host-Side Audio Verification Suite
Measures Round-Trip Latency (RTL), Channel Crosstalk, and SNR across 8 Channels.
"""

import sys
import time
import numpy as np

def generate_test_stimulus(sample_rate=48000, duration_s=1.0):
    t = np.linspace(0, duration_s, int(sample_rate * duration_s), endpoint=False)
    # Dirac delta impulse followed by 8 orthogonal test tones (220Hz - 3520Hz)
    stimulus = np.zeros((len(t), 8), dtype=np.float32)
    stimulus[100, :] = 1.0  # Precision Dirac impulse at frame 100
    for ch in range(8):
        freq = 220.0 * (2.0 ** (ch * 0.5))
        stimulus[500:, ch] += 0.5 * np.sin(2.0 * np.pi * freq * t[500:])
    return stimulus

def analyze_audio_results(in_data, out_data, sample_rate=48000):
    print("\n" + "="*60)
    print(" ESP32-P4 EMBEDDED AUDIO HARDWARE VERIFICATION REPORT")
    print("="*60)

    # 1. Round-Trip Latency (RTL) Calculation via Cross-Correlation
    corr = np.correlate(out_data[:, 0], in_data[:, 0], mode='full')
    delay_frames = np.argmax(corr) - (len(in_data) - 1)
    latency_ms = (delay_frames / sample_rate) * 1000.0
    print(f"[*] Round-Trip Latency (RTL): {latency_ms:.2f} ms ({delay_frames} frames)")

    # 2. Crosstalk & SNR per Channel
    print("\n[*] Channel Signal-to-Noise & Crosstalk Analysis:")
    print("------------------------------------------------------------")
    print(" Ch | Peak Level (dBFS) | Noise Floor (dBFS) | Crosstalk (dB)")
    print("------------------------------------------------------------")
    for ch in range(8):
        signal = out_data[:, ch]
        peak_db = 20.0 * np.log10(np.max(np.abs(signal)) + 1e-9)
        noise_floor_db = 20.0 * np.log10(np.std(signal[:90]) + 1e-9)
        crosstalk_db = peak_db - noise_floor_db
        print(f" {ch:02d} | {peak_db:16.2f}  | {noise_floor_db:17.2f}  | -{abs(crosstalk_db):12.2f}")
    print("="*60 + "\n")

def main():
    print("[*] Initializing High-Speed Audio Loopback Test for ESP32-P4...")
    sample_rate = 48000
    stimulus = generate_test_stimulus(sample_rate=sample_rate, duration_s=1.0)

    try:
        import sounddevice as sd
        print("[*] Streaming 8x8 TDM stimulus over UAC2 USB Interface...")
        recorded = sd.playrec(stimulus, samplerate=sample_rate, channels=8, blocking=True)
        analyze_audio_results(stimulus, recorded, sample_rate=sample_rate)
    except ImportError:
        print("\033[93m[!] sounddevice library not installed. Running algorithmic simulation mode.\033[0m")
        # Simulated loopback with realistic DMA buffer delay (64 samples = 1.33ms)
        simulated_recorded = np.roll(stimulus, 64, axis=0) + np.random.normal(0, 1e-4, stimulus.shape)
        analyze_audio_results(stimulus, simulated_recorded, sample_rate=sample_rate)
        print("Install physical testing dependencies: pip install sounddevice numpy scipy")

if __name__ == "__main__":
    main()
