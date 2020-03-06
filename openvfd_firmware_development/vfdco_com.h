/**
  ******************************************************************************
  * @file     vfdco_com.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     15-February-2020
  * @brief    This file contains declarations for inter-device communication
  *           for use with the OpenVFD Serial Protocol
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_TODOOOO
  ******************************************************************************
 **/

#ifndef _VFDCO_COM_H
#define _VFDCO_COM_H

#include <stdint.h>
#include "vfdco_config.h"

typedef enum {
  RX_BUFFER_DATA_IDLE,
  RX_BUFFER_DATA_USB_BUSY,
  RX_BUFFER_DATA_BT_BUSY
} COM_Data_RX_State_t;

struct COM_Data {
  COM_Data_RX_State_t rx_buffer_data_present;
  uint8_t rx_buffer_length;
  uint8_t tx_buffer_length;
  uint8_t rx_buffer[CONFIG_COM_RX_BUF_MAX];
  uint8_t *tx_buffer;
};

extern struct COM_Data global_com_data;

void COM_Handler_USB_Transfer(struct COM_Data *self);

#endif