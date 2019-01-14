#include "LED_Color.hpp"
#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Color_R.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Color_Music.hpp"
#include "LED_Color_Serial.hpp"
#include "LED_Mode_Manager.hpp"

#ifdef DEBUG

#include "ArduinoSIM.h"
void clearInterface() {
  cF2 = 0;
  cF3 = 0;
  cF4 = 0;
}

#else

#include <Arduino.h>
extern void clearInterface();

#endif

LED_Mode_Manager::LED_Mode_Manager(LED_Color *l, struct LED_SavedParam_Serialization s) :
LED((LED_MODE_t)s.led), LED_Hardware(l), s(s)
/*led_static(nullptr), led_spectrum(nullptr), led_cfade(nullptr), led_chfd(nullptr), led_resistor(nullptr),
led_music(nullptr), led_cop(nullptr), LED_Instance(nullptr)*/
{
  LED_Instance = new LED_Color_Mode *[COLORPOS_MAXCNT];
  LED_Instance[COLORPOS_STATIC]     = new LED_Color_Static(LED_Hardware, s.LED0P);
  LED_Instance[COLORPOS_SERIAL0]    = new LED_Color_Serial0(LED_Hardware);
  LED_Instance[COLORPOS_SERIAL1]    = new LED_Color_Serial1(LED_Hardware);
  LED_Instance[COLORPOS_FADE]       = new LED_Color_Spectrum(LED_Hardware);
  LED_Instance[COLORPOS_CROSSFADE]  = new LED_Color_Cross(LED_Hardware, s.LED7_delta);
  LED_Instance[COLORPOS_CHASEFADE]  = new LED_Color_Chase(LED_Hardware, micPin, &global_s, 0, s.LED8_dp);
  LED_Instance[COLORPOS_RESISTOR]   = new LED_Color_R(LED_Hardware, &global_h, &global_m, &global_s);
  LED_Instance[COLORPOS_COP]        = new LED_Color_Cop(LED_Hardware, s.LED11_pt);
  LED_Instance[COLORPOS_MUSIC]      = new LED_Color_Music(LED_Hardware, micPin);

  switch (s.led) {
    case LED_STATIC:      LED_Instance_Position = COLORPOS_STATIC;      break;
    case LED_FADE:        LED_Instance_Position = COLORPOS_FADE;        break;
    case LED_CROSSFADE:   LED_Instance_Position = COLORPOS_CROSSFADE;   break;
    case LED_CHASEFADE:   LED_Instance_Position = COLORPOS_CHASEFADE;   break;
    case LED_RESISTOR:    LED_Instance_Position = COLORPOS_RESISTOR;    break;
    case LED_COP:         LED_Instance_Position = COLORPOS_COP;         break;
    case LED_MUSIC:       LED_Instance_Position = COLORPOS_MUSIC;       break;

    case LED_SERIAL_0: {
      LED_Instance_Position = COLORPOS_SERIAL0;
      for(uint8_t i = 0; i < LED_Hardware->NUM_BYTES; ++i) LED_Hardware->rgb_arr[i] = s.SER0[i];
      LED_Hardware->render();
      break;
    }
    case LED_SERIAL_1: {
      LED_Instance_Position = COLORPOS_SERIAL1;
      for(uint8_t i = 0; i < LED_Hardware->NUM_BYTES; ++i) LED_Hardware->target_arr[i] = s.SER1[i];
      break;
    }

    default:              LED_Instance_Position = COLORPOS_STATIC;      break;
  }
}

LED_Mode_Manager::~LED_Mode_Manager() {
  for(uint8_t i = 0; i < COLORPOS_MAXCNT; ++i) if(LED_Instance[i]) delete LED_Instance[i];
  delete[] LED_Instance;
}

void LED_Mode_Manager::LED_Manager_Routine() {
  LED_Instance[LED_Instance_Position]->loop();

  // F2 was pressed, switch color mode
  if(cF2 == SHORTPRESS) {
    ++LED_Instance_Position;
    if(LED_Instance_Position == COLORPOS_SERIAL0) LED_Instance_Position = COLORPOS_STATIC;
    LED_Instance[LED_Instance_Position]->hello();
    clearInterface();
  }

  // F3 was pressed, switch color parameter 1
  if(cF3 == SHORTPRESS) {
    LED_Instance[LED_Instance_Position]->F3();
    clearInterface();
  }

  // F3 was pressed, switch color parameter 2
  if(cF3 == LONGPRESS) {
    LED_Instance[LED_Instance_Position]->F3var();
    clearInterface();
  }

  #ifdef DEBUG
  if(cF4 == LONGPRESS) {
    struct LED_SavedParam_Serialization k = EEPGenerate();
    printf("LED: %hhu\n", k.led);
    printf("LED0P: %hhu\n", k.LED0P);
    printf("LED7_delta: %hhu\n", k.LED7_delta);
    printf("LED8_dp: %hhu\n", k.LED8_dp);
    printf("LED11_pt: %hhu\n", k.LED11_pt);
    clearInterface();
  }
  #endif
}

struct LED_SavedParam_Serialization LED_Mode_Manager::EEPGenerate() {
  s.led = (uint8_t)LED;
  LED_Color_Static *led_static = (LED_Color_Static *)(LED_Instance[COLORPOS_STATIC]);
  LED_Color_Cross *led_cfade = (LED_Color_Cross *)(LED_Instance[COLORPOS_CROSSFADE]);
  LED_Color_Chase *led_chfd = (LED_Color_Chase *)(LED_Instance[COLORPOS_CHASEFADE]);
  LED_Color_Cop *led_cop = (LED_Color_Cop *)(LED_Instance[COLORPOS_COP]);

  if(led_static) s.LED0P = led_static->position;
  if(led_cfade) s.LED7_delta = led_cfade->delta;
  if(led_chfd) s.LED8_dp = led_chfd->direction;
  if(led_cop) s.LED11_pt = led_cop->pattern;
  return s;
}

void LED_Mode_Manager::EEPReadIn(struct LED_SavedParam_Serialization s) {
  LED = (LED_MODE_t)s.led;

  LED_Color_Static *led_static = (LED_Color_Static *)(LED_Instance[COLORPOS_STATIC]);
  LED_Color_Cross *led_cfade = (LED_Color_Cross *)(LED_Instance[COLORPOS_CROSSFADE]);
  LED_Color_Chase *led_chfd = (LED_Color_Chase *)(LED_Instance[COLORPOS_CHASEFADE]);
  LED_Color_Cop *led_cop = (LED_Color_Cop *)(LED_Instance[COLORPOS_COP]);

  if(led_static) led_static->position = s.LED0P;
  if(led_cfade) led_cfade->delta = s.LED7_delta;
  if(led_chfd) led_chfd->direction = s.LED8_dp;
  if(led_cop) led_cop->pattern = s.LED11_pt;
}




// blaaaaaa
