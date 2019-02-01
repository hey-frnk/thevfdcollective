#include <stdio.h>
#include <stdlib.h>
#include "ArduinoSIM.h"
#include "LED.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
// gcc LED.c Main.c -DDEBUG -std=c11 -Wall -O3 -o capp

unsigned long programruntime = 0;
uint8_t cF2 = 0, cF3 = 0, cF4 = 0;
uint8_t global_h = 21, global_m = 37, global_s = 45;
const uint8_t micPin = 13;

int main () {
  uint8_t ra[18] = {0};
  uint8_t ta[18] = {0};

  // struct LED_Color LED_Hardware;
  // LED_Color_Init(&LED_Hardware, 6, 3, ra, ta);
  LED_Color_Init(6, 3, ra, ta);

  struct LED_Color_Music ledInstance;
  LED_Color_Music_Init(&ledInstance/*, &LED_Hardware*/, 13);

  struct LED_Color_Mode *virtualMode = NULL;
  virtualMode = (struct LED_Color_Mode *)&ledInstance;

  while(1) {
    int q;
    while((q = getchar()) != '\n' && q != EOF);

    char c;
    printf("Button input: ");
    scanf("%c", &c);

    switch(c) {
      case 'e': exit(0); break;
      case 'f': break;
      case 'g': virtualMode->F3((void *)virtualMode); break;
      case 'G': virtualMode->F3Var((void *)virtualMode); break;
      default: break;
    }

    int r;
    while((r = getchar()) != '\n' && r != EOF);

    int n;
    printf("Loop count: ");
    scanf("%d", &n);

    for(int i = 0; i < n; ++i) {
      virtualMode->Update((void *)virtualMode);
      programruntime += n;
    }

    ++global_s;
    if(global_s == 60) global_s = 0;
  }
}
