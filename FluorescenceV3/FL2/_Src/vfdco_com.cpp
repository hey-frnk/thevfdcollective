#include <Arduino.h>
#include <SoftwareSerial.h>
#include "vfdco_com.h"

#define    B_GROUND   9     // Bluetooth Ground Pin
#define    B_TXD     10     // Bluetooth TXD Pin
#define    B_RXD     11     // Bluetooth RXD Pin

extern struct COM_Data global_com_data;
SoftwareSerial BTSerial(B_TXD, B_RXD);

void COM_Handler_USB_Init() {
  Serial.begin(115200);
}

void COM_Handler_BT_Serial_Init() {
  // Bluetooth Configuration
  pinMode(B_GROUND, OUTPUT);                    // Data Ground Pin (yup that's clumsy)
  digitalWrite(B_GROUND, LOW);                  // Permanent Low
  BTSerial.begin(4800);
}

void COM_Handler_USB_Transfer(const struct COM_Data *self) {
  if(self->tx_buffer) {
    TIMSK1 &= ~(1 << OCIE1A);
    Serial.write(self->tx_buffer, self->tx_buffer_length);
    TIMSK1 |= (1 << OCIE1A);
  }
}

void COM_Handler_BT_Serial_Transfer(const struct COM_Data *self) {
  if(self->tx_buffer) {
    TIMSK1 &= ~(1 << OCIE1A);
    BTSerial.write(self->tx_buffer, self->tx_buffer_length);
    TIMSK1 |= (1 << OCIE1A);
  }
}

// Serial event callback function of Arduino
void serialEvent() {
  // This condition makes the beginning of everything serial.
  int sRead = Serial.available();

  if(sRead > 0) {
    // If the communication pattern of 16 bytes is detected, write a message
    // Serial.print(sRead);
    // Temporarily disable interrupts, write message
    TIMSK1 &= ~(1 << OCIE1A);
    Serial.readBytes(global_com_data.rx_buffer, CONFIG_COM_RX_BUF_MAX);
    TIMSK1 |= (1 << OCIE1A);

    if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_IDLE) {
      global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_USB_BUSY;
    }

    // Discard the rest
    uint8_t flushBuffer[Serial.available()];
    Serial.readBytes(flushBuffer, Serial.available());
    Serial.flush();
  }

  
}

void COM_Handler_BT_Serial_Receive() {
  int bRead = BTSerial.available();

  if(bRead > 0) {
    // If the communication pattern of 16 bytes is detected, write a message
    // Serial.print(sRead);
    // Temporarily disable interrupts, write message
    TIMSK1 &= ~(1 << OCIE1A);
    BTSerial.readBytes(global_com_data.rx_buffer, CONFIG_COM_RX_BUF_MAX);
    TIMSK1 |= (1 << OCIE1A);

    if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_IDLE) {
      global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_BT_BUSY;
    }
    // Discard buffer
    uint8_t flushBuffer[BTSerial.available()];
    BTSerial.readBytes(flushBuffer, BTSerial.available());
    BTSerial.flush();
  }
}
