#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Commons/vfdco_config.h"
#include "../Commons/vfdco_com.h"

uint8_t virtual_transfer_buffer[CONFIG_COM_TX_BUF_MAX] = {0};

void COM_Handler_USB_Init() {
  return;
}

void COM_Handler_BT_Serial_Init() {
  return;
}

void COM_Handler_USB_Transfer(const struct COM_Data *self) {
  if(!self->tx_buffer) {
    printf("Error, transfer buffer NULL\n");
    return;
  }
  printf("Transmitting: ");
  for(uint_fast16_t i = 0; i < self->tx_buffer_length; ++i) 
    printf("Dec: %hhu, Hex: %hhx, ASCII: %c\n", self->tx_buffer[i], self->tx_buffer[i], self->tx_buffer[i]);

  memcpy(virtual_transfer_buffer, self->tx_buffer, self->tx_buffer_length);
}

void COM_Handler_BT_Serial_Transfer(const struct COM_Data *self) {
  COM_Handler_USB_Transfer(self);
}

void COM_Handler_BT_Serial_Receive() {
  return;
}
