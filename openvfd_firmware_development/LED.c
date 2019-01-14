/**
  ******************************************************************************
  * @file     LED.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-January-2019
  * @brief    This file contains definitions of a WS2812B library written for Arduino
  *           and the OpenVFD platform, providing expandable LED patterns
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  *           SECTION_SHARED_DEFINES
  *           SECTION_LED_COLOR
  *           SECTION_LED_COLOR_MODE
  *           SECTION_LED_COLOR_STATIC
  *           SECTION_LED_COLOR_SPECTRUM
  *           SECTION_LED_COLOR_CROSSFADE
  *           SECTION_LED_COLOR_CHASEFADE
  *           SECTION_LED_COLOR_RESISTOR
  *           SECTION_LED_COLOR_COP
  ******************************************************************************
 **/

#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "LED.h"

#ifdef DEBUG
#include "ArduinoSIM.h"
unsigned long programruntime = 0;
#else
#include <Arduino.h>
#endif

/** Begin of:
  * @toc SECTION_SHARED_DEFINES
 **/

// Faster color wheel with HSL transform and fixed point arithmetics
// https://stackoverflow.com/questions/2353211
static inline int32_t _mdisplay_fpmul(int32_t x, int32_t y) { return ((int64_t)x * (int64_t)y) / 0x01000000; }
static int32_t _mdisplay_hslp_hue2rgb(int32_t p, int32_t q, int32_t t) {
  if(t < 0x00000000) t += 0x01000000;
  if(t > 0x01000000) t -= 0x01000000;
  if(t < 0x002AAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), t));
  if(t < 0x00800000) return q;
  if(t < 0x00AAAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), 0x00AAAAAB - t));
  return p;
}

uint32_t ledPhase(uint8_t h, uint8_t s, uint8_t l) {
  int32_t hFP = _mdisplay_fpmul((int32_t)h << 23, 0x20202);
  int32_t sFP = _mdisplay_fpmul((int32_t)s << 23, 0x20202);
  int32_t lFP = _mdisplay_fpmul((int32_t)l << 23, 0x20202);

  int32_t rFP, gFP, bFP;
  if(s == 0) return ((uint32_t)l) | (((uint32_t)l) << 8) | (((uint32_t)l) << 16);
  else{
    int32_t q = (lFP < 0x00800000) ? _mdisplay_fpmul(lFP, 0x01000000 + sFP) : (lFP + sFP - _mdisplay_fpmul(lFP, sFP));
    int32_t p = _mdisplay_fpmul(0x02000000, lFP) - q;
    rFP = _mdisplay_hslp_hue2rgb(p, q, hFP + 0x00555555);
    gFP = _mdisplay_hslp_hue2rgb(p, q, hFP);
    bFP = _mdisplay_hslp_hue2rgb(p, q, hFP - 0x00555555);
  }

  return ((uint32_t)(_mdisplay_fpmul(0x3FC00000, gFP) >> 6 ) & 0x00FF0000) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, rFP) >> 14) & 0x0000FF00) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, bFP) >> 22) & 0x000000FF);
}

const uint8_t LED20_samplesCount = 8;                     // Empirical sample acquisition array length
uint16_t LED20_samples[LED20_samplesCount] = {0};         // Empirical sample acquisition array allocation
uint8_t LED20_sampleIndex = 0;                            // Buffer push sample index
const uint8_t LED20_analogSamples = 128;                  // How many samples per reading
const uint8_t LED20_sense = 2;                            // Noise threshold
const float LED20_linFactor = -0.50;                      // VU meter log mapping adjust: linFactor in [-1 ... inf], -1 <= lF < 0: log, 0: lin, > 0: exp

