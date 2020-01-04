/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CLOCK_ROUTINES_H
#define _VFDCO_CLOCK_ROUTINES_H

// Number of digits and pixels
#define GLOBAL_NUM_DIGITS_NUM_PIXELS 6

// How often should we ask the RTC what time it is? (Milliseconds)
#define GLOBAL_TIME_UPDATE_INTERVAL 111

// Initialize function. Run first.
void    vfdco_clock_initializer();

// Human interface device (Buttons) routine
void    vfdco_clock_hid_initializer();
void    vfdco_clock_hid_routine();

// RTC time refresh routine
void    vfdco_clock_time_initializer();
void    vfdco_clock_time_routine();

// VFD display data render routine
void    vfdco_clock_display_initializer();
void    vfdco_clock_display_routine();

// VFD LED light illumination routine
void    vfdco_clock_lights_initializer();
void    vfdco_clock_lights_routine();

// Communication (Serial/USB, Serial/Bluetooth) routine
void    vfdco_clock_com_initializer();
void    vfdco_clock_com_routine();

#endif

#ifdef __cplusplus
}
#endif
