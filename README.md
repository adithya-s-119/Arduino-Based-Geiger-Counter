# Arduino-Based Geiger Counter

An Arduino Uno-based Geiger counter project that counts pulses from a Geiger-Muller tube and displays radiation readings on an SH1106 128x64 OLED.

> **Safety:** A Geiger-Muller tube requires a high-voltage supply. Use appropriate electrical-safety precautions when building or testing this project. This is an educational/experimental project, not a calibrated safety instrument.

## Actual firmware implementation

The original `Firmware.ino` is preserved as uploaded, without refactoring or behavioural changes. It implements:

- Pulse counting through external interrupt `0` on a falling edge
- SH1106 OLED output over SPI: MOSI `9`, clock `10`, DC `11`, CS `12`, reset `13`
- Digital input `A1` for resetting the current count
- 15-second measurement intervals and CPM calculation
- A CPM-to-uSv/hr calculation using the firmware's `divider` value of `151`
- Cumulative-count and total-uSv values
- A 61-entry count history and dead-time calculation

The sketch requires the Arduino core plus `Adafruit_SH1106` and `Adafruit_GFX` libraries.

## Supplied files

| File(s) | Purpose |
| --- | --- |
| `Firmware.ino` | Original Arduino sketch |
| `Buzzer.cpp`, `Buzzer.h` | Supplied buzzer class source |
| `Switchable.cpp`, `Switchable.h` | Supplied switchable-output base class |
| `gfxfont.h`, `glcdfont.c` | Supplied display-font source |
| `*.hex` | Supplied compiled firmware images |
| `Buzzer_license.txt`, `gpl-3.0.md` | Supplied licensing material |

`Firmware.ino` does not include or instantiate the supplied `Buzzer` or `Switchable` classes. They are retained unchanged as supporting files.

## Getting started

1. Verify the actual hardware connections before powering the circuit.
2. Install the required Arduino and Adafruit libraries.
3. Open `Firmware.ino` in an Arduino-compatible environment and compile for the intended board.
4. Validate and calibrate readings against an appropriate reference before relying on measurements.

## License notes

The supplied `Buzzer_license.txt` and `gpl-3.0.md` are retained unchanged. Review those notices before redistributing the supporting source files.
