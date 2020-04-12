/**
  ******************************************************************************
  * @file     vfdco_mic.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the mic driver
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_MIC_DRIVER
#define _VFDCO_MIC_DRIVER

#include <stdint.h>
#include "vfdco_config.h"

// Initializer
void vfdco_mic_init();

// Read in level
uint8_t vfdco_mic_read_level(void);

#endif

#ifdef __cplusplus
}
#endif