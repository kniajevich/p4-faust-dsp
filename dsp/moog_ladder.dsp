declare name "Moog24dB_Ladder";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

cutoff = hslider("cutoff [midi:ctrl 74]", 1200, 20, 18000, 1) : si.smoo;
res    = hslider("resonance [midi:ctrl 71]", 0.6, 0.0, 1.02, 0.001) : si.smoo;
drive  = hslider("drive [midi:ctrl 1]", 1.2, 0.5, 4.0, 0.01) : si.smoo;

// 4-Pole 24dB/oct Transistor Ladder Filter with hyperbolic tangent saturation
process(x) = (x * drive) : ve.moogLadder(cutoff, res * 4.0) : ma.tanh;