static inline uint16_t LED20_mapUnsigned16(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  if(in_max - in_min == 0) return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Dynamically adjust sensitivity according to amplitude (sort-of-AGC)
static uint16_t LED20_lLevelArr_vPush(uint16_t amplitudeIn) {
  // Queue push in
  if(LED20_sampleIndex < LED20_samplesCount) LED20_samples[LED20_sampleIndex++] = amplitudeIn;
  else{
    LED20_samples[LED20_sampleIndex = 0] = amplitudeIn;
    LED20_sampleIndex++;
  }

  // Control loop of maintaining healthy amplitude
  uint16_t lMin = 1023;
  uint16_t lMax = 0;
  for(uint8_t i = 0; i < LED20_samplesCount; i++) {
    if(LED20_samples[i] < lMin) lMin = LED20_samples[i];
    if(LED20_samples[i] > lMax) lMax = LED20_samples[i];
  }

  if(lMax - lMin < LED20_sense) return 0;
  uint16_t amplitudeAmplified = LED20_mapUnsigned16(amplitudeIn, lMin, lMax, 0, 255);
  return amplitudeAmplified;
}

// This function reads the microphone input and returns a value between 0 and 255
// Threshold sets the minimum value the mic is sensitive to. Must not be larger than 49 (not checked in the function, results division by zero otherwise)
uint8_t getMicData(uint8_t MicPin) {
  #ifdef DEBUG
  srand(0);
  #endif

  uint16_t dMicRead = 512;
  uint16_t dPrevSample = 512;

  // Obtain amplitude
  uint16_t dMicMax = 0;
  uint16_t dMicMin = 1023;
  for(uint8_t i = 0; i < LED20_analogSamples; i++) {
    dPrevSample = dMicRead;
    dMicRead = analogRead(MicPin);
    // Digital low pass filtering
    dMicRead = (dMicRead >> 2) + (dPrevSample >> 1) + (dPrevSample >> 2);

    // Get minimum and maximum
    if(dMicRead > dMicMax) dMicMax = dMicRead;
    else if(dMicRead < dMicMin) dMicMin = dMicRead;
  }

  float dMicA = (float)(LED20_lLevelArr_vPush(dMicMax - dMicMin));
  // Serial.println(dMicA);
  uint8_t u = dMicA ? (uint8_t)(roundf(powf(dMicA / 255.0, LED20_linFactor) * dMicA)) : 0;
  return u;
}

#ifdef DEBUG
struct intervalEvent newiE(long p1) {
  intervalEvent iE;
  iE.interval = p1;
  iE.previousMillis = 0;
  return iE;
}

void resetiE(intervalEvent *input) {
  input->previousMillis = 0;
}

uint8_t updateIntervalEvent(intervalEvent *input) {
  unsigned long currentMillis = programruntime;
  if((currentMillis - input->previousMillis) > input->interval) {
    input->previousMillis = currentMillis;
    return TRUE;
  }
  else return FALSE;
  return FALSE;
}
#endif

/** Begin of:
  * @toc SECTION_LED_COLOR
 **/

/**
  * @brief  Implementation of void LED_Color::render (static void _LED_Color_render)
 **/
#ifdef DEBUG

static void _LED_Color_render(const struct LED_Color *self) {
  printf("Rendering %hhu bytes of LED data with %hhu pixels and %hhu bytes per pixel.\n", self->NUM_BYTES, self->NUM_RGB, (uint8_t)(self->NUM_BYTES / self->NUM_RGB));
  printf("rgb_arr: ");
  for (uint8_t i = 0; i < self->NUM_RGB; ++i) {
    printf("(%03hhu, %03hhu, %03hhu) ", self->rgb_arr[3 * i], self->rgb_arr[3 * i + 1], self->rgb_arr[3 * i + 2]);
  }
  printf("\n\n");
}

#else

static void _LED_Color_render(const struct LED_Color *self) {
  uint32_t        t_f = self->t_f;
  uint8_t         NUM_RGB = self->NUM_RGB,
                  NUM_BYTES = self->NUM_BYTES,
                  *rgb_arr = self->rgb_arr;
  // This section is writF1 by:
  // Acrobotic - 01/10/2013
  // Author: x1sc0

  /*License:
    Beerware License; if you find the code useful, and we happen to cross
    paths, you're encouraged to buy us a beer. The code is distributed hoping
    that you in fact find it useful, but  without warranty of any kind.*/
  if (!rgb_arr) return;

  while ((micros() - t_f) < 50L); // wait for 50us (data latch)

  cli(); // Disable interrupts so that timing is as precise as possible
  volatile uint8_t
   *p    = rgb_arr,   // Copy the start address of our data array
    val  = *p++,      // Get the current byte value & point to next byte
    high = PORT |  _BV(PORT_PIN), // Bitmask for sending HIGH to pin
    low  = PORT & ~_BV(PORT_PIN), // Bitmask for sending LOW to pin
    tmp  = low,       // Swap variable to adjust duty cycle
    nbits= NUM_BITS;  // Bit counter for inner loop
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
/**
  * @brief  Constructor of LED_Color class
 **/
void LED_Color_Init(struct LED_Color *self, uint8_t num_pixel, uint8_t bpp, uint8_t *rgb, uint8_t *target) {
  self->NUM_RGB = num_pixel;
  self->NUM_BYTES = num_pixel * bpp;
  self->rgb_arr = rgb;
  self->target_arr = target;

  self->render = _LED_Color_render;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Implementation of void LED_Color_Mode::ledSmoothWrite (static void _LED_Color_Mode_ledSmoothWrite)
 **/
static void _LED_Color_Mode_ledSmoothWrite(struct LED_Color_Mode *self) {
  const struct LED_Color      *l = self->l;
  const uint8_t               NUM_BYTES = self->NUM_BYTES;
  uint8_t * const             rgb_arr = self->rgb_arr;
  uint8_t * const             target_arr = self->target_arr;

  // Obtain equality
  for (uint8_t i = 0; i < NUM_BYTES; i++) {
    if (rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if (rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
  }
  l->render(l);
}

/**
  * @brief  Implementation of void LED_Color_Mode::ledDirectWrite (static void _LED_Color_Mode_ledDirectWrite)
 **/
static void _LED_Color_Mode_ledDirectWrite(struct LED_Color_Mode *self, const uint8_t *ledTarget) {
  const struct LED_Color      *l = self->l;
  const uint8_t               NUM_BYTES = self->NUM_BYTES;

  memcpy(self->rgb_arr, ledTarget, NUM_BYTES);
  memcpy(self->target_arr, ledTarget, NUM_BYTES);
  l->render(l);
}

/**
  * @brief  Implementation of virtual functions LED_Color_Mode::VTable (static void _LED_Color_Mode_F3)
 **/
static inline void _LED_Color_Mode_F3(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.F3) return;
  self->VTable.F3(unsafe_self);
}
static inline void _LED_Color_Mode_F3Var(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.F3Var) return;
  self->VTable.F3Var(unsafe_self);
}
static inline void _LED_Color_Mode_Update(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
  self->VTable.Update(unsafe_self);
}
static inline void _LED_Color_Mode_Hello(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.Hello) return;
  self->VTable.Hello(unsafe_self);
}

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self, struct LED_Color *l) {
  self->l = l;

  self->NUM_RGB = l->NUM_RGB;
  self->NUM_BYTES = l->NUM_BYTES;
  self->rgb_arr = l->rgb_arr;
  self->target_arr = l->target_arr;

  self->ledSmoothWrite = _LED_Color_Mode_ledSmoothWrite;
  self->ledDirectWrite = _LED_Color_Mode_ledDirectWrite;

  self->F3 = _LED_Color_Mode_F3;
  self->F3Var = _LED_Color_Mode_F3Var;
  self->Update = _LED_Color_Mode_Update;
  self->Hello = _LED_Color_Mode_Hello;
}


/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Static::Update (static void _LED_Color_Static_Update)
 **/
static void _LED_Color_Static_Update(const void *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;

  const uint8_t         position = self->position,
                        NUM_BYTES = self->super.NUM_BYTES;
  uint8_t * const       target_arr = self->super.target_arr;

  // If not single Color
  if(position > LED0_mcOffset) {
    for(uint8_t i = 0; i < NUM_BYTES; i++) target_arr[i] = led_Presets[position - LED0_cOffset][i];
    self->super.ledSmoothWrite(&self->super);
  }
  else{ // Save some RAM
    for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
      target_arr[offset] = led_scPresets[position][0];
      target_arr[offset + 1] = led_scPresets[position][1];
      target_arr[offset + 2] = led_scPresets[position][2];
    }
    self->super.ledSmoothWrite(&self->super);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::F3 (static void _LED_Color_Static_F3)
 **/
static void _LED_Color_Static_F3(const void *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;

  const uint8_t         NUM_RGB = self->super.NUM_RGB;

  self->position++;
  if(self->position == 16) self->position = 0;

  // Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED0PMC[i] = LED0PM[self->position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::Hello (static void _LED_Color_Static_Hello)
 **/
static void _LED_Color_Static_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self, struct LED_Color *l, uint8_t position) {
  LED_Color_Mode_Init(&self->super, l);

  self->position = position;

  struct LED_Color_Mode_VTable _static_vtable = {
    .F3 = _LED_Color_Static_F3,
    .F3Var = NULL,
    .Update = _LED_Color_Static_Update,
    .Hello = _LED_Color_Static_Hello
  };
  self->super.VTable = _static_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_SPECTRUM
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::Update (static void _LED_Color_Spectrum_Update)
 **/
static void _LED_Color_Spectrum_Update(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_BYTES = self->super.NUM_BYTES;
  uint8_t * const       target_arr = self->super.target_arr;
  const LED_S_t         saturation = self->saturation;
  const LED_L_t         lightness = self->lightness;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++;

  uint32_t phase = ledPhase(self->angle, saturation, lightness);

  for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
    target_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    target_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    target_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
  }

  self->super.ledSmoothWrite(&self->super);
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::F3 (static void _LED_Color_Spectrum_F3)
 **/
static void _LED_Color_Spectrum_F3(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::F3Var (static void _LED_Color_Spectrum_F3Var)
 **/
static void _LED_Color_Spectrum_F3Var(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->saturation == LED_S_HIGH) {
    self->saturation = LED_S_LOW; // Change to super low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->saturation == LED_S_LOW) {
    self->saturation = LED_S_REG; // Change to low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->saturation = LED_S_HIGH; // Change to normal (high) sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::Hello (static void _LED_Color_Spectrum_Hello)
 **/
static void _LED_Color_Spectrum_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Spectrum class
 **/
void LED_Color_Spectrum_Init(struct LED_Color_Spectrum *self, struct LED_Color *l) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->lightness = LED_L_HIGH;
  self->saturation = LED_S_HIGH;
  self->angleUpdater = newiE(60);

  struct LED_Color_Mode_VTable _spectrum_vtable = {
    .F3 = _LED_Color_Spectrum_F3,
    .F3Var = _LED_Color_Spectrum_F3Var,
    .Update = _LED_Color_Spectrum_Update,
    .Hello = _LED_Color_Spectrum_Hello
  };
  self->super.VTable = _spectrum_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_CROSSFADE
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Cross::Update (static void _LED_Color_Cross_Update)
 **/
static void _LED_Color_Cross_Update(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        delta = self->delta;
  const LED_L_t         lightness = self->lightness;
  uint8_t * const       rgb_arr = self->super.rgb_arr;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++; // Just let it overflow and begin from 0 :p
  uint8_t offset = 0;

  // Cycle position
  for(uint8_t i = 0; i < NUM_RGB; i++) {
    uint32_t phase = ledPhase(self->angle + (i * delta), 255, lightness);
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
    offset += 3;
  }
  self->super.l->render(self->super.l);
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::F3 (static void _LED_Color_Cross_F3)
 **/
static void _LED_Color_Cross_F3(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;

  // Higher delta: wider rainbow
  if(self->delta == 42) {
    self->delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
  else if(self->delta == 10) {
    self->delta = 21;
    displayWrite(3, 0x00, 1000, LED7PM[1]);
  }
  else if(self->delta == 21) {
    self->delta = 42;
    displayWrite(3, 0x00, 1000, LED7PM[2]);
  }
  else{
    self->delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::F3Var (static void _LED_Color_Cross_F3Var)
 **/
static void _LED_Color_Cross_F3Var(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::Hello (static void _LED_Color_Cross_Hello)
 **/
static void _LED_Color_Cross_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cross class
 **/
void LED_Color_Cross_Init(struct LED_Color_Cross *self, struct LED_Color *l, uint8_t delta) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->lightness = LED_L_HIGH;
  self->delta = delta;
  self->angleUpdater = newiE(60);

  struct LED_Color_Mode_VTable _cross_vtable = {
    .F3 = _LED_Color_Cross_F3,
    .F3Var = _LED_Color_Cross_F3Var,
    .Update = _LED_Color_Cross_Update,
    .Hello = _LED_Color_Cross_Hello
  };
  self->super.VTable = _cross_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_CHASEFADE
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Chase::Update (static void _LED_Color_Chase_Update)
 **/
static void _LED_Color_Chase_Update(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_BYTES = self->super.NUM_BYTES,
                        direction = self->direction,
                        MicPin = self->MicPin,
                        s = *(self->s);
  const LED_L_t         lightness = self->lightness;
  uint8_t * const       rgb_arr = self->super.rgb_arr;

  if(direction < 3) {                                         // If reactive to second flip
    if(self->secondFlip != s) {                               // If the second has changed
      // Change chase fade direction
      if(direction == 2) self->directionFlag = !self->directionFlag;
      self->state = 0;                                        // Reset state machine
      self->secondFlip = s;                                   // Overwrite old second with new second
      self->angle += 22;                                      // Let it overflow and get different values.
    }
  }
  else{                                                       // If reactive to sound
    if(getMicData(MicPin) > 196) {                            // If the intensity of the audio samples are higher than 196 - threshold
      if(updateIntervalEvent(&self->delayUpdater)) {          // And some time has elapsed
        self->directionFlag = !self->directionFlag;           // Flip direction
        self->state = 0;                                      // Reset state machine
        self->angle += 29;                                    // And get some different color values!
      }
    }
  }

  if(self->state < 6) {                                       // Only run this code fragment if state is in range (< 6)
    uint32_t phase = ledPhase(self->angle, 255, lightness);   // Get new phase
    uint8_t offset = 0;
    if(!self->directionFlag) offset = self->state * 3;        // Get manipulating position
    else offset = NUM_BYTES - ((self->state * 3) + 3);        // If direction backward, then backward!
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);        // Manipulate G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);     // Manipulate R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);            // Manipulate B
  }
  self->super.l->render(self->super.l);

  if(updateIntervalEvent(&self->stateUpdater)) self->state++;
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3 (static void _LED_Color_Chase_F3)
 **/
static void _LED_Color_Chase_F3(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        flipdir = self->flipdir;

  // Short press results change in direction
  // direction = 0: From right to left (default)
  // direction = 1: From left to right
  // direction = 2: Direction flip
  // direction = 3: Flip on clap or any significant change in microphone input value
  self->direction++;
  if(self->direction == 0) self->directionFlag = FALSE;
  else if(self->direction == 1) self->directionFlag = TRUE;
  else if(self->direction == 2) self->directionFlag = !flipdir;
  else if(self->direction == 4) self->direction = 0;
  char LED8PMC[NUM_DIGITS_V];
  for(uint8_t i = 0; i < 2; i++) LED8PMC[i] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED8PMC[i] = LED8PM[self->direction][i - 2];
  displayWrite(3, 0x00, 1000, LED8PMC);
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3Var (static void _LED_Color_Chase_F3Var)
 **/
static void _LED_Color_Chase_F3Var(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::Hello (static void _LED_Color_Chase_Hello)
 **/
static void _LED_Color_Chase_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Chase class
 **/
void LED_Color_Chase_Init(
  struct LED_Color_Chase *self,
  struct LED_Color *l,
  uint8_t MicPin,               // Pin number of microphone
  uint8_t *Second,              // Reference of a globally updated second variable
  uint8_t FlipSync,             // Flag to sync L-R <-> R-L chase flip with dot flip of display
  uint8_t ChaseMode             // Saved parameter chase mode (L-R/R-L/flip/mic)
) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->secondFlip = 0;
  self->directionFlag = 0;
  self->state = 0;
  self->MicPin = MicPin;
  self->flipdir = FlipSync;
  self->s = Second;
  self->lightness = LED_L_HIGH;
  self->direction = ChaseMode;
  self->stateUpdater = newiE(60);
  self->delayUpdater = newiE(250);

  struct LED_Color_Mode_VTable _chase_vtable = {
    .F3 = _LED_Color_Chase_F3,
    .F3Var = _LED_Color_Chase_F3Var,
    .Update = _LED_Color_Chase_Update,
    .Hello = _LED_Color_Chase_Hello
  };
  self->super.VTable = _chase_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_RESISTOR
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Update (static void _LED_Color_Resistor_Update)
 **/
static void _LED_Color_Resistor_Update(const void *unsafe_self) {
  struct LED_Color_Resistor *self = (struct LED_Color_Resistor *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        h = *(self->h),
                        m = *(self->m),
                        s = *(self->s);
  uint8_t * const       target_arr = self->super.target_arr;

  uint8_t clockData[NUM_RGB];
  clockData[0] = s % 10;
  clockData[1] = s / 10;
  clockData[2] = m % 10;
  clockData[3] = m / 10;
  clockData[4] = h % 10;
  clockData[5] = h / 10;

  uint8_t offset = 0;
  for(uint8_t i = 0; i < NUM_RGB; i++) {
    target_arr[offset] = led_Resistor[clockData[i]][0];        // G
    target_arr[offset + 1] = led_Resistor[clockData[i]][1];    // R
    target_arr[offset + 2] = led_Resistor[clockData[i]][2];    // B
    offset += 3;
  }

  self->super.ledSmoothWrite(&self->super);
}

/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Hello (static void _LED_Color_Resistor_Hello)
 **/
static void _LED_Color_Resistor_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void LED_Color_Resistor_Init(
  struct LED_Color_Resistor *self,
  struct LED_Color *l,
  // Reference of globally updated hour/minute/second variable
  uint8_t *h, uint8_t *m, uint8_t *s
) {
  LED_Color_Mode_Init(&self->super, l);

  self->h = h;
  self->m = m;
  self->s = s;

  struct LED_Color_Mode_VTable _resistor_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Resistor_Update,
    .Hello = _LED_Color_Resistor_Hello
  };
  self->super.VTable = _resistor_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_COP
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Cop::Update (static void _LED_Color_Cop_Update)
 **/
static void _LED_Color_Cop_Update(const void *unsafe_self) {
  struct LED_Color_Cop *self = (struct LED_Color_Cop *)unsafe_self;
  struct LED_Color_Mode *super = &self->super;
  const uint8_t         pattern = self->pattern,
                        NUM_BYTES = super->NUM_BYTES;
  uint8_t * const       rgb_arr = super->rgb_arr;

  if(updateIntervalEvent(&self->stateUpdater)) {
    if(self->state < 13) self->state++;
    else if(self->state == 13) self->state = 0;
  }

  if(pattern == 0) {
    if(self->state == 0) self->copHalfRender(super, 0, 1);         // b | r fill

    else if(self->state == 5) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[0][j];
      super->l->render(super->l);                                  // off fill
    }

    else if(self->state == 6) self->copHalfRender(super, 0, 1);    // b | r fill
    else if(self->state == 7) self->copHalfRender(super, 1, 0);    // r | b fill

    else if(self->state == 12) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[1][j];
      super->l->render(super->l);                                  // white fill
    }

    else if(self->state == 13) self->copHalfRender(super, 1, 0);   // r | b fill
  }
}

/**
  * @brief  Implementation of method LED_Color_Cop::copHalfRender (static inline void _LED_Color_Cop_copHalfRender)
 **/
static inline void _LED_Color_Cop_copHalfRender(struct LED_Color_Mode *rself, uint8_t left, uint8_t right) {
  const struct LED_Color      *l = rself->l;
  const uint8_t               NUM_BYTES = rself->NUM_BYTES;
  uint8_t * const             rgb_arr = rself->rgb_arr;

  for(uint8_t i = 0; i < (NUM_BYTES >> 1); i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[right][j];
  for(uint8_t i = (NUM_BYTES >> 1); i < NUM_BYTES; i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[left][j];
  l->render(l);
}

/**
  * @brief  Implementation of virtual function LED_Color_Cop::Hello (static void _LED_Color_Cop_Hello)
 **/
static void _LED_Color_Cop_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cop class
 **/
void LED_Color_Cop_Init(struct LED_Color_Cop *self, struct LED_Color *l, uint8_t pattern) {
  LED_Color_Mode_Init(&self->super, l);

  self->pattern = pattern;
  self->state = 0;
  self->stateUpdater = newiE(25);

  self->copHalfRender = _LED_Color_Cop_copHalfRender;

  struct LED_Color_Mode_VTable _cop_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Cop_Update,
    .Hello = _LED_Color_Cop_Hello
  };
  self->super.VTable = _cop_vtable;
}










// hi hey hello
