#include <stdlib.h>
#include <string.h>
#include "../../Commons/vfdco_config.h"
#include "../../Commons/vfdco_com.h"
#include "stm32l4xx_hal.h"

extern uint8_t CDC_Transmit_FS(uint8_t*, uint16_t);
extern UART_HandleTypeDef huart2;

uint8_t com_bt_rx_buf[64] = {0};
uint16_t com_bt_rx_buf_count;
uint16_t com_bt_rx_buf_lock_in;

inline void COM_Handler_USB_Init() {
  return;
}

inline void COM_Handler_BT_Serial_Init() {
	com_bt_rx_buf_count = 0;
	com_bt_rx_buf_lock_in = 0;
}

void COM_Handler_USB_Transfer(const struct COM_Data *self) {
  if(self->tx_buffer) CDC_Transmit_FS(self->tx_buffer, self->tx_buffer_length);
}

void COM_Handler_BT_Serial_Transfer(const struct COM_Data *self) {
	if(self->tx_buffer) HAL_UART_Transmit(&huart2, self->tx_buffer, self->tx_buffer_length, 100);
}

void COM_Handler_BT_Serial_Receive() {
	if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_IDLE) {
		// Try to receive buffer data, one byte at a time
		HAL_UART_Receive_IT(&huart2, com_bt_rx_buf + com_bt_rx_buf_count, 1);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(com_bt_rx_buf[com_bt_rx_buf_count] == 0x24) {
		// Protocol detected. Lock in buffer
		com_bt_rx_buf_lock_in = com_bt_rx_buf_count;
	}

	if(com_bt_rx_buf[com_bt_rx_buf_lock_in + global_com_data.rx_buffer_length - 1] == 0x25) {
		// Protocol valid. Transmit to com_data rx_buffer and notify
		memcpy(global_com_data.rx_buffer, com_bt_rx_buf + com_bt_rx_buf_lock_in, global_com_data.rx_buffer_length);
		global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_BT_BUSY;
		// Reset buffer
		memset(com_bt_rx_buf, 0x00, 64);
		com_bt_rx_buf_count = 0;
		com_bt_rx_buf_lock_in = 0;
	}

	++com_bt_rx_buf_count;
	if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_IDLE) {
		// Try to receive buffer data, one byte at a time
		HAL_UART_Receive_IT(&huart2, com_bt_rx_buf + com_bt_rx_buf_count, 1);
	}
	if(com_bt_rx_buf_count >= 63) {
		memset(com_bt_rx_buf, 0x00, 64);
		com_bt_rx_buf_count = 0;
		com_bt_rx_buf_lock_in = 0;
	}
}
