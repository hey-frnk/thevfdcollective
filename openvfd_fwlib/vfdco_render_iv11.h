#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_IV11RENDER_H
#define _VFDCOLLECTIVE_IV11RENDER_H

#include <stdint.h>
#include "vfdco_time.h"

// Given a time or date, the time or date will be sent to the display with optional delay in milliseconds
void        vfdco_render_time(vfdco_time_t *t, uint16_t delay);
void        vfdco_render_date(vfdco_date_t *d, uint16_t delay);

// Decimal dot overlay function: uint8_t dDotOverlay
// [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
// 7                                                                     0
void        vfdco_render_dDotOverlay(uint8_t dDotOverlay);

// Set brightness of single tube at index with the brightnessValue provided
void        vfdco_render_setBrightnessSingle(uint8_t index, uint16_t brightnessValue);
// Set brightness of all tubes with the brightnessValue provided
void        vfdco_render_setBrightnessAll(uint16_t brightnessValue);

#endif

#ifdef __cplusplus
}
#endif
