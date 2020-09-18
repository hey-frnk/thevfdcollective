/**
  ******************************************************************************
  * @file     vfdco_time.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the Arduino time driver for DS3231
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>              // RTC Clock Library
#include "vfdco_time.h"

#ifdef _TIME_IMPLEMENTATION
#error "An implementation of the time driver already exists!"
#endif
#define _TIME_IMPLEMENTATION

RTC_DS1307 rtc;

inline uint32_t _remillis() { return millis(); }
inline void _redelay(uint32_t redelay_value) { delay(redelay_value); }

// Map to HAL_GetTick on STM32
uint32_t (*vfdco_time_get_milliseconds)(void) = _remillis;
void (*vfdco_time_delay_milliseconds)(uint32_t) = _redelay;


static inline uint8_t _vfdco_bcd2dec(uint8_t input) {
  return (input >> 4) * 10 + (input & 0x0F);
}

static inline uint8_t _vfdco_dec2bcd(uint8_t input) {
  return ((input / 10) << 4) + (input % 10);
}

void vfdco_get_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  DateTime now = rtc.now();
  time->h = now.hour();
  time->m = now.minute();
  time->s = now.second();
  date->d = now.day();
  date->m = now.month();
  date->y = now.year() % 100;
}

void vfdco_set_date_time(const vfdco_date_t *date, const vfdco_time_t *time) {
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.write(_vfdco_dec2bcd(time->s));
  Wire.write(_vfdco_dec2bcd(time->m));
  Wire.write(_vfdco_dec2bcd(time->h));
  Wire.write(0x06);
  Wire.write(_vfdco_dec2bcd(date->d));
  Wire.write(_vfdco_dec2bcd(date->m));
  Wire.write(_vfdco_dec2bcd(date->y));
  Wire.write(0);
  Wire.endTransmission();
}

void vfdco_rtc_init() {
  Wire.begin();
  rtc.begin();

  Wire.beginTransmission(0x68);
  Wire.write(0x07);
  Wire.write(0x10);

  Wire.endTransmission();

  if(!rtc.isrunning()) rtc.adjust(DateTime(__DATE__, __TIME__));
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
  unsigned long currentTime = _remillis();
  if((currentTime - input->previousTime) > input->interval) {
    input->previousTime = currentTime;
    return 1;
  }
  else return 0;
  return 0;
}
