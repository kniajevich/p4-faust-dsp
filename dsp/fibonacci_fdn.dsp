declare name "FibonacciFDN";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

phi_inv = 0.61803398875;
feedback = hslider("feedback [midi:ctrl 1]", 0.618, 0.0, 0.95, 0.001) : si.smoo;
damping = hslider("damping [midi:ctrl 74]", 0.382, 0.0, 0.9, 0.001) : si.smoo;

// 8 Fibonacci prime-distributed delay taps (samples)
d1 = 55;  d2 = 89;  d3 = 144; d4 = 233;
d5 = 377; d6 = 610; d7 = 987; d8 = 1597;

delay_tap(d) = de.delay(2048, d) : *(1.0 - damping);

// 8-channel non-modal spatial matrix
process = ro.cross(8) : par(i, 8, delay_tap(d1)) : par(i, 8, *(feedback));
