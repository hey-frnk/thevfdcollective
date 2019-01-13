#include "LED_Color.hpp"
#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Color_R.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Color_Music.hpp"
#include "LED_Mode_Manager.hpp"
#include "Arduino.h"


extern uint8_t _h, _m, _s;
extern uint8_t cF2, cF3;
extern uint8_t micPin;

void clearInterface() {
  cF2 = 0;
  cF3 = 0;
}

LED_Mode_Manager::LED_Mode_Manager(LED_Color *l, struct LED_SavedParam_Serialization s) :
LED((LED_MODE_t)s.led), LED_Hardware(l), s(s),
led_static(nullptr), led_spectrum(nullptr), led_cfade(nullptr), led_chfd(nullptr), led_resistor(nullptr),
led_music(nullptr), led_cop(nullptr), LED_Instance(nullptr)
{
  led_static = new LED_Color_Static(LED_Hardware, s.LED0P);
  led_spectrum = new LED_Color_Spectrum(LED_Hardware);
  led_cfade = new LED_Color_Cross(LED_Hardware, s.LED7_delta);
  led_chfd = new LED_Color_Chase(LED_Hardware, micPin, &_s, 0, s.LED8_dp);
  led_resistor = new LED_Color_R(LED_Hardware, &_h, &_m, &_s);
  led_cop = new LED_Color_Cop(LED_Hardware, s.LED11_pt);
  led_music = new LED_Color_Music(LED_Hardware, micPin);
  led_static = new LED_Color_Static(LED_Hardware, s.LED0P);

  switch (s.led) {
    case LED_STATIC:      LED_Instance = led_static;    break;
    case LED_FADE:        LED_Instance = led_spectrum;  break;
    case LED_CROSSFADE:   LED_Instance = led_chfd;      break;
    case LED_CHASEFADE:   LED_Instance = led_chfd;      break;
    case LED_RESISTOR:    LED_Instance = led_resistor;  break;
    case LED_COP:         LED_Instance = led_cop;       break;
    default:              LED_Instance = led_static;    break;
  }
}

void LED_Mode_Manager::LED_Manager_Routine() {
  LED_Instance->loop();

  // F2 was pressed, switch color mode
  if(cF2 == SHORTPRESS) {
    switch(LED) {
      // Static: Next mode is cross fade
      case LED_STATIC:
        LED = LED_FADE;
        if(!led_spectrum) led_spectrum = new LED_Color_Spectrum(LED_Hardware);
        LED_Instance = led_spectrum;
        LED_Instance->hello();
        break;

      // Spectrum fade: Next mode is cross fade
      case LED_FADE:
        LED = LED_CROSSFADE;
        if(!led_cfade) led_cfade = new LED_Color_Cross(LED_Hardware, s.LED7_delta);
        LED_Instance = led_cfade;
        break;

      case LED_CROSSFADE:
        LED = LED_CHASEFADE;
        if(!led_chfd) led_chfd = new LED_Color_Chase(LED_Hardware, micPin, &_s, 0, s.LED8_dp);
        LED_Instance = led_chfd;
        break;

      case LED_CHASEFADE:
        LED = LED_RESISTOR;
        if(!led_resistor) led_resistor = new LED_Color_R(LED_Hardware, &_h, &_m, &_s);
        LED_Instance = led_resistor;
        break;

      case LED_RESISTOR:
        LED = LED_COP;
        if(!led_cop) led_cop = new LED_Color_Cop(LED_Hardware, s.LED11_pt);
        LED_Instance = led_cop;
        break;

      case LED_COP:
        LED = LED_MUSIC;
        if(!led_music) led_music = new LED_Color_Music(LED_Hardware, micPin);
        LED_Instance = led_music;
        break;

      default:
        LED = LED_STATIC;
        if(!led_static) led_static = new LED_Color_Static(LED_Hardware, s.LED0P);
        LED_Instance = led_static;
        break;
    }
    LED_Instance->hello();
    clearInterface();
  }

  // F3 was pressed, switch color parameter 1
  if(cF3 == SHORTPRESS) {
    LED_Instance->F3();
    clearInterface();
  }

  // F3 was pressed, switch color parameter 2
  if(cF3 == LONGPRESS) {
    LED_Instance->F3var();
    clearInterface();
  }
}

struct LED_SavedParam_Serialization LED_Mode_Manager::EEPGenerate() {
  s.led = (uint8_t)LED;
  if(led_static) s.LED0P = led_static->position;
  if(led_cfade) s.LED7_delta = led_cfade->delta;
  if(led_chfd) s.LED8_dp = led_chfd->direction;
  if(led_cop) s.LED11_pt = led_cop->pattern;
  return s;
}

void LED_Mode_Manager::EEPReadIn(struct LED_SavedParam_Serialization s) {
  LED = (LED_MODE_t)s.led;
  if(led_static) led_static->position = s.LED0P;
  if(led_cfade) led_cfade->delta = s.LED7_delta;
  if(led_chfd) led_chfd->direction = s.LED8_dp;
  if(led_cop) led_cop->pattern = s.LED11_pt;
}




// blaaaaaa
