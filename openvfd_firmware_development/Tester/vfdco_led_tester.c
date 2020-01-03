#include <stdlib.h>
#include <stdio.h>
#include "../vfdco_sk6812.h"
#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
// gcc vfdco_led_tester.c ../Commons/vfdco_color_lib.c ../Commons/vfdco_lights.c vfdco_sk6812.c vfdco_time.c -DDEBUG -std=c11 -Wall -o vfdco_led_tester

//#define TEST0
// #define TEST1
 #define TEST2

void test0() {
  hsl_t *c1 = HSL_Init(0, 255, 127);
  hsl_t *c2 = HSL_Init(85, 255, 127);
  rgb_t *r1 = RGB_Init(33, 44, 55);
  hsl_d_t d = {0, 0, 0};
  hsl_t *cArr[2] = {c1, c2};

  struct LED_Color *f1 //= (struct LED_Color *)LED_Color_Fader_Init(20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_FOREVER, 2, cArr, 6, 0);
                       = (struct LED_Color *)LED_Color_Flasher_Init(20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_ONCE, r1, 31, 3);
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

  d.h = 1;
  cArr[0] = NULL;
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

  struct LED_Color *tester[18] = {NULL};

  int cs = 2;
  int cr = LED_COLOR_REPEAT_ONCE;
  int ce = 4;
  int ct = 32;
  tester[0] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[1] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[2] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[3] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_SPLITLIN);
  tester[4] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITLIN);
  tester[5] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITLIN);

  int rs = 5;
  int rr = LED_COLOR_REPEAT_ONCE;
  int re = 6;
  int rt = 64;
  tester[6] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[7] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[8] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[9] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_RL_LINEAR);
  tester[10] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_LINEAR);
  tester[11] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_LINEAR);

  int ls = 0;
  int lr = LED_COLOR_REPEAT_ONCE;
  int le = 6;
  int lt = 64;
  tester[12] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[13] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[14] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[15] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_LR_LINEAR);
  tester[16] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);
  tester[17] = (struct LED_Color *)LED_Color_Chaser_Init(1, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_LINEAR);

  for(uint8_t i = 0; i < 18; ++i) {
    printf("Testing Unit: %hhu %p\n", i, tester[i]);
    while(tester[i]->Next(tester[i]));
    tester[i]->Delete(tester[i]);
    vfdco_clr_set_all_RGBW(0, 0, 0, 0);
  }
}

void test2() {
  struct Light_Pattern_Static ledInstance;
  Light_Pattern_Static_Init(&ledInstance);

  struct Light_Pattern *virtualMode = NULL;
  virtualMode = (struct Light_Pattern *)&ledInstance;

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
    }
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

  #ifdef TEST2
  test2();
  #endif

  return 0;
}
