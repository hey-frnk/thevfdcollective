/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include "vfdco_render.h"
#include "vfdco_display.h"

uint8_t _vfdco_bcmp[NUMOFTUBES] = {15, 15, 15, 15, 15, 15};
uint8_t _vfdco_render_register[NUMOFTUBES] = {0};
uint8_t _vfdco_bpos = 0;

inline void vfdco_render() {
  vfdco_displayWrite(_vfdco_render_register, _vfdco_bcmp, NUMOFTUBES, &_vfdco_bpos);
}

void vfdco_render_time(vfdco_time_t *t, uint8_t dDotOverlay) {
  _vfdco_render_register[0] = vfdco_render_charToSevenSeg(t->sec % 10) | (dDotOverlay & 0x01);
  _vfdco_render_register[1] = vfdco_render_charToSevenSeg(t->sec / 10) | ((dDotOverlay >> 1) & 0x01);
  _vfdco_render_register[2] = vfdco_render_charToSevenSeg(t->min % 10) | ((dDotOverlay >> 2) & 0x01);
  _vfdco_render_register[3] = vfdco_render_charToSevenSeg(t->min / 10) | ((dDotOverlay >> 3) & 0x01);
  _vfdco_render_register[4] = vfdco_render_charToSevenSeg(t->hr % 10) | ((dDotOverlay >> 4) & 0x01);
  _vfdco_render_register[5] = vfdco_render_charToSevenSeg(t->hr / 10) | ((dDotOverlay >> 5) & 0x01);
}

void vfdco_render_date(vfdco_date_t *d, uint8_t dDotOverlay, enum DATE_MODE dm) {
  _vfdco_render_register[0] = vfdco_render_charToSevenSeg(d->year % 10) | (dDotOverlay & 0x01);
  _vfdco_render_register[1] = vfdco_render_charToSevenSeg((d->year % 100) / 10) | ((dDotOverlay >> 1) & 0x01);

  if(dm == DATE_MODE_DDMMYY) {
    _vfdco_render_register[2] = vfdco_render_charToSevenSeg(d->month % 10) | ((dDotOverlay >> 2) & 0x01);
    _vfdco_render_register[3] = vfdco_render_charToSevenSeg(d->month / 10) | ((dDotOverlay >> 3) & 0x01);
    _vfdco_render_register[4] = vfdco_render_charToSevenSeg(d->day % 10) | ((dDotOverlay >> 4) & 0x01);
    _vfdco_render_register[5] = vfdco_render_charToSevenSeg(d->day / 10) | ((dDotOverlay >> 5) & 0x01);
  } else {
    _vfdco_render_register[4] = vfdco_render_charToSevenSeg(d->month % 10) | ((dDotOverlay >> 4) & 0x01);
    _vfdco_render_register[5] = vfdco_render_charToSevenSeg(d->month / 10) | ((dDotOverlay >> 5) & 0x01);
    _vfdco_render_register[2] = vfdco_render_charToSevenSeg(d->day % 10) | ((dDotOverlay >> 2) & 0x01);
    _vfdco_render_register[3] = vfdco_render_charToSevenSeg(d->day / 10) | ((dDotOverlay >> 3) & 0x01);
  }
}

inline void vfdco_render_message(const char *message, uint8_t dDotOverlay) {
  for(uint8_t i = 0; i < NUMOFTUBES; ++i) _vfdco_render_register[NUMOFTUBES - i - 1] = vfdco_render_charToSevenSeg(message[i]) | ((dDotOverlay >> (5 - i)) & 0x01);
}

// Takes a regular char input and returns the corresponding 7 segment uint8_t. Byte mapping: 0b |a|b|c|d|e|f|g|.|
uint8_t vfdco_render_charToSevenSeg(char input){
  // Takes char value (0 to 255) and converts to VFD clock display pattern
  switch(input){
    // Numbers
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
    case '.': return 0b11000110; // Temperature Dot
    case '-': return 0b00000010;
    case '0': return 0b11111100;
  }
  return 0;
}

inline void vfdco_render_setBrightnessSingle(uint8_t index, uint8_t brightnessValue) {
  _vfdco_bcmp[index] = brightnessValue;
}

inline void vfdco_render_setBrightnessAll(uint8_t brightnessValue) {
  for(uint8_t i = 0; i < NUMOFTUBES; ++i) _vfdco_bcmp[i] = brightnessValue;
}
