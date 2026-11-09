declare name "PhiWavetableOsc";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

// Golden Ratio Fundamental Constants
phi = 1.61803398875;
phi_inv = 0.61803398875;
q_phi = 1.0 / sqrt(phi);

// Modulation and Parameter Controls
freq = hslider("freq [midi:pitch]", 220, 20, 8000, 0.1);
morph = hslider("morph [midi:ctrl 1]", 0.0, 0.0, 1.0, 0.001) : si.smoo;
gain = hslider("gain [midi:ctrl 7]", 0.8, 0, 1, 0.01) : si.smoo;
cutoff = hslider("cutoff [midi:ctrl 74]", 2400, 40, 14000, 1) : si.smoo;
gate = button("gate");

// Dual Morphing Oscillators with Golden Ratio Detuning
osc_fundamental = os.sawtooth(freq) * (1.0 - morph) + os.triangle(freq) * morph;
osc_harmonic    = os.sawtooth(freq * phi_inv) * (1.0 - morph) + os.square(freq * phi_inv) * morph;

// Interleaved Wave Blending with Non-Modal Resonant Filter
raw_voice = (osc_fundamental * phi_inv + osc_harmonic * (1.0 - phi_inv)) * en.adsr(0.005, 0.15, 0.7, 0.4, gate);
filtered_voice = raw_voice * gain : fi.resonlp(cutoff, q_phi, 1.0);

process = filtered_voice <: _, _;
