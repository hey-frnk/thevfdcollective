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

#ifndef _VFDCOLLECTIVE_DS3231_H
#define _VFDCOLLECTIVE_DS3231_H

#include "vfdco_time.h"

void vfdco_RTC_readDateTime(vfdco_date_t *d, vfdco_time_t *t);
void vfdco_RTC_setDateTime(vfdco_date_t *d, vfdco_time_t *t);

#endif

#ifdef __cplusplus
}
#endif
