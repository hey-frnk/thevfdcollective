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

#ifndef _VFDCOLLECTIVE_TIME_H
#define _VFDCOLLECTIVE_TIME_H

#include "vfdco_config.h"

typedef struct vfdco_time {
  uint8_t hr, min, sec;
} vfdco_time_t;

typedef struct vfdco_date {
  uint8_t day, month, year;
} vfdco_date_t;

void vfdco_getDateTime(vfdco_date_t *d, vfdco_time_t *t);
void vfdco_setDateTime(vfdco_date_t *d, vfdco_time_t *t);

typedef enum INTERVALEVENT_STATE {
  INTERVALEVENT_NOTELAPSED,
  INTERVALEVENT_ELAPSED
} INTERVALEVENT_STATE_t;

typedef struct intervalEvent{
  uint32_t interval;
  uint32_t previousMillis;
} intervalEvent_t;

// Non blocking, non interrupting timer for less time critical timed functions
intervalEvent_t newiE(uint32_t p1);
uint8_t updateIntervalEvent(intervalEvent_t *input);

#endif

#ifdef __cplusplus
}
#endif
