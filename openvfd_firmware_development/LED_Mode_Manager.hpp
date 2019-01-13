#ifndef LED_MODEMGR_HPP
#define LED_MODEMGR_HPP

#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Color_R.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Color_Music.hpp"

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

  LED_Color_Static *led_static;
  LED_Color_Spectrum *led_spectrum;
  LED_Color_Cross *led_cfade;
  LED_Color_Chase *led_chfd;
  LED_Color_R *led_resistor;
  LED_Color_Music *led_music;
  LED_Color_Cop *led_cop;
  LED_Color_Mode *LED_Instance;
public:
  LED_Mode_Manager(LED_Color *l, struct LED_SavedParam_Serialization s);
  void LED_Manager_Routine();
  struct LED_SavedParam_Serialization EEPGenerate();
  void EEPReadIn(struct LED_SavedParam_Serialization s);
};

#endif
