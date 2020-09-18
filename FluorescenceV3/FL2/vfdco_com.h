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
  * @file     vfdco_com.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     15-February-2020
  * @brief    This file contains declarations for inter-device communication
  *           for use with the OpenVFD Serial Protocol
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_COM_H
#define _VFDCO_COM_H

#include <stdint.h>
#include "vfdco_config.h"

/**
 * @brief Serial receive buffer state
 */
typedef enum {
  RX_BUFFER_DATA_IDLE,
  RX_BUFFER_DATA_USB_BUSY,
  RX_BUFFER_DATA_BT_BUSY
} COM_Data_RX_State_t;

/**
 * @brief Local structure of COM data consisting of
 * - a COM_Data_RX_State_t status variable 
 * - length variables for rx_buffer and tx_buffer
 * - a receive buffer rx_buffer set to COM_RX_BUF_MAX. Should be the length of the longest protocol command
 * - a transmit buffer tx_buffer that holds a momentary transfer array
 */
struct COM_Data {
  COM_Data_RX_State_t rx_buffer_data_present;
  uint8_t rx_buffer_length;
  uint8_t tx_buffer_length;
  uint8_t rx_buffer[CONFIG_COM_RX_BUF_MAX];
  uint8_t *tx_buffer;
};

/**
 * @brief Global COM data variable
 */
extern struct COM_Data global_com_data;

/**
 * @brief In this function, the USB communication interface is initialized
 * By using Arduino, use this function to implement Serial.begin(...)
 * By using STM32 HAL, this function can stay unused as initialization is done by STM32 HAL in void main()
 */
void COM_Handler_USB_Init();

/**
 * @brief In this function, data in self->tx_buffer is sent over the USB interface
 * @param self The COM structure holding the tx_buffer to be transfered
 */
void COM_Handler_USB_Transfer(const struct COM_Data *self);

/**
 * @brief This is the callback function, data from the USB interface is present
 * By using Arduino, this function should be void serialEvent()
 * By using STM32 HAL, the callback is implemented in static int8_t CDC_Receive_FS(uint8_t *, uint32_t *) in usbd_cdc_if.c
 */
// void receive function(void)

/**
 * @brief In this function, the bluetooth serial communication interface is initialized
 * By using Arduino, use this function to implement BTSerial.begin(...)
 * By using STM32 HAL, this function can stay unused as initialization is done by STM32 HAL in void main()
 */
void COM_Handler_BT_Serial_Init();

/**
 * @brief In this function, data in self->tx_buffer is sent over the bluetooth serial interface
 * @param self The COM structure holding the tx_buffer to be transfered
 */
void COM_Handler_BT_Serial_Transfer(const struct COM_Data *self);

/**
 * @brief This is the callback function, data from the bluetooth serial interface is present
 * By using Arduino, this function should be void serialEvent()
 * By using STM32 HAL, the callback is implemented in (...tbd...)
 */
void COM_Handler_BT_Serial_Receive();

#endif

#ifdef __cplusplus
}
#endif