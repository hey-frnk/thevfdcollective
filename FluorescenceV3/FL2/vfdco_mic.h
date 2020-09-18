/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 04/25/2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**
  ******************************************************************************
  * @file     vfdco_mic.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the microphone driver
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_MIC_DRIVER
#define _VFDCO_MIC_DRIVER

#include <stdint.h>
#include "vfdco_config.h"

/**
 * @brief Initialize microphone driver
 * By using Arduino, configure GPIO settings by pinMode(...)
 * By using STM32 HAL, this function can stay unused as initialization is done by STM32 HAL in void main()
 */
void vfdco_mic_init();

/**
 * @brief Read in sound intensity level captured by the microphone
 * @return uint8_t Level, number between and including 0 and 6
 */
uint8_t vfdco_mic_read_level(void);

#endif

#ifdef __cplusplus
}
#endif