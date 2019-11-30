#include <stdlib.h>
#include <stdio.h>
#include "vfdco_sk6812.h"
#include "vfdco_color_lib.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
// gcc vfdco_led_tester.c vfdco_color_lib.c vfdco_sk6812.c vfdco_time.c -DDEBUG -std=c11 -Wall -o vfdco_led_tester


int main(void) {
  hsl_t *c1 = HSL_Init(0, 255, 127);
  hsl_t *c2 = HSL_Init(85, 255, 127);
  hsl_d_t d = {0, 0, 0};
  hsl_t *cArr[2] = {c1, c2};

  // printf("c1: %08x, c2: %08x\n", _led_color_hsl2rgb(c1->h, c1->s, c1->l), _led_color_hsl2rgb(c2->h, c2->s, c2->l));

  vfdco_clr_init(6);

  struct LED_Color *f1 = // (struct LED_Color *)LED_Color_Fader_Init(LED_COLOR_BLEND_MODE_NORMAL, 0, 1, 2, cArr, 6, 0);
                         // (struct LED_Color *)LED_Color_Flasher_Init(LED_COLOR_BLEND_MODE_NORMAL, 0, 3, c1, 31, 3);
                         (struct LED_Color *)LED_Color_Chaser_Init(LED_COLOR_BLEND_MODE_NORMAL, 2, 0, 4, c1, &d, 38, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITLIN);

  for(;;) {
    int q;
    while((q = getchar()) != '\n' && q != EOF);

    char c;
    printf("Button input: ");
    scanf("%c", &c);

    if(c <= 'm' && c > '0') for(uint8_t i = 0; i < (c - '0'); ++i) {
      if(!f1->Next(f1)) {
        f1->Delete(f1);
        exit(1);
      }
    }

    switch(c) {
      case 'E': exit(0); break;
      case 'F': break;
      case 'n': {
        if(!f1->Next(f1)) {
          f1->Delete(f1);
          exit(1);
        }
        break;
      }
      default: break;
    }
  }

  HSL_Delete(c1);
  HSL_Delete(c2);
  return 0;
}
