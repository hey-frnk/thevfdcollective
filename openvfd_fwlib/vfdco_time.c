/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include "vfdco_time.h"

#ifdef USE_RTC_DS3231
#include "vfdco_ds3231.h"
#endif

#ifdef USE_CPU_STM32F042
#define millis() HAL_GetTick()
#endif

inline void vfdco_getDateTime(vfdco_date_t *d, vfdco_time_t *t) {
  vfdco_RTC_readDateTime(d, t);
}

inline void vfdco_setDateTime(vfdco_date_t *d, vfdco_time_t *t) {
  vfdco_RTC_setDateTime(d, t);
}

intervalEvent_t newiE(uint32_t p1){
  intervalEvent_t iE;
  iE.interval = p1;
  iE.previousMillis = 0;
  return iE;
}

uint8_t updateIntervalEvent(intervalEvent_t *input) {
  uint32_t currentMillis = millis();
  if((currentMillis - input->previousMillis) > input->interval) {
    input->previousMillis = currentMillis;
    return 1;
  }
  return 0;
}
