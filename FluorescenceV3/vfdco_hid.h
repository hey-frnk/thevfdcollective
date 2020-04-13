/**
  ******************************************************************************
  * @file     vfdco_hid.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-April-2020
  * @brief    This file contains declarations for interaction functions through
  *           physical push buttons
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_HID_H
#define _VFDCO_HID_H

#include <stdint.h>

#define   BUTTON_SHORTPRESS_THRESHOLD   50
#define   BUTTON_LONGPRESS_THRESHOLD    600

#define   BUTTON_STATE_OFF         0b00000000
#define   BUTTON_STATE_SHORTPRESS  0b00000001
#define   BUTTON_STATE_LONGPRESS   0b00000010

#define   BUTTON_F1                0
#define   BUTTON_F2                1
#define   BUTTON_F3                2
#define   BUTTON_F4                3

/**
 * @brief Reset the state of a single button
 * @param button Which button
 */
void      vfdco_hid_button_reset(uint8_t button);

/**
 * @brief Retrieve the state of a single button
 * @param button Which button
 * @return uint8_t Button state. 00: Off, 01: Short press, 02: Long press, 03: Error
 */
uint8_t   vfdco_hid_button_retrieve(uint8_t button);

/**
 * @brief Retrieve state of all four buttons
 * @param b0 Pass button state variable of b0 by reference
 * @param b1 Pass button state variable of b1 by reference
 * @param b2 Pass button state variable of b2 by reference
 * @param b3 Pass button state variable of b3 by reference
 */
void      vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3);

/**
 * @brief Initializes the HID
 * By using Arduino, configure GPIO settings by pinMode(...)
 * By using STM32 HAL, this function can stay unused as initialization is done by STM32 HAL in void main()
 */
void      vfdco_hid_init();

/**
 * @brief For the Fluorescence simulator, button sates are set explicitly. DO NOT implement outide the simulator
 * @param count Counter value. count < BUTTON_SHORTPRESS_THRESHOLD < count < BUTTON_LONGPRESS_THRESHOLD < count
 * @param button Which button
 */
void      _vfdco_hid_button_set(uint16_t count, uint8_t button);


#endif

#ifdef __cplusplus
}
#endif