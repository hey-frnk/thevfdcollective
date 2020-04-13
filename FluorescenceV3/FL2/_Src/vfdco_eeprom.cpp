/**
  ******************************************************************************
  * @file     vfdco_24cXX.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the serialization driver for internal ATmega328P EEPROM
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include <Arduino.h>
#include <EEPROM.h>              // EEPROM Access
#include <string.h>
#include <stdlib.h>
#include "vfdco_serialization.h"

#ifdef _SERIALIZATION_IMPLEMENTATION
#error "An implementation of the serialization driver already exists!"
#endif
#define _SERIALIZATION_IMPLEMENTATION

void _write_eeprom_internal(uint16_t address, uint8_t *data, uint8_t data_length) {
  // Single writes
  for(uint8_t i = 0; i < data_length; ++i) {
  	EEPROM.update(address, data[i]);
    address += 1;
  }
}

void _read_eeprom_internal(uint16_t address, uint8_t *target, uint8_t target_length) {
  // Single reads
  for(uint8_t i = 0; i < target_length; ++i) {
  	target[i] = EEPROM.read(address);
    address += 1;
  }
}

static inline uint16_t _vfdco_calculate_length(const uint8_t *length_arr, const uint8_t length_arr_length) {
  uint16_t calculated_data_size = 0;
  for(uint8_t i = 0; i < length_arr_length; ++i) calculated_data_size += length_arr[i];
  return calculated_data_size;
}

void vfdco_serialization_write(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length) {
  struct Serialization_Header _h = {
    .vfdco_1 = 'v',
    .vfdco_2 = 'f',
    .sw_str = CONFIG_SW_STRING,
    .hw_str = CONFIG_HW_STRING,
    .data_container_length = _vfdco_calculate_length(length_arr, length_arr_length)
  };
  union Serialization_Header_Access {
    struct Serialization_Header _header;
    uint8_t _arr[sizeof(struct Serialization_Header)];
  };
  union Serialization_Header_Access _uh = {._header = _h};
  uint16_t header_offset = (uint16_t)sizeof(struct Serialization_Header);

  // Write header and data
  _write_eeprom_internal(0, _uh._arr, sizeof(struct Serialization_Header));
  for(uint8_t i = 0; i < length_arr_length; ++i) {
    _write_eeprom_internal(header_offset, data[i], length_arr[i]);
    header_offset += length_arr[i];
  }
}

SERIALIZATION_HEADER_STATUS_t vfdco_serialization_read(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length) {
  // Get header
  struct Serialization_Header _h;
  _read_eeprom_internal(0, (uint8_t *)&_h, sizeof(struct Serialization_Header));

  // Check header consistency
  // Format correct?
  if(_h.vfdco_1 != 'v' || _h.vfdco_2 != 'f') 
    return SERIALIZATION_HEADER_STATUS_FORMAT_MISMATCH;
  // Hardware version matched?
  char hw_str_reference[CONFIG_HW_STRING_LENGTH] = CONFIG_HW_STRING;
  if(memcmp(hw_str_reference, _h.hw_str, CONFIG_HW_STRING_LENGTH))
    return SERIALIZATION_HEADER_STATUS_HW_MISMATCH;
  // Software version matched?
  char sw_str_reference[CONFIG_SW_STRING_LENGTH] = CONFIG_SW_STRING;
  if(memcmp(sw_str_reference, _h.sw_str, CONFIG_SW_STRING_LENGTH))
    return SERIALIZATION_HEADER_STATUS_SW_MISMATCH;
  // Data length 
  uint16_t calculated_container_length = _vfdco_calculate_length(length_arr, length_arr_length);
  if(_h.data_container_length != calculated_container_length) 
    return SERIALIZATION_HEADER_STATUS_DATA_LENGTH_ERR;

  uint16_t header_offset = (uint16_t)sizeof(struct Serialization_Header);
  for(uint8_t i = 0; i < length_arr_length; ++i) {
    _read_eeprom_internal((uint16_t)header_offset, data[i], length_arr[i]);
    header_offset += length_arr[i];
  }

  return SERIALIZATION_HEADER_STATUS_OK;
}
