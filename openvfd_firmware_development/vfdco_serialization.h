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
#include "vfdco_config.h"

typedef enum {
  // Everything's good
  SERIALIZATION_HEADER_STATUS_OK = 0,
  // The two bytes of vfdco_1 and vfdco_2 are wrong. Should trigger default writing
  SERIALIZATION_HEADER_STATUS_FORMAT_MISMATCH = -50,
  // HW version of read doesn't comply to HW_STR in CONFIG. Should trigger default writing
  SERIALIZATION_HEADER_STATUS_HW_MISMATCH = -51,
  // SW version of read doesn't comply to SW_STR in CONFIG. Should trigger change mapping
  SERIALIZATION_HEADER_STATUS_SW_MISMATCH = -52,
  // data_container_length == 0 or value not reasonable (very large). Should trigger default writing
  SERIALIZATION_HEADER_STATUS_DATA_LENGTH_ERR = -53,
  // Data error. File not large enough?
  SERIALIZATION_HEADER_STATUS_DATA_ERR = -60
} SERIALIZATION_HEADER_STATUS_t;

struct Serialization_Header {
  uint8_t vfdco_1, vfdco_2;
  uint8_t sw_str[CONFIG_SW_STRING_LENGTH];
  uint8_t hw_str[CONFIG_HW_STRING_LENGTH];
  uint16_t data_container_length;
};

static inline uint16_t _vfdco_calculate_length(const uint8_t *length_arr, const uint8_t length_arr_length) {
  uint16_t calculated_data_size = 0;
  for(uint8_t i = 0; i < length_arr_length; ++i) calculated_data_size += length_arr[i];
  return calculated_data_size;
}

void vfdco_serialization_write(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length);
SERIALIZATION_HEADER_STATUS_t vfdco_serialization_read(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length);

#endif