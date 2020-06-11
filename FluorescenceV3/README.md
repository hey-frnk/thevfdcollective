This is the path for the firmware version 3 of Fluorescence, the 6 digit VFD digital clock.
The firmware can be configured to run on Arduino, STM32 and macOS/Windows/Linux using the simulator.

- Commons: Source files that are platform independent and used across and compatible to all platforms (FL2/FL3/FLSim)
- FL2:    Fluorescence hardware version 2.x. Uses Arduino AVR platform, ATmega328P. To compile, run install.sh, open FL2.ino in Arduino IDE and hit upload!
- FL3:    Fluorescence hardware version 3.x. Uses STM32 platform, STM32L412. To compile, open project in System Workbench for STM32 and hit build/debug/run!
- FLSim:  Fluorescence Simulator. Written in C++ with Qt GUI. Can be compiled on macOS, Windows and Linux