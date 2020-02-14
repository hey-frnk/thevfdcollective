#include <stdio.h>
#include "../vfdco_serialization.h"

/*void vfdco_serialization_init_retrieve_offsets(uint8_t *offset_diff_arr, uint8_t length_diff_arr) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r");

  fseek(bin_file, sizeof(struct Serialization_Header), SEEK_SET);
  printf("File pointer moved to %d\n", sizeof(struct Serialization_Header));

  for(uint8_t i = 0; i < length_diff_arr; ++i) {
    uint8_t _size_offset;
    fread(&_size_offset, sizeof(uint8_t), 1, bin_file);
    fseek(bin_file, _size_offset, SEEK_CUR);
    offset_diff_arr[i] = _size_offset + 1;
    printf("Size Offset: %hhu\n", offset_diff_arr[i]);
  }
}*/ 

/* uint16_t vfdco_serialization_calculate_access_offset(const uint8_t *offset_diff_arr, uint8_t access_index) {
  uint16_t calculated_offset = 0;
  for(uint8_t i = 0; i < access_index; ++i) calculated_offset += offset_diff_arr[i];
  printf("vfdco_serialization_calculate_access_offset: Calculated offset of %hu\n", calculated_offset);
  return calculated_offset;
}

void vfdco_serialization_write_serialized(uint16_t access_address, uint8_t *data, uint8_t data_length) {
  printf("Write called (%hu, %p, %hhu)\n", access_address, data, data_length);
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r+");

  // Navigate to access position
  fseek(bin_file, sizeof(struct Serialization_Header) + access_address, SEEK_SET);
  printf("File pointer moved to %lu\n", sizeof(struct Serialization_Header));
  // Skip length
  // fseek(bin_file, 1, SEEK_CUR);
  // Write settings data to file
  fwrite(data, sizeof(uint8_t), data_length, bin_file);
  printf("Writing data to file: ");
  for(uint8_t i = 0; i < data_length; ++i) printf("[@addr: %lu, @cont: %hhu] ", sizeof(struct Serialization_Header) + access_address + i, data[i]);
  printf("\n");

  fclose(bin_file);
}


void vfdco_serialization_read_serialized(uint16_t access_address, uint8_t *data, uint8_t data_length) {

  printf("Read called (%hu, %p, %hhu)\n", access_address, data, data_length);
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r");

  // Navigate to access position
  fseek(bin_file, sizeof(struct Serialization_Header) + access_address, SEEK_SET);
  // Skip length
  // fseek(bin_file, 1, SEEK_CUR);
  // Write settings data to file
  fread(data, sizeof(uint8_t), data_length, bin_file);
  printf("Reading data from file: ");
  for(uint8_t i = 0; i < data_length; ++i) printf("[@addr: %lu, @cont: %hhu] ", sizeof(struct Serialization_Header) + access_address + i, data[i]);
  printf("\n");

  fclose(bin_file);
}*/ 

void vfdco_write_serialized(uint8_t *const data[], const uint8_t *length_arr, uint16_t data_length) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r+");
  
  struct Serialization_Header _h = {
    .vfdco_1 = 'v',
    .vfdco_2 = 'f',
    .sw_str = {'3', '.', '0', ' ', ' ', ' '},
    .hw_str = {'K', 'A', 'T', 'O'},
    .data_container_length = data_length,
  };

  fwrite(&_h, sizeof(struct Serialization_Header), 1, bin_file);
  for(uint8_t i = 0; i < data_length; ++i) {
    fwrite(data[i], sizeof(uint8_t), length_arr[i], bin_file);
  }

  fclose(bin_file);
}

void vfdco_read_serialized(uint8_t *const data[], const uint8_t *length_arr, uint16_t data_length) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r");
  fseek(bin_file, 0, SEEK_SET);

  // Get header
  struct Serialization_Header _h;
  fread(&_h, sizeof(struct Serialization_Header), 1, bin_file);

  for(uint8_t i = 0; i < data_length; ++i) {
    fread(data[i], sizeof(uint8_t), length_arr[i], bin_file);
  }

  fclose(bin_file);
}