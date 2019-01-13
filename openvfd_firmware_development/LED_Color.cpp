#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arduino.h"
#include "LED_Color.hpp"

LED_Color::LED_Color(uint8_t num_pixel, uint8_t bpp, uint8_t pin) :
  NUM_RGB(num_pixel), NUM_BYTES(num_pixel * bpp)
{
  // Mapping conversion
  PORT = digitalPinToPort(pin);
  PORT_PIN = digitalPinToBitMask(pin);

  rgb_arr = (uint8_t *)calloc(NUM_BYTES, sizeof(uint8_t));
  target_arr = (uint8_t *)calloc(NUM_BYTES, sizeof(uint8_t));

  #ifdef DEBUG
    printf("LED Color instance created with pin %hhu (PORT: %hhu, PPIN: %hhu)\n", pin, PORT, PORT_PIN);
  #endif
}

LED_Color::~LED_Color() {
  free(rgb_arr);
  free(target_arr);
}

#ifdef DEBUG
  void LED_Color::render() {
    printf("Rendering %hhu bytes of LED data with %hhu pixels and %hhu bytes per pixel.\n", NUM_BYTES, NUM_RGB, (uint8_t)(NUM_BYTES / NUM_RGB));
    printf("rgb_arr: ");
    for(uint8_t i = 0; i < NUM_RGB; ++i) {
      printf("(%03hhu, %03hhu, %03hhu) ", rgb_arr[3 * i], rgb_arr[3 * i + 1], rgb_arr[3 * i + 2]);
    }
    printf("\n\n");
  }
#else
  void LED_Color::render() {
    if(!rgb_arr) return;

    while((micros() - t_f) < 50L);  // wait for 50us (data latch)

    cli(); // Disable interrupts so that timing is as precise as possible
    volatile uint8_t
     *p    = rgb_arr,   // Copy the start address of our data array
      val  = *p++,      // Get the current byte value & point to next byte
      high = PORT |  _BV(PORT_PIN), // Bitmask for sending HIGH to pin
      low  = PORT & ~_BV(PORT_PIN), // Bitmask for sending LOW to pin
      tmp  = low,       // Swap variable to adjust duty cycle
      nbits= 8;         // Bit counter for inner loop
    volatile uint16_t
      nbytes = NUM_BYTES; // Byte counter for outer loop
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
#endif

LED_Color_Mode::LED_Color_Mode(LED_Color *l) : l(l)
{
  // Number of pixels * bytes per pixel determine size of color array
  NUM_RGB = l->NUM_RGB;
  NUM_BYTES = l->NUM_BYTES;

  rgb_arr = l->rgb_arr;
  target_arr = l->target_arr;
}

LED_Color_Mode::~LED_Color_Mode() {}

void LED_Color_Mode::ledSmoothWrite() {
  // Obtain equality
  for(uint8_t i = 0; i < NUM_BYTES; i++) {
    if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
  }
  l->render();
}

void LED_Color_Mode::ledDirectWrite(uint8_t *ledTarget) {
  memcpy(rgb_arr, ledTarget, NUM_BYTES);
  memcpy(target_arr, ledTarget, NUM_BYTES);
  l->render();
}

void LED_Color_Mode::F3() { return; }
void LED_Color_Mode::F3var() { return; }
