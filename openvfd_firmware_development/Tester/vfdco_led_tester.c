#include <stdlib.h>
#include <stdio.h>
#include "../vfdco_sk6812.h"
#include "../vfdco_color_lib.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
// gcc vfdco_led_tester.c vfdco_color_lib.c vfdco_sk6812.c vfdco_time.c -DDEBUG -std=c11 -Wall -o vfdco_led_tester

// #define TEST0
#define TEST1

void test0() {
  hsl_t *c1 = HSL_Init(0, 255, 127);
  hsl_t *c2 = HSL_Init(85, 255, 127);
  hsl_d_t d = {0, 0, 0};
  hsl_t *cArr[2] = {c1, c2};

  struct LED_Color *f1 = (struct LED_Color *)LED_Color_Fader_Init(20, LED_COLOR_BLEND_MODE_NORMAL, 0, 6, 1, cArr, 6, 7);
                      // (struct LED_Color *)LED_Color_Flasher_Init(LED_COLOR_BLEND_MODE_NORMAL, 0, 3, c1, 31, 3);
                      // (struct LED_Color *)LED_Color_Chaser_Init(4, LED_COLOR_BLEND_MODE_NORMAL, 0, 1, 6, c1, &d, 174, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);
  for(;;) {
    int q;
    while((q = getchar()) != '\n' && q != EOF);

    char c;
    printf("Button input: ");
    scanf("%c", &c);

    if(c <= 'm' && c > '0') {
      for(uint8_t i = 0; i < (c - '0'); ++i) {
        if(!f1->Next(f1)) {
          f1->Delete(f1);
          exit(1);
        }
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
}


/*uint_fast32_t             timer1_interval,        // Timer interval
  LED_COLOR_BLEND_MODE_t    blend_mode,             // Pixel blend setting.
  uint8_t                   start_pos,              // Pixel index to start
  int8_t                    repeat,                 // Repeat N times
  uint8_t                   length,                 // Pixel index to start
  hsl_t                     *pk,                    // Array of HSL colors
  hsl_d_t                   *pk_diff,               // Difference peaks
  uint16_t                  duration,               // Chase duration
  uint8_t                   cpreserving,            // Color preserving?
  uint8_t                   mode                    // Chase mode*/

void test1() {
  hsl_t *c1 = HSL_Init(0, 255, 127);
  hsl_d_t d = {0, 0, 0};

  struct LED_Color *tester[24] = {NULL};

  printf("L->R, NONPRESERVING, Start 1, End 6, t: 80\n");
  tester[0] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 0, 0, 6, c1, &d, 40, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_LR_LINEAR);

  printf("L->R, PRESERVINGFAS, Start 1, End 6, t: 80\n");
  tester[1] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 0, 0, 6, c1, &d, 40, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);

  printf("L->R, PRESERVINGSLO, Start 1, End 6, t: 80\n");
  tester[2] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 0, 0, 6, c1, &d, 40, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_LINEAR);

  /*for(uint8_t i = 0; i < 4; ++i) {


    tester[6 * i] =     (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 5, 0, 6, c1, &d, 156, i, LED_COLOR_CHASER_MODE_RL_LINEAR);
    tester[6 * i + 1] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 0, 0, 6, c1, &d, 166, i, LED_COLOR_CHASER_MODE_LR_LINEAR);
    tester[6 * i + 2] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 2, 0, 3, c1, &d, 176, i, LED_COLOR_CHASER_MODE_SPLITLIN);
    tester[6 * i + 3] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 5, 0, 6, c1, &d, 186, i, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
    tester[6 * i + 4] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 0, 0, 6, c1, &d, 186, i, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
    tester[6 * i + 5] = (struct LED_Color *)LED_Color_Chaser_Init(80, LED_COLOR_BLEND_MODE_NORMAL, 3, 0, 3, c1, &d, 196, i, LED_COLOR_CHASER_MODE_SPLITACC);
  }*/

  for(uint8_t i = 0; i < 24; ++i) {
    printf("Testing Unit: %hhu %p\n", i, tester[i]);
    while(tester[i]->Next(tester[i]));
    tester[i]->Delete(tester[i]);
    vfdco_clr_set_all_RGBW(0, 0, 0, 0);
  }
}

int main(void) {
  vfdco_clr_init(6);

  #ifdef TEST0
  test0();
  #endif

  #ifdef TEST1
  test1();
  #endif

  return 0;
}
