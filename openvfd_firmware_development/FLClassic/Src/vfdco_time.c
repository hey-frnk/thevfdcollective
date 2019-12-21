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

#include "stm32f0xx_hal.h"
#include "../../vfdco_time.h"

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
  unsigned long currentTime = HAL_GetTick();
  if((currentTime - input->previousTime) > input->interval) {
    input->previousTime = currentTime;
    return 1;
  }
  else return 0;
  return 0;
}
