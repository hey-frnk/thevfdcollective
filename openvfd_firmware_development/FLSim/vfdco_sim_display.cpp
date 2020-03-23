/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */


#include <stdio.h>
#include <string.h>
#include "../vfdco_config.h"
#include "../vfdco_display.h"
#include "../vfdco_time.h"

#include <QLabel>
#include <QEventLoop>
#include <QTimer>

extern QLabel *display[CONFIG_NUM_DIGITS][8];
extern QLabel *display_bin[CONFIG_NUM_DIGITS];

extern QColor active_color;
extern QColor inactive_color;

#ifdef _DISPLAY_IMPLEMENTATION
#error "An implementation of the display driver already exists!"
#endif
#define _DISPLAY_IMPLEMENTATION

#define NORENDER
#undef NORENDER

void _vfdco_display_render_direct(uint8_t *data);

void vfdco_display_set_dim_factor(uint8_t dim_factor) {
  printf("Setting Display Dim Factor: %hhu", dim_factor);
}

uint8_t vfdco_display_char_convert(char input) {
  // Takes char value (0 to 255) and converts to VFD clock display pattern
  switch(input) {
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
void vfdco_display_render_time(vfdco_time_t *time, const uint8_t decimal_dot_register, time_format_t time_mode) {
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
    if(time->h > 12 && time->h < 22) _rreg[5] = ' ';
  }
  _vfdco_display_render_direct(_rreg);
}

void vfdco_display_render_date(vfdco_date_t *date, /*const uint8_t decimal_dot_register, */date_format_t date_mode) {
  uint8_t _rreg[CONFIG_NUM_DIGITS];
  _rreg[0] = vfdco_display_char_convert(date->y % 10);
  _rreg[1] = vfdco_display_char_convert((date->y / 10) & 0x01);

  if(date_mode == DATE_FORMAT_DDMMYY) {
    _rreg[2] = vfdco_display_char_convert(date->m % 10);
    _rreg[3] = vfdco_display_char_convert((date->m / 10) & 0x01);
    _rreg[4] = vfdco_display_char_convert(date->d % 10);
    _rreg[5] = vfdco_display_char_convert(date->d / 10);
  } else {
    _rreg[2] = vfdco_display_char_convert(date->d % 10);
    _rreg[3] = vfdco_display_char_convert((date->d / 10) & 0x01);
    _rreg[4] = vfdco_display_char_convert(date->m % 10);
    _rreg[5] = vfdco_display_char_convert(date->m / 10);
  }
  _vfdco_display_render_direct(_rreg);
}

void vfdco_display_render_message(const char *message, const uint8_t decimal_dot_register, uint16_t delay) {
  uint8_t _rreg[CONFIG_NUM_DIGITS];
  for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
    _rreg[CONFIG_NUM_DIGITS - i - 1] = vfdco_display_char_convert(message[i]) | ((decimal_dot_register >> (5 - i)) & 0x01);
  }
  _vfdco_display_render_direct(_rreg);

  QEventLoop loop;
  QTimer::singleShot(delay, &loop, SLOT(quit()));
  loop.exec();
}

void _vfdco_display_render_direct(uint8_t *data) {
  for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) {
      for(uint_fast8_t j = 0; j < 8; ++j) {
          display[i][j]->setStyleSheet("background-color:" + (((data[CONFIG_NUM_DIGITS - i - 1] >> (8 - j - 1)) & 0x01) ? active_color.name() : inactive_color.name()));
      }
      display_bin[CONFIG_NUM_DIGITS - i - 1]->setText(QString("%1").arg(data[i], 8, 2, QChar('0')));
  }
}

// Function mapping
void vfdco_display_init(uint8_t initial_dim_factor) {
  printf("IV-11 fake debug display init with %d digits, successful\n", CONFIG_NUM_DIGITS);
  printf("Setting Display Dim Factor: %hhu", initial_dim_factor);
}
