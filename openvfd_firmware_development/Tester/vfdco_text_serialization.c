#include <stdio.h>
#include "../vfdco_serialization.h"

void vfdco_write_serialized(struct Serialized_Data **data_container, uint16_t container_length) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "w");
  
  struct Serialization_Header _h = {
    .vfdco_1 = 'v',
    .vfdco_2 = 'f',
    .sw_str = {'3', '.', '0', ' ', ' ', ' '},
    .hw_str = {'K', 'A', 'T', 'O'},
    .data_container_length = container_length,
  };

  fwrite(&_h, sizeof(struct Serialization_Header), 1, bin_file);
  for(uint8_t i = 0; i < container_length; ++i) {
    fwrite(&data_container[i]->length, sizeof(uint8_t), 1, bin_file);
    fwrite(data_container[i]->data, sizeof(uint8_t), data_container[i]->length, bin_file);
  }

  fclose(bin_file);
}

void vfdco_read_serialized(struct Serialized_Data **data_container, uint16_t container_length) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r");
  fseek(bin_file, 0, SEEK_SET);

  // Get header
  struct Serialization_Header _h;
  fread(&_h, sizeof(struct Serialization_Header), 1, bin_file);

  for(uint8_t i = 0; i < container_length; ++i) {
    fread(&data_container[i]->length, sizeof(uint8_t), 1, bin_file);
    fread(data_container[i]->data, sizeof(uint8_t), data_container[i]->length, bin_file);
  }

  fclose(bin_file);
}
