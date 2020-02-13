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

struct Serialized_Data {
  uint8_t length;   // Length of serialized data, max 255 bytes per instance
  uint8_t *data;    // Aligned, serialized data
};

#define NUM_SERIALIZABLE 8
extern struct Serialized_Data *serialized_data[NUM_SERIALIZABLE];

#define CREATE_SERIALIZED_ENTRIES(ENTRY) \
  ENTRY(0, SERIALIZABLE_GUI_TIME, 7) \
  ENTRY(1, SERIALIZABLE_GUI_DATE, 14) \
  ENTRY(2, SERIALIZABLE_LIGHTS_STATIC, 3) \
  ENTRY(3, SERIALIZABLE_LIGHTS_BLISS, 4) \
  ENTRY(4, SERIALIZABLE_LIGHTS_SPECTRUM, 11) \
  ENTRY(5, SERIALIZABLE_LIGHTS_RAINBOW, 2) \
  ENTRY(6, SERIALIZABLE_ROUTINE_GUI, 11) \
  ENTRY(7, SERIALIZABLE_ROUTINE_LIGHTS, 22)

struct Serialization_Header {
  uint8_t vfdco_1, vfdco_2;
  uint8_t sw_str[6];
  uint8_t hw_str[4];
  uint16_t data_container_length;
};

void vfdco_write_serialized(struct Serialized_Data **data_container, uint16_t container_length);
void vfdco_read_serialized(struct Serialized_Data **data_container, uint16_t container_length);

#endif