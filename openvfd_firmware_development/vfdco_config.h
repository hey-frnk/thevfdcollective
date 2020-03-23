/**
  ******************************************************************************
  * @file     vfdco_config.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     21-March-2020
  * @brief    This header links the selected configuration header
  *           and performs a parameter validity check
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_H
#define _VFDCO_CONFIG_H

// Development platform. Leave one active
// #include "vfdco_config_hw2.h" // Arduino/AVR (HW Version < 3.0)
// #include "vfdco_config_stm.h" // STM32F0 (HW Version >= 3.0)
#include "vfdco_config_sim.h" // Simulation platform






// Do not touch from here
// ******** VALIDITY CHECK ********
// Platform validation 
#if (defined(_VFDCO_CONFIG_STM_H) + defined(_VFDCO_CONFIG_HW2_H) + defined(_VFDCO_CONFIG_SIM_H)) != 1
#error "Multiple or no configuration headers! Please specify the platform by only including one configuration header!"
#endif

// Color/gamma correction should not be enabled both
#if (defined(CONFIG_ENABLE_GAMMACORRECTION) + defined(CONFIG_ENABLE_COLORCORRECTION)) > 1
#warning "It is not advised to enable both color and gamma correction"
#endif

// ******** Joint configuration parameters  ********
// NUM_BPP & NUM_BYTES from LED configuration
#if CONFIG_USE_RGBW == 0
  #define CONFIG_NUM_BPP ((uint8_t) 3)
#else 
  #define CONFIG_NUM_BPP ((uint8_t) 4)
#endif
#define CONFIG_NUM_BYTES ((uint8_t)(CONFIG_NUM_PIXELS * CONFIG_NUM_BPP))

#endif

#ifdef __cplusplus
}
#endif
