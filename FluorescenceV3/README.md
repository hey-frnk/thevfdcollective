This is the path for the firmware version 3 of Fluorescence, the 6 digit VFD digital clock.
The firmware can be configured to run on Arduino, STM32 and macOS/Windows/Linux using the simulator.

- Commons: Source files that are platform independent and used on all platforms (FL2/FL3/FLSim)
- FL2: Fluorescence/OpenVFD hardware version 2.x. Uses Arduino AVR platform, ATmega328P. To compile, run install.sh, open FL2.ino and hit upload!
- FL3: Fluorescence version 3.x. source files that are used in both FLClassic and FLDainty STM32 System Workbench projects
- FLClassic: Fluorescence version 3.x. STM32 System Workbench project that uses an STM32F042K6
- FLDainty: Fluorescence version 3.x. STM32 System Workbench project that uses an STM32F042F6
- FLSim: Fluorescence Simulator written in C++ with Qt GUI. Can be compiled on macOS, Windows and Linux