#ifndef LED_MODEMGR_HPP
#define LED_MODEMGR_HPP

#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Color_R.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Color_Music.hpp"

extern uint8_t cF2, cF3, cF4;
extern const uint8_t micPin;
extern uint8_t global_h, global_m, global_s;

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

#define COLORPOS_STATIC      0
#define COLORPOS_FADE        1
#define COLORPOS_CROSSFADE   2
#define COLORPOS_CHASEFADE   3
#define COLORPOS_RESISTOR    4
#define COLORPOS_COP         5
#define COLORPOS_MUSIC       6

#define COLORPOS_SERIAL0     7
#define COLORPOS_SERIAL1     8

#define COLORPOS_MAXCNT      9

struct LED_SavedParam_Serialization {
  uint8_t led,
          LED0P,
          *SER0,
          *SER1,
          LED7_delta,
          LED8_dp,
          LED11_pt,
          LED21_DF,
          LED21_hEN,
          LED21_mEN,
          LED21_hDS,
          LED21_mDS;
};

class LED_Mode_Manager {
private:
  LED_MODE_t LED;
  LED_Color *LED_Hardware;
  struct LED_SavedParam_Serialization s;

  LED_Color_Mode **LED_Instance;
  uint8_t LED_Instance_Position;
public:
  LED_Mode_Manager(LED_Color *l, struct LED_SavedParam_Serialization s);
  ~LED_Mode_Manager();
  void LED_Manager_Routine();
  struct LED_SavedParam_Serialization EEPGenerate();
  void EEPReadIn(struct LED_SavedParam_Serialization s);
};

#endif
