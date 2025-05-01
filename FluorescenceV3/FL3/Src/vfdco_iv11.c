/**
  ******************************************************************************
  * @file     vfdco_iv11.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     24-October-2020
  * @brief    This file contains definitions for communication functions to an
  *           array of IV-11 vfd displays
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "stm32l4xx_hal.h"
#include <string.h>
#include "../../Commons/vfdco_config.h"
#include "../../Commons/vfdco_display.h"
#include "../../Commons/vfdco_time.h"

#ifdef _DISPLAY_IMPLEMENTATION
#error "An implementation of the display driver already exists!"
#endif
#define _DISPLAY_IMPLEMENTATION

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim16;

#define DIGIT_FADE_BRIGHTNESS_MAX 15
#define DIGIT_FADE_PWM_PHASE_MAX 16

typedef enum {
  DIGIT_FADE_NONE,
  DIGIT_FADE_OUT,
  DIGIT_FADE_SWITCH,
  DIGIT_FADE_IN
} digit_fade_state_t;

typedef struct {
  digit_fade_state_t state;
  uint8_t brightness;     // 0 = off, 16 = full
  uint8_t old_char;
  uint8_t new_char;
} digit_fade_t;

uint8_t display_buf[CONFIG_NUM_DIGITS] = {0};
uint8_t spi_buf[CONFIG_NUM_DIGITS] = {0};
uint8_t zeros_buf[CONFIG_NUM_DIGITS] = {0}; // Somehow DMA doesn't like stack memory

uint8_t dimming_factor;
uint8_t dimming_counter;

digit_fade_t fade_states[CONFIG_NUM_DIGITS];
uint8_t digit_fade_mode;

void vfdco_display_set_dim_factor(uint8_t _dimming_factor) {
  dimming_factor = _dimming_factor;
  dimming_counter = 0;
}

void vfdco_display_set_digit_fade_mode(uint8_t _digit_fade_mode) {
  digit_fade_mode = _digit_fade_mode;
}

static uint8_t pwm_phase = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if(htim->Instance == TIM16) {
    if (pwm_phase == 0) {
      digit_fade_t *fade = fade_states;
      for (uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i, ++fade) {
        uint8_t state = fade->state;
        uint8_t brightness = fade->brightness;

        uint8_t new_state = state;
        uint8_t new_brightness = brightness;

        if (state == DIGIT_FADE_OUT) {
          new_brightness = brightness - (brightness > 0);
          new_state = (brightness == 0) ? DIGIT_FADE_SWITCH : state;
        } else if (state == DIGIT_FADE_SWITCH) {
          new_state = DIGIT_FADE_IN;
          new_brightness = 0;
        } else if (state == DIGIT_FADE_IN) {
          new_brightness = (brightness < DIGIT_FADE_BRIGHTNESS_MAX) ? (brightness + 1) : DIGIT_FADE_BRIGHTNESS_MAX;
          new_state = (brightness == DIGIT_FADE_BRIGHTNESS_MAX) ? DIGIT_FADE_NONE : state;
        }

        fade->brightness = new_brightness;
        fade->state = new_state;
      }
    }
  
    digit_fade_t *fade = fade_states;
    uint8_t *disp = display_buf;
    uint8_t *spi = spi_buf;

    for (uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i, ++fade, ++disp, ++spi) {
      uint8_t dp_bit = *disp & 0x01;
      uint8_t out;

      switch (fade->state) {
        case DIGIT_FADE_OUT:
        case DIGIT_FADE_IN: {
          uint8_t char_sel = (fade->state == DIGIT_FADE_OUT) ? fade->old_char : fade->new_char;
          if (pwm_phase < fade->brightness) {
            out = (char_sel & 0xFE) | dp_bit;
          } else {
            if (digit_fade_mode == CONFIG_DIGIT_FADE_BLEND) {
              out = fade->new_char & fade->old_char;
            } else {
              out = dp_bit;
            }
          }
          break;
        }

        case DIGIT_FADE_SWITCH:
          out = (digit_fade_mode == CONFIG_DIGIT_FADE_BLEND) ? fade->new_char & fade->old_char : dp_bit;
          break;

        default:
          out = *disp;
          break;
      }

      *spi = out;
    }

    uint8_t *tx_buf = (dimming_counter < 1) ? spi_buf : zeros_buf;
    HAL_SPI_Transmit_DMA(&hspi1, tx_buf, CONFIG_NUM_DIGITS);
  }
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  // Toggle set/reset upon SPI transfer completion
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  
  // Crossfader
  ++pwm_phase;
  if(pwm_phase >= DIGIT_FADE_PWM_PHASE_MAX) {
    pwm_phase = 0;
  }

  // Dimmer
  ++dimming_counter;
  if(dimming_counter == (1 << dimming_factor)) {
    dimming_counter = 0;
  }
}

uint8_t vfdco_display_char_convert(char input) {
  // Takes char value (0 to 255) and converts to VFD clock display pattern
  switch(input){
    // seven little NUMBERS make a fire out of this flameeee
    case 0: return 0b11111100;
    case 1: return 0b01100000;
    case 2: return 0b11011010;
    case 3: return 0b11110010;
    case 4: return 0b01100110;
    case 5: return 0b10110110;
    case 6: return 0b10111110;
    case 7: return 0b11100000;
    case 8: return 0b11111110;
    case 9: return 0b11110110;
    // Letters
    case 'A': return 0b11101110;
    case 'B': return 0b00111110;
    case 'C': return 0b10011100;
    case 'D': return 0b01111010;
    case 'E': return 0b10011110;
    case 'F': return 0b10001110;
    case 'G': return 0b11110110;
    case 'H': return 0b01101110;
    case 'I': return 0b00001100;
    case 'J': return 0b01110000;
    case 'L': return 0b00011100;
    case 'N': return 0b00101010;
    case 'O': return 0b11111100;
    case 'P': return 0b11001110;
    case 'Q': return 0b11100110;
    case 'R': return 0b00001010;
    case 'S': return 0b10110110;
    case 'T': return 0b00011110;
    case 'U': return 0b00111000;
    case 'V': return 0b01111100;
    case 'Y': return 0b01110110;
    // Special characters
    case ' ': return 0b00000000; // Empty Output
    case '.': return 0b11000110; // Temperature Degree
    case '-': return 0b00000010;
    case '0': return 0b11111100;
  }
  return 0;
}

// Given a time or date, the time or date will be sent to the display
// Decimal dot overlay function: uint8_t decimal_dot_register
// [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
// 7                                                                     0
void vfdco_display_render_time(const vfdco_time_t *time, const uint8_t decimal_dot_register, time_format_t time_mode) {
  uint8_t _hour = time->h; // 12h fix
  if(time_mode != TIME_FORMAT_24H) {
    if       (_hour > 12) _hour -= 12; // 12h offset
    else if  (_hour == 0) _hour  = 12; // 12 AM fix
  }

  uint8_t target_buf[CONFIG_NUM_DIGITS];
  target_buf[0] = vfdco_display_char_convert(time->s % 10) | ( decimal_dot_register       & 0x01);
  target_buf[1] = vfdco_display_char_convert(time->s / 10) | ((decimal_dot_register >> 1) & 0x01);
  target_buf[2] = vfdco_display_char_convert(time->m % 10) | ((decimal_dot_register >> 2) & 0x01);
  target_buf[3] = vfdco_display_char_convert(time->m / 10) | ((decimal_dot_register >> 3) & 0x01);
  target_buf[4] = vfdco_display_char_convert(  _hour % 10) | ((decimal_dot_register >> 4) & 0x01);
  target_buf[5] = vfdco_display_char_convert(  _hour / 10) | ((decimal_dot_register >> 5) & 0x01);

  // Remove leading zero
  if(time_mode == TIME_FORMAT_12H_NO_LZ) {
    if(time->h > 12 && time->h < 22) target_buf[5] &= 0x01;
  }

  for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
    if((display_buf[i] & 0xFE) != (target_buf[i] & 0xFE)) {
      if(digit_fade_mode == CONFIG_DIGIT_FADE_NONE) {
        fade_states[i].state = DIGIT_FADE_NONE;
        display_buf[i] = target_buf[i];
      } else {
        fade_states[i].state = DIGIT_FADE_OUT;
        fade_states[i].old_char = display_buf[i];
        fade_states[i].new_char = target_buf[i];
        fade_states[i].brightness = DIGIT_FADE_BRIGHTNESS_MAX;
      }
    } else {
      fade_states[i].new_char = target_buf[i];
    }
  }

  memcpy(display_buf, target_buf, CONFIG_NUM_DIGITS);
}

void vfdco_display_render_date(const vfdco_date_t *date, /*const uint8_t decimal_dot_register, */date_format_t date_mode) {
  uint8_t target_buf[CONFIG_NUM_DIGITS];
  target_buf[0] = vfdco_display_char_convert(date->y % 10);
  target_buf[1] = vfdco_display_char_convert(date->y / 10);

  if(date_mode == DATE_FORMAT_DDMMYY) {
    target_buf[2] = vfdco_display_char_convert(date->m % 10) | 0x01;
    target_buf[3] = vfdco_display_char_convert(date->m / 10);
    target_buf[4] = vfdco_display_char_convert(date->d % 10) | 0x01;
    target_buf[5] = vfdco_display_char_convert(date->d / 10);
  } else {
    target_buf[2] = vfdco_display_char_convert(date->d % 10) | 0x01;
    target_buf[3] = vfdco_display_char_convert(date->d / 10);
    target_buf[4] = vfdco_display_char_convert(date->m % 10) | 0x01;
    target_buf[5] = vfdco_display_char_convert(date->m / 10);
  }
  memcpy(display_buf, target_buf, CONFIG_NUM_DIGITS);
}

void vfdco_display_render_message(const char *message, const uint8_t decimal_dot_register, uint16_t delay) {
  uint8_t target_buf[CONFIG_NUM_DIGITS];
  for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
    target_buf[CONFIG_NUM_DIGITS - i - 1] = vfdco_display_char_convert(message[i]) | ((decimal_dot_register >> (5 - i)) & 0x01);
  }
  if(delay) {
    // Temporarily disable interrupts, write message
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
    HAL_SPI_Transmit(&hspi1, target_buf, CONFIG_NUM_DIGITS, 40);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    vfdco_time_delay_milliseconds(delay);
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
  } else {
    memcpy(display_buf, target_buf, CONFIG_NUM_DIGITS);
  }
}

// Function mapping
inline void vfdco_display_init(uint8_t initial_dim_factor, uint8_t digit_fade_mode) {
  vfdco_display_set_dim_factor(initial_dim_factor);
  vfdco_display_set_digit_fade_mode(digit_fade_mode);
  HAL_TIM_Base_Start_IT(&htim16);
}
