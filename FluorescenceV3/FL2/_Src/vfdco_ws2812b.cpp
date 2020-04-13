/*
 *
 *  Created on: Nov 18, 2019
 *      Author: HARDWARECOP
 */

#ifdef _LED_IMPLEMENTATION
#error "An implementation of the LED driver already exists!"
#endif
#define _LED_IMPLEMENTATION

#include <Arduino.h>
#include "vfdco_led.h"

#if (CONFIG_USE_RGBW == 1)
#error "Wrong driver linked. This driver only works for CONFIG_USE_RGBW == 0 (WS2812B). Fix: Set CONFIG_USE_RGBW to 0 or use SK6812 driver."
#endif

#define    LED_PIN    13    // ATMEGA: 19   LED Pin, LEDPIN

#define    PORT        (PORTB)          // Digital pin's port
#define    PORT_PIN    (PORTB5)         // Digital pin's bit position
#define    NUM_BITS    (8)              // Const 8

uint8_t	 rgb_arr[CONFIG_NUM_BYTES] = {0};
void render();

// Dimming
volatile uint8_t _led_dim_factor = 0;

#if CONFIG_ENABLE_GAMMACORRECTION == 1
static const uint8_t gamma8[] = { // Cheap gamma correction https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255};
#endif

void vfdco_clr_init(uint8_t initial_dim_factor) {
  pinMode(LED_PIN, OUTPUT);
  _led_dim_factor = initial_dim_factor;
}

void vfdco_clr_set_dim_factor(uint8_t dim_factor) {
  _led_dim_factor = dim_factor;
}

#if CONFIG_ENABLE_COLORCORRECTION == 1
static inline uint8_t _vfdco_clr_scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;
}
#endif

void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  #if CONFIG_ENABLE_GAMMACORRECTION == 1
  rgb_arr[3 * index] = gamma8[g];
  rgb_arr[3 * index + 1] = gamma8[r];
  rgb_arr[3 * index + 2] = gamma8[b];
  #else
    #if CONFIG_ENABLE_COLORCORRECTION == 1
    // TypicalLEDStrip = 0xFFB0F0: 255(R), 176(G), 240(B)
    rgb_arr[3 * index] = _vfdco_clr_scale8(g, 0xB0);
    rgb_arr[3 * index + 1] = r; // no scale
    rgb_arr[3 * index + 2] = _vfdco_clr_scale8(b, 0xF0);
    #else
    rgb_arr[3 * index] = g;
    rgb_arr[3 * index + 1] = r;
    rgb_arr[3 * index + 2] = b;
    #endif
  #endif
}
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  vfdco_clr_set_RGB(index, r, g, b);
}
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}

void vfdco_clr_target_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
  tp[3] = 0;
}
void vfdco_clr_target_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
  tp[3] = w;
}
void vfdco_clr_target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i += 4) vfdco_clr_target_RGB(tp + i, r, g, b);
}
void vfdco_clr_target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i += 4) vfdco_clr_target_RGBW(tp + i, r, g, b, w);
}

/**
 * @brief Smooth fading between intermediate target array and LED buffer
 * @param target_arr base address of intermediate target array
 */
void vfdco_clr_minimize_difference(uint8_t *target_arr) {
  uint8_t j = 0;
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i++) {
    if((i & 0x03) == 0x03) continue;
    if(rgb_arr[j] < target_arr[i]) rgb_arr[j]++;
    else if(rgb_arr[j] > target_arr[i]) rgb_arr[j]--;
    ++j;
  }
}

void vfdco_clr_render() {
  render();
}

// This section is writF1 by:
// Acrobotic - 01/10/2013
// Author: x1sc0

/*License:
  Beerware License; if you find the code useful, and we happen to cross
  paths, you're encouraged to buy us a beer. The code is distributed hoping
  that you in fact find it useful, but  without warranty of any kind.*/

uint32_t t_f = 0;

inline void render(void) {
  while((micros() - t_f) < 50L);  // wait for 50us (data latch)
  cli(); // Disable interrupts so that timing is as precise as possible
  static volatile uint8_t shifted_rgb_arr[CONFIG_NUM_BYTES];
  for(uint_fast8_t i = 0; i < CONFIG_NUM_BYTES; ++i) shifted_rgb_arr[i] = rgb_arr[i] >> _led_dim_factor;
  volatile uint8_t
   *p    = shifted_rgb_arr,   // Copy the start address of our data array
    val  = *p++, // Get the current byte value, dim & point to next byte
    high = PORT |  _BV(PORT_PIN), // Bitmask for sending HIGH to pin
    low  = PORT & ~_BV(PORT_PIN), // Bitmask for sending LOW to pin
    tmp  = low,       // Swap variable to adjust duty cycle
    nbits= NUM_BITS;  // Bit counter for inner loop
  volatile uint16_t
    nbytes = CONFIG_NUM_BYTES; // Byte counter for outer loop
  asm volatile(
    // Instruction        CLK     Description                 Phase
   "nextbit:\n\t"         // -    label                       (T =  0)
    "sbi  %0, %1\n\t"     // 2    signal HIGH                 (T =  2)
    "sbrc %4, 7\n\t"      // 1-2  if MSB set                  (T =  ?)
     "mov  %6, %3\n\t"    // 0-1   tmp'll set signal high     (T =  4)
    "dec  %5\n\t"         // 1    decrease bitcount           (T =  5)
    "nop\n\t"             // 1    nop (idle 1 clock cycle)    (T =  6)
    "st   %a2, %6\n\t"    // 2    set PORT to tmp             (T =  8)
    "mov  %6, %7\n\t"     // 1    reset tmp to low (default)  (T =  9)
    "breq nextbyte\n\t"   // 1-2  if bitcount ==0 -> nextbyte (T =  ?)
    "rol  %4\n\t"         // 1    shift MSB leftwards         (T = 11)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "rjmp nextbit\n\t"    // 2    bitcount !=0 -> nextbit     (T = 20)
   "nextbyte:\n\t"        // -    label                       -
    "ldi  %5, 8\n\t"      // 1    reset bitcount              (T = 11)
    "ld   %4, %a8+\n\t"   // 2    val = *p++                  (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "dec %9\n\t"          // 1    decrease bytecount          (T = 19)
    "brne nextbit\n\t"    // 2    if bytecount !=0 -> nextbit (T = 20)
    ::
    // Input operands         Operand Id (w/ constraint)
    "I" (_SFR_IO_ADDR(PORT)), // %0
    "I" (PORT_PIN),           // %1
    "e" (&PORT),              // %a2
    "r" (high),               // %3
    "r" (val),                // %4
    "r" (nbits),              // %5
    "r" (tmp),                // %6
    "r" (low),                // %7
    "e" (p),                  // %a8
    "w" (nbytes)              // %9
  );
  sei();                          // Enable interrupts
  t_f = micros();                 // t_f will be used to measure the 50us
                                  // latching period in the next call of the
                                  // function.
}