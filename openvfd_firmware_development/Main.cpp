#include <iostream>
#include "LED_Color.hpp"
#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"

// cd documents/github/thevfdcollective/openvfd_firmware_development
// g++ Main.cpp LED_Color.cpp LED_Color_Static.cpp LED_Color_Spectrum.cpp LED_Color_Cross.cpp LED_Shared.cpp -DDEBUG -std=c++11 -Wall -o Main

using namespace std;
extern unsigned long programruntime;

LED_Color *WS2812B = new LED_Color(6, 3, 13);
LED_Color_Static led_static = LED_Color_Static(WS2812B);
LED_Color_Spectrum led_spectrum = LED_Color_Spectrum(WS2812B);
LED_Color_Cross led_cfade = LED_Color_Cross(WS2812B);

LED_Color_Mode *m = &led_static;

void mChanger(LED_Color_Mode **mode) {
  if(         dynamic_cast<LED_Color_Static *>      (*mode))    {*mode = &led_spectrum;     printf("Changed to Spectrum fade\n");}
  else if(    dynamic_cast<LED_Color_Spectrum *>    (*mode))    {*mode = &led_cfade;        printf("Changed to Cross fade\n");}
  else if(    dynamic_cast<LED_Color_Cross *>       (*mode))    {*mode = &led_static;       printf("Changed to Static fade\n");}
}

int main () {

  while(1) {

    char c;
    int n, t;
    cout << "Next (task, loopincr, timeincr): ";
    cin >> ws;
    cin >> c >> n >> t;

    switch(c) {
      case 'e': exit(0); break;
      case 'f': mChanger(&m); m->F2(); break;
      case 'F': m->F2var(); break;
      case 'g': m->F3(); break;
      case 'G': m->F3var(); break;
      default: break;
    }

    for(int i = 0; i < n; ++i) {
      m->loop();
      programruntime += t;
    }
  }
}
