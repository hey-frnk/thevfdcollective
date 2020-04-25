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

#include "../Commons/vfdco_time.h"
#include <stdio.h>
#include <chrono>
#include <QTime>
#include <QTimer>
#include <QEventLoop>

#ifdef _TIME_IMPLEMENTATION
#error "An implementation of the time driver already exists!"
#endif
#define _TIME_IMPLEMENTATION

extern int clk_div;

uint32_t _vfdco_time_get_milliseconds() {
    return QTime::currentTime().msecsSinceStartOfDay();
}

void _vfdco_time_delay_milliseconds(uint32_t delay) {
    QEventLoop loop;
    QTimer::singleShot(delay, &loop, SLOT(quit()));
    loop.exec();
}

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

void vfdco_set_date_time(const vfdco_date_t *date, const vfdco_time_t *time) {
  printf("Dummy date/time set: %02hhu.%02hhu.20%02hhu - %02hhu:%02hhu:%02hhu",
    date->d, date->m, date->y, time->h, time->m, time->s
  );
}

void vfdco_rtc_init() {
  
}

time_event_t Time_Event_Init(uint16_t interval) {
  time_event_t iE;
  iE.interval = interval * clk_div;
  iE.previousTime = 0;
  return iE;
}

void Time_Event_Reset(time_event_t *input) {
  input->previousTime = 0;
}

unsigned char Time_Event_Update(time_event_t *input) {
  unsigned long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  if((currentTime - input->previousTime) > input->interval) {
    input->previousTime = currentTime;
    return 1;
  }
  else return 0;
  return 0;
}
