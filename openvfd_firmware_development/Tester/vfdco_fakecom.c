#include <stdlib.h>
#include <stdio.h>
#include "../vfdco_config.h"
#include "../vfdco_com.h"

void (*COM_Handler_Update)(struct COM_Data *self) = NULL;
void (*COM_Handler_Transfer)(struct COM_Data *self) = NULL;
void (*COM_Handler_Receive)(struct COM_Data *self) = NULL;

static inline void _COM_Handler_USB_Virtual_Transfer(struct COM_Data *self) {
  printf("VCP COM Handler Fake: Sending: ");
  for(uint8_t i = 0; i < self->tx_buffer_length; ++i) printf("%hhx", self->tx_buffer[i]);
  printf("\n");
}

static inline void _COM_Handler_USB_Virtual_Receive(struct COM_Data *self) {
  
}

void COM_Handler_CDC_Invoke() {
  COM_Handler_Transfer = _COM_Handler_USB_Virtual_Transfer;
  COM_Handler_Receive = _COM_Handler_USB_Virtual_Receive;
}