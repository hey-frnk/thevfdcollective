/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include "vfdco_ds3231.h"

#ifdef USE_CPU_STM32F042
/**
  * @target       STM32F042K6
  * @peripheral   I2C SDA/SCL (2 Pins) using blocking r/w (no DMA)
  */

#include "stm32f0xx_hal.h"

#define   DS3231_ADDRESS 0xD0

extern I2C_HandleTypeDef hi2c1;

static inline uint8_t _vfdco_bcd2dec(uint8_t input) {
  return (input >> 4) * 10 + (input & 0x0F);
}

static inline uint8_t _vfdco_dec2bcd(uint8_t input) {
  return ((input / 10) << 4) + (input % 10);
}

void vfdco_RTC_readDateTime(vfdco_date_t *d, vfdco_time_t *t) {
  uint8_t _i2cReadIn[7] = {0}; // 7 bytes required to read in date/time registers of DS3231
  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, _i2cReadIn, 7, 100); // I2C read into buffer

  t->sec = _vfdco_bcd2dec(_i2cReadIn[0] & 0x7F);
  t->min= _vfdco_bcd2dec(_i2cReadIn[1]);
  t->hr = _vfdco_bcd2dec(_i2cReadIn[2] & 0x3F);
  d->day = _vfdco_bcd2dec(_i2cReadIn[4]);
  d->month = _vfdco_bcd2dec(_i2cReadIn[5]);
  d->year = _vfdco_bcd2dec(_i2cReadIn[6]);
}

void vfdco_RTC_setDateTime(vfdco_date_t *d, vfdco_time_t *t) {
  uint8_t _i2cWrite[7];
  _i2cWrite[0] = _vfdco_dec2bcd(t->sec);
  _i2cWrite[1] = _vfdco_dec2bcd(t->min);
  _i2cWrite[2] = _vfdco_dec2bcd(t->hr);
  _i2cWrite[4] = _vfdco_dec2bcd(d->day);
  _i2cWrite[5] = _vfdco_dec2bcd(d->month);
  _i2cWrite[6] = _vfdco_dec2bcd(d->year);
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, _i2cWrite, 7, 1000);

}

#endif
