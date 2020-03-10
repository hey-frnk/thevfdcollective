/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_H
#define _VFDCO_CONFIG_H

// AVR (Arduino) platform
#ifdef __AVR__
#include "vfdco_config_hw2.h"

// ARM (STM32) platform
#elif __arm__
#include "vfdco_config_stm.h"

// Simulation PC platform
#else
#include "vfdco_config_sim.h"

#endif


#endif

#ifdef __cplusplus
}
#endif
