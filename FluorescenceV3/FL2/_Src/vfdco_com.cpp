#include <Arduino.h>
#include "vfdco_com.h"

void COM_Handler_USB_Init() {
  Serial.begin(115200);
}

void COM_Handler_USB_Transfer(struct COM_Data *self) {
  if(self->tx_buffer) Serial.write(self->tx_buffer, self->tx_buffer_length);
}