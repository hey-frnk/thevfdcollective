/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include "vfdco_display.h"

#ifdef USE_CPU_STM32F042
/**
  * @target       STM32F042K6
  * @peripheral   SPI MOSI, SCK (2 Pins) using non blocking r/w (DMA); RCK, BRCK GPIO (2 Pins) for CS
  */

#include "stm32f0xx_hal.h"

#define SPI1_BUF_LENGTH 6
#define _vfdco_display_setRCK(a) HAL_GPIO_WritePin(D_RCK_GPIO_Port, D_RCK_Pin, a)

extern SPI_HandleTypeDef hspi1;
uint8_t _vfdco_display_buf[SPI1_BUF_LENGTH];

void vfdco_displayWrite(const uint8_t *data, const uint8_t *bcmp, uint8_t dataLength, uint8_t *bpos) {
  // Disable interrupt (mutex)
  NVIC_DisableIRQ(TIM3_IRQn);
  // 4 bit PWM counter
  if(++(*bpos) == 16) *bpos = 0;

  // Fill buffer, brightness data: set while position is less or equal duty cycle
  for(size_t i = 0; i < dataLength; ++i)
    _vfdco_display_buf[SPI1_BUF_LENGTH - i - 1] = data[i] & ((*bpos <= bcmp[i] && bcmp[i]) ? 0xFF : 0x00);

  // Write to SPI buffer & toggle latch
  HAL_SPI_Transmit(&hspi1, _vfdco_display_buf, SPI1_BUF_LENGTH, 40);
  _vfdco_display_setRCK(GPIO_PIN_SET);
  _vfdco_display_setRCK(GPIO_PIN_RESET);
  NVIC_EnableIRQ(TIM3_IRQn);
}

#endif
