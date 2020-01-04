/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include "../vfdco_display.h"
#include "../vfdco_time.h"
#include <stdio.h>

uint8_t     num_digits = 0;

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
void vfdco_display_render_time(vfdco_time_t *time, uint8_t decimal_dot_register) {
  uint8_t _rreg[num_digits];
  _rreg[0] = vfdco_display_char_convert(time->s % 10) | ( decimal_dot_register       & 0x01);
  _rreg[1] = vfdco_display_char_convert(time->s / 10) | ((decimal_dot_register >> 1) & 0x01);
  _rreg[2] = vfdco_display_char_convert(time->m % 10) | ((decimal_dot_register >> 2) & 0x01);
  _rreg[3] = vfdco_display_char_convert(time->m / 10) | ((decimal_dot_register >> 3) & 0x01);
  _rreg[4] = vfdco_display_char_convert(time->h % 10) | ((decimal_dot_register >> 4) & 0x01);
  _rreg[5] = vfdco_display_char_convert(time->h / 10) | ((decimal_dot_register >> 5) & 0x01);
  vfdco_display_render_direct(_rreg);
}

void vfdco_display_render_date(vfdco_date_t *date, uint8_t decimal_dot_register, uint_fast8_t date_mode) {
  uint8_t _rreg[num_digits];
  _rreg[0] = vfdco_display_char_convert(date->y % 10) | (decimal_dot_register & 0x01);
  _rreg[1] = vfdco_display_char_convert((date->y % 100) / 10) | ((decimal_dot_register >> 1) & 0x01);

  if(date_mode == DATE_MODE_DDMMYY) {
    _rreg[2] = vfdco_display_char_convert(date->m % 10) | ((decimal_dot_register >> 2) & 0x01);
    _rreg[3] = vfdco_display_char_convert(date->m / 10) | ((decimal_dot_register >> 3) & 0x01);
    _rreg[4] = vfdco_display_char_convert(date->d % 10) | ((decimal_dot_register >> 4) & 0x01);
    _rreg[5] = vfdco_display_char_convert(date->d / 10) | ((decimal_dot_register >> 5) & 0x01);
  } else {
    _rreg[4] = vfdco_display_char_convert(date->m % 10) | ((decimal_dot_register >> 4) & 0x01);
    _rreg[5] = vfdco_display_char_convert(date->m / 10) | ((decimal_dot_register >> 5) & 0x01);
    _rreg[2] = vfdco_display_char_convert(date->d % 10) | ((decimal_dot_register >> 2) & 0x01);
    _rreg[3] = vfdco_display_char_convert(date->d / 10) | ((decimal_dot_register >> 3) & 0x01);
  }
  vfdco_display_render_direct(_rreg);
}

void vfdco_display_render_message(const char *message, uint8_t decimal_dot_register) {
  uint8_t _rreg[num_digits];
  for(uint8_t i = 0; i < num_digits; ++i) {
    _rreg[num_digits - i - 1] = vfdco_display_char_convert(message[i]) | ((decimal_dot_register >> (5 - i)) & 0x01);
  }
  vfdco_display_render_direct(_rreg);
}

void vfdco_display_render_direct(uint8_t *data) {
  // Five lines
  char seg_h_on[]  = "---";
  char seg_h_off[] = "   ";
  char seg_v_on[]  = "|";
  char seg_v_off[] = " ";

  //  ---    ---    ---    ---    ---    ---
  // |   |  |   |  |   |  |   |  |   |  |   |
  //  ---    ---    ---    ---    ---    ---
  // |   |  |   |  |   |  |   |  |   |  |   |
  //  ---    ---    ---    ---    ---    ---

  // Segment a
  printf("\n ");
  for(uint_fast8_t i = 0; i < num_digits; ++i)
    printf("%s    ", ((data[i] >> 7) & 0x01) ? seg_h_on : seg_h_off);
  printf("\n");

  // Segment b, f
  for(uint_fast8_t i = 0; i < num_digits; ++i)
    printf("%s   %s  ", ((data[i] >> 6) & 0x01) ? seg_v_on : seg_v_off, ((data[i] >> 2) & 0x01) ? seg_v_on : seg_v_off);
  printf("\n");

  // Segment g
  printf(" ");
  for(uint_fast8_t i = 0; i < num_digits; ++i)
    printf("%s    ", ((data[i] >> 1) & 0x01) ? seg_h_on : seg_h_off);
  printf("\n");

  // Segment c, e
  for(uint_fast8_t i = 0; i < num_digits; ++i)
    printf("%s   %s  ", ((data[i] >> 5) & 0x01) ? seg_v_on : seg_v_off, ((data[i] >> 3) & 0x01) ? seg_v_on : seg_v_off);
  printf("\n");

  // Segment d
  printf(" ");
  for(uint_fast8_t i = 0; i < num_digits; ++i)
    printf("%s    ", ((data[i] >> 4) & 0x01) ? seg_h_on : seg_h_off);
  printf("\n");
}

// Function mapping
void vfdco_display_init(uint_fast8_t _num_digits) {
  num_digits = _num_digits;

  printf("IV-11 fake debug display init with %d digits, successful\n", _num_digits);
}
