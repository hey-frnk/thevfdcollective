/**
  ******************************************************************************
  * @file     LED.c
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
  *           SECTION_LED_COLOR_MUSIC
  *           SECTION_LED_COLOR_SERIAL
  *           SECTION_LED_MODE_MANAGER
  ******************************************************************************
 **/

#ifdef DEBUG
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "LED.h"
#include "ArduinoSIM.h"
extern unsigned long programruntime;

void clearInterface() {
  cF2 = 0;
  cF3 = 0;
}
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
  return (rand() % 256);
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

void render(/*const struct LED_Color *self*/) {
  printf("Rendering %hhu bytes of LED data with %hhu pixels and %hhu bytes per pixel.\n", num_bytes, num_rgb, (uint8_t)(num_bytes / num_rgb));
  printf("rgb_arr: ");
  for (uint8_t i = 0; i < num_rgb; ++i) {
    printf("(%03hhu, %03hhu, %03hhu) ", rgb_arr[3 * i], rgb_arr[3 * i + 1], rgb_arr[3 * i + 2]);
  }
  printf("\n\n");
}

#else

static inline void render(/*const struct LED_Color *self*/) {
  // uint32_t        t_f = self->t_f;
  // uint8_t         num_bytes = self->num_bytes,
  //                 *rgb_arr = self->rgb_arr;
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
    nbytes = num_bytes; // Byte counter for outer loop
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
void LED_Color_Init(/*struct LED_Color *self, */uint8_t num_pixel, uint8_t bpp, uint8_t *rgb, uint8_t *target) {
  num_rgb = num_pixel;
  num_bytes = num_pixel * bpp;
  rgb_arr = rgb;
  target_arr = target;

  // self->render = _LED_Color_render;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Implementation of void LED_Color_Mode::ledSmoothWrite (static void _LED_Color_Mode_ledSmoothWrite)
 **/
// static inline void _LED_Color_Mode_ledSmoothWrite(struct LED_Color_Mode *self) {
inline void ledSmoothWrite(void) {
  // const struct LED_Color      *l = self->l;
  // const uint8_t               num_bytes = num_bytes;
  // uint8_t * const             rgb_arr = rgb_arr;
  // uint8_t * const             target_arr = target_arr;

  // Obtain equality
  for (uint8_t i = 0; i < num_bytes; i++) {
    if (rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if (rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
  }
  render();
}

/**
  * @brief  Implementation of void LED_Color_Mode::ledDirectWrite (static void _LED_Color_Mode_ledDirectWrite)
 **/
/* static inline void _LED_Color_Mode_ledDirectWrite(struct LED_Color_Mode *self, const uint8_t *ledTarget) {
  const struct LED_Color      *l = self->l;
  const uint8_t               num_bytes = num_bytes; */
inline void ledDirectWrite(const uint8_t *ledTarget) {
  memcpy(rgb_arr, ledTarget, num_bytes);
  memcpy(target_arr, ledTarget, num_bytes);
  render();
}

/**
  * @brief  Implementation of virtual functions LED_Color_Mode::VTable (static void _LED_Color_Mode_F3)
 **/
static inline void _LED_Color_Mode_F3(const struct LED_Color_Mode *unsafe_self) {
  if(!unsafe_self->VTable.F3) return;
  unsafe_self->VTable.F3(unsafe_self);
}
static inline void _LED_Color_Mode_F3Var(const struct LED_Color_Mode *unsafe_self) {
  if(!unsafe_self->VTable.F3Var) return;
  unsafe_self->VTable.F3Var(unsafe_self);
}
static inline void _LED_Color_Mode_Update(const struct LED_Color_Mode *unsafe_self) {
  // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
  unsafe_self->VTable.Update(unsafe_self);
}
static inline void _LED_Color_Mode_Hello(const struct LED_Color_Mode *unsafe_self) {
  if(!unsafe_self->VTable.Hello) return;
  unsafe_self->VTable.Hello();
}

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self/*, struct LED_Color *l*/) {
  // self->l = l;

  // self->ledSmoothWrite = _LED_Color_Mode_ledSmoothWrite;
  // self->ledDirectWrite = _LED_Color_Mode_ledDirectWrite;

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
static void _LED_Color_Static_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;
  // const struct LED_Color *l = self->super.l;

  const uint8_t         position = self->position;
  //                      num_bytes = num_bytes;
  // uint8_t * const       target_arr = target_arr;

  // If not single Color
  if(position > LED0_mcOffset) {
    for(uint8_t i = 0; i < num_bytes; i++) target_arr[i] = led_Presets[position - LED0_cOffset][i];
    ledSmoothWrite();
  }
  else{ // Save some RAM
    for(uint8_t offset = 0; offset < num_bytes; offset += 3) {
      target_arr[offset] = led_scPresets[position][0];
      target_arr[offset + 1] = led_scPresets[position][1];
      target_arr[offset + 2] = led_scPresets[position][2];
    }
    ledSmoothWrite();
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::F3 (static void _LED_Color_Static_F3)
 **/
static void _LED_Color_Static_F3(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;
  // const struct LED_Color *l = self->super.l;

  // const uint8_t         num_rgb = num_rgb;

  self->position++;
  if(self->position == 16) self->position = 0;

  // Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < num_rgb; i++) LED0PMC[i] = pgm_read_byte_near(&LED0PM[self->position][i - 2]); // LED0PM[self->position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::Hello (static void _LED_Color_Static_Hello)
 **/
static inline void _LED_Color_Static_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self/*, struct LED_Color *l*/, uint8_t position) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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
static void _LED_Color_Spectrum_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  // Dereference const (read only) variables
  // const uint8_t         num_bytes = num_bytes;
  // uint8_t * const       target_arr = target_arr;
  const LED_S_t         saturation = self->saturation;
  const LED_L_t         lightness = self->lightness;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++;

  uint32_t phase = ledPhase(self->angle, saturation, lightness);

  for(uint8_t offset = 0; offset < num_bytes; offset += 3) {
    target_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    target_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    target_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
  }

  ledSmoothWrite();
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::F3 (static void _LED_Color_Spectrum_F3)
 **/
static void _LED_Color_Spectrum_F3(const struct LED_Color_Mode *unsafe_self) {
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
static void _LED_Color_Spectrum_F3Var(const struct LED_Color_Mode *unsafe_self) {
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
static inline void _LED_Color_Spectrum_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Spectrum class
 **/
void LED_Color_Spectrum_Init(struct LED_Color_Spectrum *self/*, struct LED_Color *l*/) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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
static void _LED_Color_Cross_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  // Dereference const (read only) variables
  const uint8_t         // num_rgb = num_rgb,
                        delta = self->delta;
  const LED_L_t         lightness = self->lightness;
  // uint8_t * const       rgb_arr = rgb_arr;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++; // Just let it overflow and begin from 0 :p
  uint8_t offset = 0;

  // Cycle position
  for(uint8_t i = 0; i < num_rgb; i++) {
    uint32_t phase = ledPhase(self->angle + (i * delta), 255, lightness);
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
    offset += 3;
  }
  render();
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::F3 (static void _LED_Color_Cross_F3)
 **/
static void _LED_Color_Cross_F3(const struct LED_Color_Mode *unsafe_self) {
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
static void _LED_Color_Cross_F3Var(const struct LED_Color_Mode *unsafe_self) {
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
static inline void _LED_Color_Cross_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cross class
 **/
void LED_Color_Cross_Init(struct LED_Color_Cross *self/*, struct LED_Color *l*/, uint8_t delta) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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
static void _LED_Color_Chase_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  // Dereference const (read only) variables
  const uint8_t         // num_bytes = num_bytes,
                        direction = self->direction,
                        MicPin = self->MicPin,
                        s = *(self->s);
  const LED_L_t         lightness = self->lightness;
  // uint8_t * const       rgb_arr = rgb_arr;

  if(direction < 3) {                                         // If reactive to second flip
    if(self->secondFlip != s) {                               // If the second has changed
      // Change chase fade direction
      if(direction == 2) self->directionFlag = !self->directionFlag;
      self->state = 0;                                        // Reset state machine
      self->secondFlip = s;                                   // Overwrite old second with new second
      self->angle += 22;                                      // Let it overflow and get different values.
    }
  }
  else {                                                      // If reactive to sound
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
    else offset = num_bytes - ((self->state * 3) + 3);        // If direction backward, then backward!
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);        // Manipulate G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);     // Manipulate R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);            // Manipulate B
  }
  render();

  if(updateIntervalEvent(&self->stateUpdater)) self->state++;
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3 (static void _LED_Color_Chase_F3)
 **/
static void _LED_Color_Chase_F3(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  const uint8_t         // num_rgb = num_rgb,
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
  for(uint8_t i = 2; i < num_rgb; i++) LED8PMC[i] = LED8PM[self->direction][i - 2];
  displayWrite(3, 0x00, 1000, LED8PMC);
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3Var (static void _LED_Color_Chase_F3Var)
 **/
static void _LED_Color_Chase_F3Var(const struct LED_Color_Mode *unsafe_self) {
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
static inline void _LED_Color_Chase_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Chase class
 **/
void LED_Color_Chase_Init(
  struct LED_Color_Chase *self,
  /*struct LED_Color *l,*/
  uint8_t MicPin,               // Pin number of microphone
  uint8_t *Second,              // Reference of a globally updated second variable
  uint8_t FlipSync,             // Flag to sync L-R <-> R-L chase flip with dot flip of display
  uint8_t ChaseMode             // Saved parameter chase mode (L-R/R-L/flip/mic)
) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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
static void _LED_Color_Resistor_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Resistor *self = (struct LED_Color_Resistor *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  // Dereference const (read only) variables
  const uint8_t         // num_rgb = num_rgb,
                        h = *(self->h),
                        m = *(self->m),
                        s = *(self->s);
  // uint8_t * const       target_arr = target_arr;

  uint8_t clockData[num_rgb];
  clockData[0] = s % 10;
  clockData[1] = s / 10;
  clockData[2] = m % 10;
  clockData[3] = m / 10;
  clockData[4] = h % 10;
  clockData[5] = h / 10;

  uint8_t offset = 0;
  for(uint8_t i = 0; i < num_rgb; i++) {
    target_arr[offset] = led_Resistor[clockData[i]][0];        // G
    target_arr[offset + 1] = led_Resistor[clockData[i]][1];    // R
    target_arr[offset + 2] = led_Resistor[clockData[i]][2];    // B
    offset += 3;
  }

  ledSmoothWrite();
}

/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Hello (static void _LED_Color_Resistor_Hello)
 **/
static inline void _LED_Color_Resistor_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void LED_Color_Resistor_Init(
  struct LED_Color_Resistor *self,
  /* struct LED_Color *l,*/
  // Reference of globally updated hour/minute/second variable
  uint8_t *h, uint8_t *m, uint8_t *s
) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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
static void _LED_Color_Cop_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Cop *self = (struct LED_Color_Cop *)unsafe_self;
  struct LED_Color_Mode *super = &self->super;
  const uint8_t         pattern = self->pattern;
                        // num_bytes = super->num_bytes;
  // uint8_t * const       rgb_arr = super->rgb_arr;

  if(updateIntervalEvent(&self->stateUpdater)) {
    if(self->state < 13) self->state++;
    else if(self->state == 13) self->state = 0;
  }

  if(pattern == 0) {
    if(self->state == 0) self->copHalfRender(super, 0, 1);         // b | r fill

    else if(self->state == 5) {
      for(uint8_t i = 0; i < num_bytes; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[0][j];
      render();                                                    // off fill
    }

    else if(self->state == 6) self->copHalfRender(super, 0, 1);    // b | r fill
    else if(self->state == 7) self->copHalfRender(super, 1, 0);    // r | b fill

    else if(self->state == 12) {
      for(uint8_t i = 0; i < num_bytes; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[1][j];
      render();                                                    // white fill
    }

    else if(self->state == 13) self->copHalfRender(super, 1, 0);   // r | b fill
  }
}

/**
  * @brief  Implementation of method LED_Color_Cop::copHalfRender (static inline void _LED_Color_Cop_copHalfRender)
 **/
static inline void _LED_Color_Cop_copHalfRender(struct LED_Color_Mode *rself, uint8_t left, uint8_t right) {
  // const struct LED_Color      *l = rself->l;
  // const uint8_t               num_bytes = rself->num_bytes;
  // uint8_t * const             rgb_arr = rself->rgb_arr;

  for(uint8_t i = 0; i < (num_bytes >> 1); i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[right][j];
  for(uint8_t i = (num_bytes >> 1); i < num_bytes; i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[left][j];
  render();
}

/**
  * @brief  Implementation of virtual function LED_Color_Cop::Hello (static void _LED_Color_Cop_Hello)
 **/
static inline void _LED_Color_Cop_Hello(void) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cop class
 **/
void LED_Color_Cop_Init(struct LED_Color_Cop *self/*, struct LED_Color *l*/, uint8_t pattern) {
  LED_Color_Mode_Init(&self->super/*, l*/);

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

/** Begin of:
  * @toc SECTION_LED_COLOR_MUSIC
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Music::Update (static void _LED_Color_Music_Update)
 **/
static void _LED_Color_Music_Update(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;
  // const struct LED_Color *l = self->super.l;
  // Dereference const (read only) variables
  const uint8_t         // num_bytes = num_bytes,
                        delta = self->delta,
                        MicPin = self->MicPin;
  const LED_L_t         lightness = self->lightness;
  // uint8_t * const       rgb_arr = rgb_arr;

  // If time interval passed, decrease turned on LEDs by one (regular state update)
  if(updateIntervalEvent(&self->stateUpdater)) if(self->state < 7) self->state++;

  // Cross fade LED color position update
  if(updateIntervalEvent(&self->angleUpdater)) self->angle++; // Just let it overflow and begin from 0 :p

  // Read microphone information, convert value to updateable state
  // Get mic data (log), divide by 36
  uint8_t rLevel = 6 - (uint8_t)(roundf(getMicData(MicPin) / 42.5));
  // Write the less valued LEDs only when the sound is actively changed

  if(self->state >= rLevel) {
    self->state = rLevel;                                        // If the new state is lower than the previous state: Overwrite current state with new rLevel (interrupt state)
    self->delayState = FALSE;                                        // Delay state = 0 (reset)
  }
  else{
    if(updateIntervalEvent(&self->delayUpdater)) self->delayState = TRUE;    // If the time has elapsed, write empty
  }

  if(self->state < 7) {                                           // If new information
    uint8_t offset = 0;

    for(uint8_t i = 0; i < (6 - self->state); i++) {
      uint32_t phase = ledPhase(self->angle + (i * delta), 255, lightness);
      rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);      // G
      rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);   // R
      rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);          // B
      offset += 3;
    }

    // And set all the others zero
    for(uint8_t lOffset = offset; lOffset < num_bytes; lOffset++) rgb_arr[lOffset] = 0;

    // Black out the inactives
    if(self->delayState) {
      if(self->state < 6) {
        uint8_t tOffset = 0;                                  // Temporary offset variable
        for(uint8_t i = 0; i < (5 - self->state); i++) {
          rgb_arr[tOffset] = 0;
          rgb_arr[tOffset + 1] = 0;
          rgb_arr[tOffset + 2] = 0;
          tOffset += 3;
        }
      }
    }

    render();
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Music::F3 (static void _LED_Color_Music_F3)
 **/
static void _LED_Color_Music_F3(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;

  // Higher delta: wider rainbow
  if(self->delta == 42) {
    self->state = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
  else if(self->state == 10) {
    self->state = 21;
    displayWrite(3, 0x00, 1000, LED7PM[1]);
  }
  else if(self->state == 21) {
    self->state = 42;
    displayWrite(3, 0x00, 1000, LED7PM[2]);
  }
  else{
    self->state = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Music::F3Var (static void _LED_Color_Music_F3Var)
 **/
static void _LED_Color_Music_F3Var(const struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;

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
  * @brief  Implementation of virtual function LED_Color_Music::Hello (static void _LED_Color_Music_Hello)
 **/
static inline void _LED_Color_Music_Hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSOUND + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Music class
 **/
void LED_Color_Music_Init(struct LED_Color_Music *self/*, struct LED_Color *l*/, uint8_t MicPin) {
  LED_Color_Mode_Init(&self->super/*, l*/);

  self->angle = 0;
  self->delta = 21;
  self->state = 0;
  self->delayState = FALSE;
  self->MicPin = MicPin;
  self->lightness = LED_L_HIGH;
  self->angleUpdater = newiE(60);
  self->stateUpdater = newiE(25);
  self->delayUpdater = newiE(250);

  struct LED_Color_Mode_VTable _music_vtable = {
    .F3 = _LED_Color_Music_F3,
    .F3Var = _LED_Color_Music_F3Var,
    .Update = _LED_Color_Music_Update,
    .Hello = _LED_Color_Music_Hello
  };
  self->super.VTable = _music_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_SERIAL
 **/
/**
  * @brief  Implementation of virtual functions LED_Color_Serial0/1::Update (static void _LED_Color_Serial0/1_Update)
 **/
static void _LED_Color_Serial0_Update(const struct LED_Color_Mode *unsafe_self) { return; }
static void _LED_Color_Serial1_Update(const struct LED_Color_Mode *unsafe_self) {
  // struct LED_Color_Serial1 *self = (struct LED_Color_Serial1 *)unsafe_self;
  ledSmoothWrite();
}

/**
  * @brief  Constructor of LED_Color_Serial0/1 class
 **/
void LED_Color_Serial0_Init(struct LED_Color_Serial0 *self/*, struct LED_Color *l*/) {
  LED_Color_Mode_Init(&self->super/*, l*/);

  struct LED_Color_Mode_VTable _serial0_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Serial0_Update,
    .Hello = NULL
  };
  self->super.VTable = _serial0_vtable;
}

void LED_Color_Serial1_Init(struct LED_Color_Serial1 *self/*, struct LED_Color *l*/) {
  LED_Color_Mode_Init(&self->super/*, l*/);

  struct LED_Color_Mode_VTable _serial1_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Serial1_Update,
    .Hello = NULL
  };
  self->super.VTable = _serial1_vtable;
}

/** Begin of:
  * @toc SECTION_LED_MODE_MANAGER
 **/
/**
  * @brief  Implementation of method LED_Mode_Manager::LED_Manager_Routine (static void _LED_Mode_Manager_Routine)
 **/
static void _LED_Mode_Manager_Routine(struct LED_Mode_Manager *self) {
  struct LED_Color_Mode *activeInstance = self->LED_Instance[self->LED_Instance_Position];
  activeInstance->Update(activeInstance);

  // F2 was pressed, switch color mode
  if(cF2 == SHORTPRESS) {
    ++self->LED_Instance_Position;
    if(self->LED_Instance_Position >= COLORPOS_SERIAL0) self->LED_Instance_Position = COLORPOS_STATIC;
    activeInstance = self->LED_Instance[self->LED_Instance_Position]; // Re-reference
    activeInstance->Hello(activeInstance);
    clearInterface();
  }

  // F3 was pressed, switch color parameter 1
  if(cF3 == SHORTPRESS) {
    activeInstance->F3(activeInstance);
    clearInterface();
  }

  // F3 was pressed, switch color parameter 2
  if(cF3 == LONGPRESS) {
    activeInstance->F3Var(activeInstance);
    clearInterface();
  }
}

/**
  * @brief  Implementation of method LED_Mode_Manager::EEPGenerate (static struct LED_SavedParam_Serialization _LED_Mode_Manager_EEPGenerate)
 **/
static struct LED_SavedParam_Serialization _LED_Mode_Manager_EEPGenerate(struct LED_Mode_Manager *self) {
  struct LED_SavedParam_Serialization s;
  s.led = (uint8_t)self->LED;
  struct LED_Color_Static *led_static = (struct LED_Color_Static *)(self->LED_Instance[COLORPOS_STATIC]);
  struct LED_Color_Cross *led_cfade = (struct LED_Color_Cross *)(self->LED_Instance[COLORPOS_CROSSFADE]);
  struct LED_Color_Chase *led_chfd = (struct LED_Color_Chase *)(self->LED_Instance[COLORPOS_CHASEFADE]);
  struct LED_Color_Cop *led_cop = (struct LED_Color_Cop *)(self->LED_Instance[COLORPOS_COP]);

  s.LED0P = led_static->position;
  s.LED7_delta = led_cfade->delta;
  s.LED8_dp = led_chfd->direction;
  s.LED11_pt = led_cop->pattern;

  return s;
}

/**
  * @brief  Implementation of method LED_Mode_Manager::EEPReadIn (static void _LED_Mode_Manager_EEPReadIn)
 **/
static void _LED_Mode_Manager_EEPReadIn(struct LED_Mode_Manager *self, struct LED_SavedParam_Serialization s) {
  self->LED = (LED_MODE_t)s.led;

  struct LED_Color_Static *led_static = (struct LED_Color_Static *)(self->LED_Instance[COLORPOS_STATIC]);
  struct LED_Color_Cross *led_cfade = (struct LED_Color_Cross *)(self->LED_Instance[COLORPOS_CROSSFADE]);
  struct LED_Color_Chase *led_chfd = (struct LED_Color_Chase *)(self->LED_Instance[COLORPOS_CHASEFADE]);
  struct LED_Color_Cop *led_cop = (struct LED_Color_Cop *)(self->LED_Instance[COLORPOS_COP]);

  if(led_static) led_static->position = s.LED0P;
  if(led_cfade) led_cfade->delta = s.LED7_delta;
  if(led_chfd) led_chfd->direction = s.LED8_dp;
  if(led_cop) led_cop->pattern = s.LED11_pt;
}

/**
  * @brief  Constructor of LED_Mode_Manager
 **/
void LED_Mode_Manager_Init(
  struct LED_Mode_Manager *self,
  // struct LED_Color *l,                    // Hardware instance
  struct LED_SavedParam_Serialization s   // Saved parameters
) {
  self->LED = (LED_MODE_t)s.led;
  // self->LED_Hardware = l;

  self->LED_Manager_Routine = _LED_Mode_Manager_Routine;
  self->EEPGenerate         = _LED_Mode_Manager_EEPGenerate;
  self->EEPReadIn           = _LED_Mode_Manager_EEPReadIn;

  LED_Color_Static_Init       (&led_static/*, l*/, s.LED0P);
  LED_Color_Serial0_Init      (&led_s0/*, l*/);
  LED_Color_Serial1_Init      (&led_s1/*, l*/);
  LED_Color_Spectrum_Init     (&led_spectrum/*, l*/);
  LED_Color_Cross_Init        (&led_cfade/*, l*/, s.LED7_delta);
  LED_Color_Chase_Init        (&led_chfd/*, l*/, micPin, &global_s, 0, s.LED8_dp);
  LED_Color_Resistor_Init     (&led_resistor/*, l*/, &global_h, &global_m, &global_s);
  LED_Color_Cop_Init          (&led_cop/*, l*/, s.LED11_pt);
  LED_Color_Music_Init        (&led_music/*, l*/, micPin);

  self->LED_Instance = (struct LED_Color_Mode **)malloc(COLORPOS_MAXCNT * sizeof(struct LED_Color_Mode *));
  self->LED_Instance[COLORPOS_STATIC]     = (struct LED_Color_Mode *) &led_static;
  self->LED_Instance[COLORPOS_SERIAL0]    = (struct LED_Color_Mode *) &led_s0;
  self->LED_Instance[COLORPOS_SERIAL1]    = (struct LED_Color_Mode *) &led_s1;
  self->LED_Instance[COLORPOS_FADE]       = (struct LED_Color_Mode *) &led_spectrum;
  self->LED_Instance[COLORPOS_CROSSFADE]  = (struct LED_Color_Mode *) &led_cfade;
  self->LED_Instance[COLORPOS_CHASEFADE]  = (struct LED_Color_Mode *) &led_chfd;
  self->LED_Instance[COLORPOS_RESISTOR]   = (struct LED_Color_Mode *) &led_resistor;
  self->LED_Instance[COLORPOS_COP]        = (struct LED_Color_Mode *) &led_cop;
  self->LED_Instance[COLORPOS_MUSIC]      = (struct LED_Color_Mode *) &led_music;

  switch (s.led) {
    case LED_STATIC:      self->LED_Instance_Position = COLORPOS_STATIC;      break;
    case LED_FADE:        self->LED_Instance_Position = COLORPOS_FADE;        break;
    case LED_CROSSFADE:   self->LED_Instance_Position = COLORPOS_CROSSFADE;   break;
    case LED_CHASEFADE:   self->LED_Instance_Position = COLORPOS_CHASEFADE;   break;
    case LED_RESISTOR:    self->LED_Instance_Position = COLORPOS_RESISTOR;    break;
    case LED_COP:         self->LED_Instance_Position = COLORPOS_COP;         break;
    case LED_MUSIC:       self->LED_Instance_Position = COLORPOS_MUSIC;       break;

    case LED_SERIAL_0: {
      self->LED_Instance_Position = COLORPOS_SERIAL0;
      for(uint8_t i = 0; i < num_bytes; ++i) rgb_arr[i] = s.SER0[i];
      render();
      break;
    }
    case LED_SERIAL_1: {
      self->LED_Instance_Position = COLORPOS_SERIAL1;
      for(uint8_t i = 0; i < num_bytes; ++i) target_arr[i] = s.SER1[i];
      break;
    }

    default:              self->LED_Instance_Position = COLORPOS_STATIC;      break;
  }
}


/**
  ******************************************************************************
  * @end      END OF FILE LED.c
  ******************************************************************************
 **/
