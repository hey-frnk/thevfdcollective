#include <stdio.h>
#include <string.h>
#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include "../Commons/vfdco_config.h"
#include "../Commons/vfdco_serialization.h"

#ifdef _SERIALIZATION_IMPLEMENTATION
#error "An implementation of the serialization driver already exists!"
#endif
#define _SERIALIZATION_IMPLEMENTATION

static inline uint16_t _vfdco_calculate_length(const uint8_t *length_arr, const uint8_t length_arr_length) {
  uint16_t calculated_data_size = 0;
  for(uint8_t i = 0; i < length_arr_length; ++i) calculated_data_size += length_arr[i];
  return calculated_data_size;
}

void vfdco_serialization_write(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length) {
    QString qPath = QDir::currentPath() + "/serialization.bin";
    qDebug() << qPath;
    QByteArray pathBytes = qPath.toUtf8(); // convert to UTF-8 path
    const char *path = pathBytes.constData();

    FILE *bin_file = fopen(path, "wb+"); // "wb+" ensures file is created if not existing
    if (!bin_file) {
        perror("fopen failed");
        return;
    }

    struct Serialization_Header _h = {
        .vfdco_1 = 'v',
        .vfdco_2 = 'f',
        .sw_str = CONFIG_SW_STRING,
        .hw_str = CONFIG_HW_STRING,
        .data_container_length = _vfdco_calculate_length(length_arr, length_arr_length)
    };

    if (fwrite(&_h, sizeof(struct Serialization_Header), 1, bin_file) != 1) {
        perror("fwrite header failed");
        fclose(bin_file);
        return;
    }

    size_t data_bytes_written = 0;
    printf("SERIALIZATION: Header written, starting data write to file!\n");

    for (uint8_t i = 0; i < length_arr_length; ++i) {
        size_t written = fwrite(data[i], sizeof(uint8_t), length_arr[i], bin_file);
        if (written != length_arr[i]) {
            fprintf(stderr, "Error: Could not write full data block %u\n", i);
            fclose(bin_file);
            return;
        }

        data_bytes_written += written;
        printf("Block write of size %hhu at offset %zu: ", length_arr[i], data_bytes_written);
        for (uint8_t j = 0; j < length_arr[i]; ++j) {
            printf("%hhu ", data[i][j]);
        }
        printf("\n");
    }

    fclose(bin_file);
    printf("Serialization complete.\n");
}

SERIALIZATION_HEADER_STATUS_t vfdco_serialization_read(uint8_t *const data[], const uint8_t *length_arr, const uint8_t length_arr_length) {
  FILE *bin_file;
  bin_file = fopen("serialization.bin", "r");
  if(!bin_file) return SERIALIZATION_HEADER_STATUS_DATA_ERR;
  fseek(bin_file, 0, SEEK_SET);

  // Get header
  struct Serialization_Header _h;
  fread(&_h, sizeof(struct Serialization_Header), 1, bin_file);

  // Check header consistency
  // Format correct?
  if(_h.vfdco_1 != (uint8_t)'v' || _h.vfdco_2 != (uint8_t)'f')
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

  printf("SERIALIZATION: Header OK, starting read from file! \n");
  size_t data_bytes_read = 0;
  for(uint8_t i = 0; i < length_arr_length; ++i) {
    data_bytes_read += fread(data[i], sizeof(uint8_t), length_arr[i], bin_file);
    printf("Block read of size %hhu at offset %zu: ", length_arr[i], data_bytes_read);
      for(uint8_t j = 0; j < length_arr[i]; ++j) printf("%hhu ", data[i][j]);
    printf("\n");
  }
  if((uint16_t)data_bytes_read != calculated_container_length)
    return SERIALIZATION_HEADER_STATUS_DATA_ERR;

  fclose(bin_file);
  return SERIALIZATION_HEADER_STATUS_OK;
}
