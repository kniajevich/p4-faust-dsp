declare name "Buchla292_LPG";
declare author "Alexandr Ovchinnikov (@kniajevich)";
declare license "GPLv3 / Commercial";

import("stdfaust.lib");

// Golden ratio tuning
phi = 1.61803398875;

// Buchla 292 Vactrol Lowpass Gate with Strike Impulse Dynamic
strike  = button("strike");
control = hslider("control [midi:ctrl 74]", 0.15, 0.0, 1.0, 0.001) : si.smoo;
damping = hslider("damping [midi:ctrl 71]", 0.4, 0.05, 1.0, 0.001) : si.smoo;

// Asymmetric Vactrol Response (fast light attack, non-linear chemical decay)
vactrol_impulse = en.ar(0.003, 0.22 * (1.0 + damping * phi), strike);
vactrol_state   = (control + vactrol_impulse) : min(1.0) : max(0.0);

// Simultaneous dynamic filter opening and VCA expansion
lpg_cutoff = 30.0 * pow(500.0, vactrol_state);
lpg_gain   = pow(vactrol_state, phi);

// Non-linear 12dB/oct optocoupler response
process(x) = x : fi.lowpass(2, lpg_cutoff) * lpg_gain;
