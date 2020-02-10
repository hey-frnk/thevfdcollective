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

#include <stdint.h>

struct Serialized_Data {
  uint8_t length;   // Length of serialized data, max 255 bytes per instance
  uint8_t *data;    // Aligned, serialized data
};

/* Preprocessor absuse starts here.

  INSTANCE_GLOBAL will create a SIZE bit uint8_t array with its struct member initialized.
  The corresponsing expanded syntax will look exactly like this:

  uint8_t _serializable_identifier_arr[size];
  const uint8_t _serializable_identifier_INDEX = ...;
  struct Serialized_Data _serializable_identifier_arr_serialized = {
    .length = size,
    .data = _serializable_identifier_arr_serialized
  };


  INSTANCE_GLOBAL_ARR will fill up the structs into an array of serialized data structs. It looks like this:

  struct Serialized_Data *serialized_data[NUM_SERIALIZABLE] = {
    &_serializable_identifier_arr_serialized, (1st entry)
    &_serializable_identifier_arr_serialized, (2nd entry)
    ...
    &_serializable_identifier_arr_serialized ((NUM_SERIALIZABLE - 1)th entry)
  };
*/

#define NUM_SERIALIZABLE 8
#define CREATE_SERIALIZED_ENTRIES(ENTRY) \
  ENTRY(0, SERIALIZABLE_GUI_TIME, 7) \
  ENTRY(1, SERIALIZABLE_GUI_DATE, 14) \
  ENTRY(2, SERIALIZABLE_LIGHTS_STATIC, 3) \
  ENTRY(3, SERIALIZABLE_LIGHTS_BLISS, 4) \
  ENTRY(4, SERIALIZABLE_LIGHTS_SPECTRUM, 11) \
  ENTRY(5, SERIALIZABLE_LIGHTS_RAINBOW, 2) \
  ENTRY(6, SERIALIZABLE_ROUTINE_GUI, 11) \
  ENTRY(7, SERIALIZABLE_ROUTINE_LIGHTS, 22)

#define INSTANCE_GLOBAL(_index, _serializable_identifier, size) uint8_t _serializable_identifier ## _arr[size]; \
  struct Serialized_Data _serializable_identifier ## _serialized = {.length = size, .data = _serializable_identifier ## _arr}; \
  const uint8_t _serializable_identifier ## _INDEX = _index;

CREATE_SERIALIZED_ENTRIES(INSTANCE_GLOBAL)

struct Serialized_Data *serialized_data[NUM_SERIALIZABLE] = {
  #define INSTANCE_GLOBAL_ARR(_index, _serializable_identifier, size) &_serializable_identifier ## _serialized,
  CREATE_SERIALIZED_ENTRIES(INSTANCE_GLOBAL_ARR)
};
// Preprocessor abuse ends here
