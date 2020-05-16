#include <stdlib.h>
// #include "../Inc/usbd_cdc_if.h"
#include "../../Commons/vfdco_config.h"
#include "../../Commons/vfdco_com.h"

extern uint8_t CDC_Transmit_FS(uint8_t*, uint16_t);

inline void COM_Handler_USB_Init() {
  return;
}

void COM_Handler_USB_Transfer(const struct COM_Data *self) {
  if(self->tx_buffer) CDC_Transmit_FS(self->tx_buffer, self->tx_buffer_length);
}