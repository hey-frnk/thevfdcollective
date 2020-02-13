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
#include <time.h>

uint32_t _vfdco_time_get_milliseconds() { return 42; }
void _vfdco_time_delay_milliseconds(uint32_t delay) { printf("Dummy delay of %u milliseconds.\n", delay); }

uint32_t (*vfdco_time_get_milliseconds)(void) = _vfdco_time_get_milliseconds;
void (*vfdco_time_delay_milliseconds)(uint32_t) = _vfdco_time_delay_milliseconds;

void vfdco_get_date_time(vfdco_date_t *_date, vfdco_time_t *_time) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  _date->y = 20;
  _date->m = timeinfo->tm_mon + 1;
  _date->d = timeinfo->tm_mday; // Happy halloween
  _time->h = timeinfo->tm_hour;
  _time->m = timeinfo->tm_min;
  _time->s = timeinfo->tm_sec;
}

void vfdco_set_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  printf("Dummy date/time set: %02hhu.%02hhu.20%02hhu - %02hhu:%02hhu:%02hhu",
    date->d, date->m, date->y, time->h, time->m, time->s
  );
}

time_event_t Time_Event_Init(uint16_t interval) {
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
