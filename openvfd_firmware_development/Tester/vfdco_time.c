/**
  ******************************************************************************
  * @file     vfdco_time.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the dummy driver to simulate time functionality
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "../vfdco_time.h"
#include <stdio.h>

void vfdco_get_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  date->y = 20;
  date->m = 10;
  date->d = 31; // Happy halloween

  time->h = 12;
  time->m = 13;
  time->s = 14;
}

void vfdco_set_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  printf("Dummy date/time set: %02hhu.%02hhu.20%02hhu - %02hhu:%02hhu:%02hhu",
    date->d, date->m, date->y, time->h, time->m, time->s
  );
}

time_event_t Time_Event_Init(long interval) {
  time_event_t iE;
  iE.interval = interval;
  iE.previousTime = 0;
  return iE;
}

void Time_Event_Reset(time_event_t *input) {
  input->previousTime = 0;
}

unsigned char Time_Event_Update(time_event_t *input) {
  // lel this is always true in simulation
  return 1;
}