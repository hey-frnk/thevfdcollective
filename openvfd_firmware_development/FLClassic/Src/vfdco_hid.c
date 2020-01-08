// Dummy HID driver

#include "stm32f0xx_hal.h"
#include "../../vfdco_hid.h"

uint8_t buttonRegister = 0;

extern TIM_HandleTypeDef htim14;

static inline uint8_t _vfdco_hid_button_mapper(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
		case GPIO_PIN_5: return BUTTON_F1;
		case GPIO_PIN_6: return BUTTON_F2;
		case GPIO_PIN_7: return BUTTON_F3;
		case GPIO_PIN_8: return BUTTON_F4;
		default: return BUTTON_F1;
	}
	return BUTTON_F1;
}

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
	uint8_t mask = BUTTON_STATE_SHORTPRESS;
	if      (count < BUTTON_SHORTPRESS_THRESHOLD) mask = BUTTON_STATE_OFF;
	else if (count > BUTTON_LONGPRESS_THRESHOLD)  mask = BUTTON_STATE_LONGPRESS;

	buttonRegister |= (mask << (button << 1));
}

void vfdco_hid_button_reset(uint8_t button) {
	buttonRegister &= ~(0b00000011 << (button << 1));
}

uint8_t vfdco_hid_button_retrieve(uint8_t button) {
  return (buttonRegister >> (button << 1)) & 0b00000011;
}

void vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  *b0 = vfdco_hid_button_retrieve(BUTTON_F1);
  vfdco_hid_button_reset(BUTTON_F1);
  *b1 = vfdco_hid_button_retrieve(BUTTON_F2);
  vfdco_hid_button_reset(BUTTON_F2);
  *b2 = vfdco_hid_button_retrieve(BUTTON_F3);
  vfdco_hid_button_reset(BUTTON_F3);
  *b3 = vfdco_hid_button_retrieve(BUTTON_F4);
  vfdco_hid_button_reset(BUTTON_F4);
}



// Interrupt callback routine
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// Debouncing
	// Trigger counter (Timer instance 14 - no particular reason y it's 14)
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) == 0){
			__HAL_TIM_SET_COUNTER(&htim14, 0);   //Reset Counter
			HAL_TIM_Base_Start(&htim14);
	} else {
			uint16_t counter = __HAL_TIM_GET_COUNTER(&htim14);
			_vfdco_hid_button_set(counter, _vfdco_hid_button_mapper(GPIO_Pin));
			HAL_TIM_Base_Stop(&htim14);
	}
}
