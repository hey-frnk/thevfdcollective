/**
  ******************************************************************************
  * @file     vfdco_24cXX.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the serialization driver for 24CXX EEPROM
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "stm32f0xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include "../../vfdco_serialization.h"

#ifdef _SERIALIZATION_IMPLEMENTATION
#error "An implementation of the serialization driver already exists!"
#endif
#define _SERIALIZATION_IMPLEMENTATION

#define   EEPROM_24CXX_ADDRESS 0xA0
extern I2C_HandleTypeDef hi2c1;

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

  // Write serialization header to EEPROM beginning, only if dirty
  for(uint8_t i = 0; i < (uint8_t)sizeof(struct Serialization_Header); ++i) {
    //uint8_t read_check = 0;
    //if(HAL_I2C_Mem_Read(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)i, I2C_MEMADD_SIZE_16BIT, &read_check, sizeof(uint8_t), 100) != HAL_OK)
     //exit(-98);
    //if(read_check != _uh._arr[i])
  	volatile HAL_StatusTypeDef s = HAL_I2C_Mem_Write(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)i, I2C_MEMADD_SIZE_16BIT, _uh._arr + i, sizeof(uint8_t), 1000);
  	HAL_Delay(1);
  	if(s != HAL_OK) {

  	}
  }
  
  // Write serialized data to EEPROM beginning + header offset, only if dirty
  for(uint8_t i = 0; i < length_arr_length; ++i) {
    for(uint8_t j = 0; j < length_arr[i]; ++j) {
      // uint8_t read_check = 0;
      //if(HAL_I2C_Mem_Read(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)(header_offset + j), I2C_MEMADD_SIZE_16BIT, &read_check, sizeof(uint8_t), 100) != HAL_OK)
        // exit(-100);
      // if(read_check != data[i][j])
    	volatile HAL_StatusTypeDef s = HAL_I2C_Mem_Write(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)(header_offset + j), I2C_MEMADD_SIZE_16BIT, data[i] + j, sizeof(uint8_t), 1000);
    	if(s != HAL_OK) {

    	}
    }
    header_offset += length_arr[i];
  }
}

SERIALIZATION_HEADER_STATUS_t vfdco_serialization_read(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length) {
  // Get header
  struct Serialization_Header _h;
  HAL_I2C_Mem_Read(&hi2c1, EEPROM_24CXX_ADDRESS, 0, I2C_MEMADD_SIZE_16BIT, (uint8_t *)&_h, sizeof(struct Serialization_Header), 1000);
    // exit(-109);

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
  /* for(uint8_t i = 0; i < length_arr_length; ++i, header_offset += length_arr[i]) {
    volatile HAL_StatusTypeDef s = HAL_I2C_Mem_Read(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)header_offset, I2C_MEMADD_SIZE_16BIT, data[i], length_arr[i], 1000);
      // exit(-110);
  }*/
  for(uint8_t i = 0; i < length_arr_length; ++i, header_offset += length_arr[i]) {
      for(uint8_t j = 0; j < length_arr[i]; ++j) {
      	volatile HAL_StatusTypeDef s = HAL_I2C_Mem_Read(&hi2c1, EEPROM_24CXX_ADDRESS, (uint16_t)(header_offset + j), I2C_MEMADD_SIZE_16BIT, data[i] + j, sizeof(uint8_t), 1000);
      	if(s != HAL_OK) {

      	}
      }
    }

  return SERIALIZATION_HEADER_STATUS_OK;
}
