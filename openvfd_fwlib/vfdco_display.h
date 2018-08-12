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

#ifndef _VFDCOLLECTIVE_DISPLAY_H
#define _VFDCOLLECTIVE_DISPLAY_H

#include "vfdco_config.h"

void vfdco_displayWrite(const uint8_t *data, const uint8_t *bcmp, uint8_t dataLength, uint8_t *bpos);

#endif

#ifdef __cplusplus
}
#endif
