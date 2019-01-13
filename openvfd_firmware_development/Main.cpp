#if defined(__AVR_ATmega328P__)

#else

#include <iostream>
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

// cd documents/github/thevfdcollective/openvfd_firmware_development
// g++ Main.cpp LED_Color.cpp LED_Color_Static.cpp LED_Color_Spectrum.cpp LED_Color_Cross.cpp LED_Color_Chase.cpp LED_Color_R.cpp LED_Color_Cop.cpp LED_Color_Music.cpp LED_Shared.cpp LED_Mode_Manager.cpp -DDEBUG -std=c++11 -Wall -o Main

using namespace std;
extern unsigned long programruntime;
uint8_t global_h = 21, global_m = 37, global_s = 45;
uint8_t cF2 = 0, cF3 = 0, cF4 = 0;
const uint8_t micPin = 13;

int main () {
  LED_Color LED_Hardware = LED_Color(6, 3);
  struct LED_SavedParam_Serialization k = {
    (uint8_t)LED_COP,
    1,
    NULL,
    NULL,
    21,
    0,
    0,
    1,
    22,
    0,
    8,
    0
  };

  LED_Mode_Manager m(&LED_Hardware, k);

  while(1) {

    char c;
    int n, t;
    cout << "Next (task, loopincr, timeincr): ";
    cin >> ws;
    cin >> c >> n >> t;

    switch(c) {
      case 'c': {int c; cout << "newsec"; cin >> c; global_s = c; break;}
      case 'e': exit(0); break;
      case 'f': cF2 = SHORTPRESS; break;
      // case 'F': m->F2var(); break;
      case 'g': cF3 = SHORTPRESS; break;
      case 'G': cF3 = LONGPRESS; break;
      case 'S': cF4 = LONGPRESS; break;
      default: break;
    }

    for(int i = 0; i < n; ++i) {
      programruntime += t;
      m.LED_Manager_Routine();
    }
  }
}

#endif
