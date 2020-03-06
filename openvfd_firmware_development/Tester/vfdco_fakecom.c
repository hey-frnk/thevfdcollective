#include <stdlib.h>
#include <stdio.h>
#include "../vfdco_config.h"
#include "../vfdco_com.h"
#include <stdio.h>

void COM_Handler_USB_Transfer(struct COM_Data *self) {
  if(!self->tx_buffer) {
    printf("Error, transfer buffer NULL\n");
    return;
  }
  printf("Transmitting: ");
  for(uint_fast16_t i = 0; i < self->tx_buffer_length; ++i) 
    printf("Dec: %hhu, Hex: %hhx, ASCII: %c\n", self->tx_buffer[i], self->tx_buffer[i], self->tx_buffer[i]);
}