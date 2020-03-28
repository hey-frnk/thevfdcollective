/**
  ******************************************************************************
  * @file     vfdco_time.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the time driver for DS3231
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "stm32f0xx_hal.h"
#include "../../vfdco_time.h"

#ifdef _TIME_IMPLEMENTATION
#error "An implementation of the time driver already exists!"
#endif
#define _TIME_IMPLEMENTATION

#define   DS3231_ADDRESS 0xD0
extern I2C_HandleTypeDef hi2c1;

// Map to HAL_GetTick on STM32
uint32_t (*vfdco_time_get_milliseconds)(void) = HAL_GetTick;
void (*vfdco_time_delay_milliseconds)(uint32_t) = HAL_Delay;


static uint8_t _vfdco_bcd2dec(uint8_t input) {
  return (input >> 4) * 10 + (input & 0x0F);
}

static uint8_t _vfdco_dec2bcd(uint8_t input) {
  return ((input / 10) << 4) + (input % 10);
}

void vfdco_get_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  uint8_t _i2cReadIn[7] = {0}; // 7 bytes required to read in date/time registers of DS3231
  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, _i2cReadIn, 7, 100); // I2C read into buffer

  time->s = _vfdco_bcd2dec(_i2cReadIn[0] & 0x7F);
  time->m = _vfdco_bcd2dec(_i2cReadIn[1]);
  time->h = _vfdco_bcd2dec(_i2cReadIn[2] & 0x3F);
  date->d = _vfdco_bcd2dec(_i2cReadIn[4]);
  date->m = _vfdco_bcd2dec(_i2cReadIn[5]);
  date->y = _vfdco_bcd2dec(_i2cReadIn[6]);
}

void vfdco_set_date_time(vfdco_date_t *date, vfdco_time_t *time) {
  uint8_t _i2cWrite[7];
  _i2cWrite[0] = _vfdco_dec2bcd(time->s);
  _i2cWrite[1] = _vfdco_dec2bcd(time->m);
  _i2cWrite[2] = _vfdco_dec2bcd(time->h);
  _i2cWrite[4] = _vfdco_dec2bcd(date->d);
  _i2cWrite[5] = _vfdco_dec2bcd(date->m);
  _i2cWrite[6] = _vfdco_dec2bcd(date->y);
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, _i2cWrite, 7, 1000);
}

inline void vfdco_rtc_init() {
  return;
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
  unsigned long currentTime = HAL_GetTick();
  if((currentTime - input->previousTime) > input->interval) {
    input->previousTime = currentTime;
    return 1;
  }
  else return 0;
  return 0;
}
