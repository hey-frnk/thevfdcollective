#include <stdio.h>
#include "../vfdco_serialization.h"

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
#define INSTANCE_GLOBAL(_index, _serializable_identifier, size) uint8_t _serializable_identifier ## _arr[size] = {0x68}; \
  struct Serialized_Data _serializable_identifier ## _serialized = {.length = size, .data = _serializable_identifier ## _arr}; \
  const uint8_t _serializable_identifier ## _INDEX = _index;

CREATE_SERIALIZED_ENTRIES(INSTANCE_GLOBAL)

struct Serialized_Data *serialized_data[NUM_SERIALIZABLE] = {
  #define INSTANCE_GLOBAL_ARR(_index, _serializable_identifier, size) &_serializable_identifier ## _serialized,
  CREATE_SERIALIZED_ENTRIES(INSTANCE_GLOBAL_ARR)
};

void vfdco_write_serialized(struct Serialized_Data **data_container, uint16_t container_length) {
  FILE *text_file;
  text_file = fopen("serialization.bin", "w");


  struct Serialization_Header _h = {
    .vfdco_1 = 'v',
    .vfdco_2 = 'f',
    .sw_str = {'3', '.', '0', ' ', ' ', ' '},
    .hw_str = {'K', 'A', 'T', 'O'},
    .data_container_length = 17,
  };

  fwrite(&_h, sizeof(struct Serialization_Header), 1, text_file);
  for(uint8_t i = 0; i < container_length; ++i) {
    fwrite(&data_container[i]->length, sizeof(uint8_t), 1, text_file);
    fwrite(data_container[i]->data, sizeof(uint8_t), data_container[i]->length, text_file);
  }


  fclose(text_file);
}

void vfdco_read_serialized(struct Serialized_Data **data_container, uint16_t container_length) {

}
