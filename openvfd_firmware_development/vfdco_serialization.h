/**
  ******************************************************************************
  * @file     vfdco_serialization.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     04-February-2020
  * @brief    This file contains declarations for the serialization protocol
  *           used in Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifndef _VFDCO_SERIALIZATION_H
#define _VFDCO_SERIALIZATION_H

#include <stdint.h>

struct Serialization_Header {
  uint8_t vfdco_1, vfdco_2;
  uint8_t sw_str[6];
  uint8_t hw_str[4];
  uint16_t data_container_length;
};

/* uint16_t vfdco_serialization_calculate_access_offset(const uint8_t *offset_diff_arr, uint8_t access_index);
void vfdco_serialization_write_serialized(uint16_t access_address, uint8_t *data, uint8_t data_length);
void vfdco_serialization_read_serialized(uint16_t access_address, uint8_t *data, uint8_t data_length); */

void vfdco_write_serialized(uint8_t *const data[], const uint8_t *length_arr, uint16_t data_length);
void vfdco_read_serialized(uint8_t *const data[], const uint8_t *length_arr, uint16_t data_length);

#endif