/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include <Arduino.h>
#include <digitalWriteFast.h>
#include <string.h>
#include "vfdco_config.h"
#include "vfdco_display.h"
#include "vfdco_time.h"

#ifdef _DISPLAY_IMPLEMENTATION
#error "An implementation of the display driver already exists!"
#endif
#define _DISPLAY_IMPLEMENTATION

#define    CLOCK_PIN   2    // ATMEGA:  4   74HC595 SPI Clock Pin, SCK
#define    LATCH_PIN   3    // ATMEGA:  5   74HC595 SPI Latch Pin, RCK
#define    DATA_PIN    4    // ATMEGA:  6   74HC595 SPI Data Pin, SER

uint8_t display_buf[CONFIG_NUM_DIGITS] = {0};

struct Display_Dimmer {
  uint8_t dim_factor;
  uint8_t dim_counter;
};
struct Display_Dimmer display_dimmer;

// void vfdco_display_render_direct(uint8_t *data);

void vfdco_display_set_dim_factor(uint8_t dim_factor) {
  display_dimmer.dim_factor = dim_factor;
  display_dimmer.dim_counter = 0;
}

// Interrupt callback
ISR(TIMER1_COMPA_vect) {
  if(display_dimmer.dim_counter < 1) {
    // If the pulse is ON, write data to SPI
    for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
      register uint8_t d = display_buf[i];
      for(volatile register uint_fast8_t j = 0; j < 8; ++j)  {
        PORTD = (d & (1 << j)) ? PORTD | _BV(PD4) : PORTD & ~_BV(PD4);
        PORTD |= _BV(PD2);
        PORTD &= ~_BV(PD2);
      }
    }
    PORTD &= ~_BV(PD3);
    PORTD |= _BV(PD3);
    
  } else {
    // If the pulse is ON, else write zeros to SPI
    for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
      register uint8_t d = display_buf[i];
      for(volatile register uint_fast8_t j = 0; j < 8; ++j)  {
        PORTD &= ~_BV(PD4);
        PORTD |= _BV(PD2);
        PORTD &= ~_BV(PD2);
      }
    }
    PORTD &= ~_BV(PD3);
    PORTD |= _BV(PD3);
  }
  ++display_dimmer.dim_counter;
  if(display_dimmer.dim_counter == (1 << display_dimmer.dim_factor)) display_dimmer.dim_counter = 0;
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

  uint8_t _rreg[CONFIG_NUM_DIGITS];
  _rreg[0] = vfdco_display_char_convert(time->s % 10) | ( decimal_dot_register       & 0x01);
  _rreg[1] = vfdco_display_char_convert(time->s / 10) | ((decimal_dot_register >> 1) & 0x01);
  _rreg[2] = vfdco_display_char_convert(time->m % 10) | ((decimal_dot_register >> 2) & 0x01);
  _rreg[3] = vfdco_display_char_convert(time->m / 10) | ((decimal_dot_register >> 3) & 0x01);
  _rreg[4] = vfdco_display_char_convert(  _hour % 10) | ((decimal_dot_register >> 4) & 0x01);
  _rreg[5] = vfdco_display_char_convert(  _hour / 10) | ((decimal_dot_register >> 5) & 0x01);

  // Remove leading zero
  if(time_mode == TIME_FORMAT_12H_NO_LZ) {
    if(time->h > 12 && time->h < 22) _rreg[5] &= 0x01;
  }
  // vfdco_display_render_direct(_rreg);
  memcpy(display_buf, _rreg, CONFIG_NUM_DIGITS);
}

void vfdco_display_render_date(const vfdco_date_t *date, /*const uint8_t decimal_dot_register, */date_format_t date_mode) {
  uint8_t _rreg[CONFIG_NUM_DIGITS];
  _rreg[0] = vfdco_display_char_convert(date->y % 10);
  _rreg[1] = vfdco_display_char_convert(date->y / 10);

  if(date_mode == DATE_FORMAT_DDMMYY) {
    _rreg[2] = vfdco_display_char_convert(date->m % 10) | 0x01;
    _rreg[3] = vfdco_display_char_convert(date->m / 10);
    _rreg[4] = vfdco_display_char_convert(date->d % 10) | 0x01;
    _rreg[5] = vfdco_display_char_convert(date->d / 10);
  } else {
    _rreg[2] = vfdco_display_char_convert(date->d % 10) | 0x01;
    _rreg[3] = vfdco_display_char_convert(date->d / 10);
    _rreg[4] = vfdco_display_char_convert(date->m % 10) | 0x01;
    _rreg[5] = vfdco_display_char_convert(date->m / 10);
  }
  // vfdco_display_render_direct(_rreg);
  memcpy(display_buf, _rreg, CONFIG_NUM_DIGITS);
}

void vfdco_display_render_message(const char *message, const uint8_t decimal_dot_register, uint16_t delay_option) {
  uint8_t _rreg[CONFIG_NUM_DIGITS];
  for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
    _rreg[CONFIG_NUM_DIGITS - i - 1] = vfdco_display_char_convert(message[i]) | ((decimal_dot_register >> (5 - i)) & 0x01);
  }
  if(delay_option) {
    // Temporarily disable interrupts, write message
    TIMSK1 &= ~(1 << OCIE1A);
    digitalWriteFast(LATCH_PIN, LOW);
    for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; i++) shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, _rreg[i]);
    digitalWriteFast(LATCH_PIN, HIGH);
    vfdco_time_delay_milliseconds(delay_option);
    TIMSK1 |= (1 << OCIE1A);
  } else {
    memcpy(display_buf, _rreg, CONFIG_NUM_DIGITS);
  }
}

// Function mapping
void vfdco_display_init(uint8_t initial_dim_factor) {
  // Pin configuration
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  // Timer configuration
  TCNT1 = 0;
  // ~ 1kHz
  OCR1A = 16000;
  // Interrupt request
  TIMSK1 |= (1 << OCIE1A); 
  sei();
  // CTC mode
  TCCR1B |= (1 << WGM12);
  // Prescaler, 64
  TCCR1B |= (1 << CS10);
  // TCCR1B |= (1 << CS11);

  vfdco_display_set_dim_factor(initial_dim_factor);
}