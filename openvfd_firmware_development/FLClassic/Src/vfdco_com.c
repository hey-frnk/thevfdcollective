#include <stdlib.h>
#include "../Inc/usbd_cdc_if.h"
#include "../../vfdco_config.h"
#include "../../vfdco_com.h"

void COM_Handler_USB_Transfer(struct COM_Data *self) {
  CDC_Transmit_FS(self->tx_buffer, self->tx_buffer_length);
}