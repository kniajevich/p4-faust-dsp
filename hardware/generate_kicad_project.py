#!/usr/bin/env python3
"""
Automated KiCad 8 Project & Eurorack 10HP Front Panel Outline Generator
Outputs .kicad_sch and .kicad_pcb mechanical files for ESP32-P4 Eurorack Shield.
"""

import os

EURORACK_HEIGHT_MM = 128.5
EURORACK_10HP_WIDTH_MM = 50.5
MOUNT_SLOT_W_MM = 3.2
MOUNT_SLOT_H_MM = 4.2

def generate_pcb_outline(filename="hardware/esp32p4_shield.kicad_pcb"):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    w = EURORACK_10HP_WIDTH_MM
    h = EURORACK_HEIGHT_MM

    content = f"""(kicad_pcb (version 20240108) (generator p4_audio_shield_gen)
  (general (thickness 1.6))
  (paper "A4")
  (layers
    (0 "F.Cu" signal)
    (31 "B.Cu" signal)
    (32 "B.Adhes" user)
    (33 "F.Adhes" user)
    (34 "B.Paste" user)
    (35 "F.Paste" user)
    (36 "B.SilkS" user "SilkTop")
    (37 "F.SilkS" user "SilkBottom")
    (38 "B.Mask" user)
    (39 "F.Mask" user)
    (44 "Edge.Cuts" user)
  )
  (setup
    (pad_to_mask_clearance 0.05)
  )
  ;; 3U 10HP PCB Edge Cuts Outline
  (gr_line (start 0 0) (end {w} 0) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_line (start {w} 0) (end {w} {h}) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_line (start {w} {h}) (end 0 {h}) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_line (start 0 {h}) (end 0 0) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))

  ;; Standard Eurorack M3 Mounting Slots (Top & Bottom)
  (gr_circle (center 7.5 3.0) (end 9.1 3.0) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_circle (center {w - 7.5} 3.0) (end {w - 5.9} 3.0) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_circle (center 7.5 {h - 3.0}) (end 9.1 {h - 3.0}) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
  (gr_circle (center {w - 7.5} {h - 3.0}) (end {w - 5.9} {h - 3.0}) (stroke (width 0.15) (type solid)) (layer "Edge.Cuts"))
)
"""
    with open(filename, "w") as f:
        f.write(content)
    print(f"[*] Successfully generated KiCad PCB template: {filename}")

if __name__ == "__main__":
    generate_pcb_outline()
