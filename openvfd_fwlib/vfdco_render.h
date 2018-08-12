/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_IV11RENDER_H
#define _VFDCOLLECTIVE_IV11RENDER_H

#include "vfdco_config.h"
#include "vfdco_time.h"

#define NUMOFTUBES 6

enum DATE_MODE{
  DATE_MODE_DDMMYY,
  DATE_MODE_MMDDYY
};

// Internal ISR of brightness and content PWM handler. Call at high frequency for flickerless display
void        vfdco_render();

uint8_t     vfdco_render_charToSevenSeg(char input);

// Given a time or date, the time or date will be sent to the display
// Decimal dot overlay function: uint8_t dDotOverlay
// [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
// 7                                                                     0
void        vfdco_render_time(vfdco_time_t *t, uint8_t dDotOverlay);
void        vfdco_render_date(vfdco_date_t *d, uint8_t dDotOverlay, enum DATE_MODE dm);

void        vfdco_render_message(const char *message, uint8_t dDotOverlay);

// Set brightness of single tube at index with the brightnessValue provided
void        vfdco_render_setBrightnessSingle(uint8_t index, uint8_t brightnessValue);
// Set brightness of all tubes with the brightnessValue provided
void        vfdco_render_setBrightnessAll(uint8_t brightnessValue);

// Internal ISR of brightness PWM handler. Call at ~5 kHz
// void        _vfdco_render_brightnessUpdater();

#endif

#ifdef __cplusplus
}
#endif
