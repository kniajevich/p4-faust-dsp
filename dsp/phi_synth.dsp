declare name "PhiResonantSynth";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

// Golden Ratio Fundamentals
phi = 1.61803398875;
phi_inv = 0.61803398875;
q_phi = 1.0 / sqrt(phi);

// MIDI & Parameter Bindings
freq = hslider("freq [midi:pitch]", 440, 20, 10000, 0.1);
gain = hslider("gain [midi:ctrl 7]", 0.7, 0, 1, 0.01) : si.smoo;
cutoff = hslider("cutoff [midi:ctrl 74]", 1200, 50, 12000, 1) : si.smoo;
res = hslider("resonance [midi:ctrl 71]", q_phi, 0.1, 5.0, 0.01) : si.smoo;
gate = button("gate");

// Dual Oscillator with Phi Interval Overtones
osc1 = os.sawtooth(freq);
osc2 = os.sawtooth(freq * phi_inv);
voice = (osc1 * 0.6 + osc2 * 0.4) * en.adsr(0.01, 0.1, 0.8, 0.3, gate);

// Anti-standing resonant lowpass filter
process = voice * gain : fi.resonlp(cutoff, res, 1.0) <: _, _;
