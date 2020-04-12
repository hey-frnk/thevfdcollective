#include <Arduino.h>
#include "vfdco_com.h"

void COM_Handler_USB_Init() {
  Serial.begin(115200);
}

void COM_Handler_USB_Transfer(struct COM_Data *self) {
  if(self->tx_buffer) Serial.write(self->tx_buffer, self->tx_buffer_length);
}

extern struct COM_Data global_com_data;

// Serial event callback function of Arduino
void serialEvent() {
  // This condition makes the beginning of everything serial.
  int sRead = Serial.available();

  if(sRead > 0) {
    // If the communication pattern of 16 bytes is detected, write a message
    // Serial.print(sRead);
    Serial.readBytes(global_com_data.rx_buffer, CONFIG_COM_RX_BUF_MAX);

    if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_IDLE) {
      global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_USB_BUSY;
    }
  }

  // Discard the rest
  uint8_t flushBuffer[Serial.available()];
  Serial.readBytes(flushBuffer, Serial.available());
  Serial.flush();

  /* int bRead = BTSerial.available();

  if(bRead > 0) {
    uint8_t inputBuffer[24];
    BTSerial.readBytes(inputBuffer, 24);

    serialCommandDecode(inputBuffer, serialCommandEncodeBluetooth);

    // Discard buffer
    uint8_t flushBuffer[BTSerial.available()];
    BTSerial.readBytes(flushBuffer, BTSerial.available());
    BTSerial.flush();

  } */
}