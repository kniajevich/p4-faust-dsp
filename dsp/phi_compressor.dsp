declare name "PhiDynamicsCompressor";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

// Golden Ratio Time Tuning
phi = 1.61803398875;
phi_inv = 0.61803398875;

threshold = hslider("threshold [midi:ctrl 70]", -18.0, -60.0, 0.0, 0.1);
ratio     = hslider("ratio [midi:ctrl 71]", phi, 1.0, 20.0, 0.1);
attack    = hslider("attack [midi:ctrl 73]", 0.01, 0.001, 0.5, 0.001);
release   = hslider("release [midi:ctrl 72]", 0.01 * phi, 0.01, 1.5, 0.001);
makeup    = hslider("makeup [midi:ctrl 7]", 2.0, 0.0, 24.0, 0.1) : ba.db2linear : si.smoo;

// Dual-channel dynamic compression engine
process(l, r) = (l, r) : co.compressor_stereo(ratio, threshold, attack, release) : *(makeup), *(makeup);
