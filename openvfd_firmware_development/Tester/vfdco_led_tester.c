#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../vfdco_led.h"
#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"
#include "../vfdco_display.h"
#include "../vfdco_hid.h"
#include "../vfdco_gui.h"
#include "../vfdco_clock_routines.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
/*
gcc vfdco_led_tester.c ../Commons/vfdco_color_lib.c ../Commons/vfdco_lights.c ../Commons/vfdco_clock_routines.c ../Commons/vfdco_gui.c vfdco_fakedisplay.c vfdco_hid.c vfdco_fakeled.c vfdco_time.c -DDEBUG -std=c11 -Wall -o vfdco_led_tester
*/

#define ever (;;)

// #define TEST0
// #define TEST1
#define TEST2
 //#define TEST3 // gets real

void test0() {
  hsl_t c1 = HSL_Init(0, 255, 127);
  // hsl_t c2 = HSL_Init(85, 255, 127);
  hsl_t c3 = HSL_Init(0, 0, 0);
  // rgb_t *r1 = RGB_Init(33, 44, 55);
  hsl_d_t d = {0, 0, 0};

  struct LED_Color_Fader f1;
  LED_Color_Fader_Init(
    &f1,
    20,
    #ifdef LED_COLOR_FADER_EXTENDED
    0,
    #endif
    LED_COLOR_REPEAT_FOREVER,
    c1,
    c3,
    #ifdef LED_COLOR_FADER_EXTENDED
    6,
    #endif
    0
  );
                       //= (struct LED_Color *)LED_Color_Flasher_Init(20, /*LED_COLOR_BLEND_MODE_NORMAL,*/ 0, LED_COLOR_REPEAT_ONCE, r1, 31, 3);
                       //= (struct LED_Color *)LED_Color_Chaser_Init(4, /*LED_COLOR_BLEND_MODE_NORMAL,*/ 0, LED_COLOR_REPEAT_FOREVER, 6, c1, &d, 31, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);
  for(;;) {
    int q;
    while((q = getchar()) != '\n' && q != EOF);

    char c;
    printf("Button input: ");
    scanf("%c", &c);

    if(c <= 'm' && c > '0') {
      for(uint8_t i = 0; i < (c - '0'); ++i) {
        if(
          #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
          !f1.super.Next(&f1.super)
          #else
          !f1.Next(&f1)
          #endif
        ) {
          exit(1);
        }
      }
    }

    switch(c) {
      case 'E': exit(0); break;
      case 'F': break;
      case 'n': {
        if(
          #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
          !f1.super.Next(&f1.super)
          #else
          !f1.Next(&f1)
          #endif
        ) {
          exit(1);
        }
        break;
      }
      default: break;
    }
  }

  d.h = 1;
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

/* void test1() {
  hsl_t *c1 = HSL_Init(0, 255, 127);
  hsl_d_t d = {0, 0, 0};

  struct LED_Color *tester[18] = {NULL};

  int cs = 2;
  int cr = LED_COLOR_REPEAT_ONCE;
  int ce = 4;
  int ct = 32;
  tester[0] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[1] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[2] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[3] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_SPLITLIN);
  tester[4] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITLIN);
  tester[5] = (struct LED_Color *)LED_Color_Chaser_Init(1, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITLIN);

  int rs = 5;
  int rr = LED_COLOR_REPEAT_ONCE;
  int re = 6;
  int rt = 64;
  tester[6] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[7] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[8] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[9] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_RL_LINEAR);
  tester[10] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_LINEAR);
  tester[11] = (struct LED_Color *)LED_Color_Chaser_Init(1, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_LINEAR);

  int ls = 0;
  int lr = LED_COLOR_REPEAT_ONCE;
  int le = 6;
  int lt = 64;
  tester[12] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[13] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[14] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[15] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_NON_PRESERVING, LED_COLOR_CHASER_MODE_LR_LINEAR);
  tester[16] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);
  tester[17] = (struct LED_Color *)LED_Color_Chaser_Init(1, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_LINEAR);

  for(uint8_t i = 0; i < 18; ++i) {
    printf("Testing Unit: %hhu %p\n", i, tester[i]);
    while(tester[i]->Next(tester[i]));
    tester[i]->Delete(tester[i]);
    vfdco_clr_set_all_RGBW(0, 0, 0, 0);
  }
}*/

void test2() {
  printf("LED_Color_Fader: %lu\n",               sizeof(struct LED_Color_Fader));
  printf("LED_Color_Chaser: %lu\n",              sizeof(struct LED_Color_Chaser));

  printf("Light_Pattern: %lu\n",                 sizeof(struct Light_Pattern));
  printf("Light_Pattern_Static: %lu\n",          sizeof(struct Light_Pattern_Static));
  printf("Light_Pattern_Spectrum: %lu\n",        sizeof(struct Light_Pattern_Spectrum));
  printf("Light_Pattern_Rainbow: %lu\n",         sizeof(struct Light_Pattern_Rainbow));
  printf("Light_Pattern_Chase: %lu\n",           sizeof(struct Light_Pattern_Chase));
  printf("Light_Pattern_Time_Code: %lu\n",       sizeof(struct Light_Pattern_Time_Code));
  printf("Light_Pattern_MomentsOfBliss: %lu\n",  sizeof(struct Light_Pattern_MomentsOfBliss));
  printf("Container_Light_Pattern_t: %lu\n",     sizeof(Container_Light_Pattern_t));

  printf("GUI_Format: %lu\n",                    sizeof(struct GUI_Format));
  printf("GUI_Format_Time_Date_Setter: %lu\n",   sizeof(struct GUI_Format_Time_Date_Setter));
  printf("GUI_Format_Time: %lu\n",               sizeof(struct GUI_Format_Time));
  printf("GUI_Format_Date: %lu\n",               sizeof(struct GUI_Format_Date));
  printf("GUI_Format_Time_Date_Setter: %lu\n",   sizeof(struct GUI_Format_Time_Date_Setter));
  printf("GUI_Format_Stopwatch: %lu\n",          sizeof(struct GUI_Format_Stopwatch));
}

// SH... gets real
void test3() {
  vfdco_clock_initializer();

  int execute_n_times = 1;

  for ever {
    // WoHOo hOw FuN iS tHhIis??
  	vfdco_clock_hid_routine();
  	vfdco_clock_time_routine();
  	vfdco_clock_display_routine();
  	vfdco_clock_lights_routine();
  	vfdco_clock_com_routine();

    if(--execute_n_times > 0) continue;

    // loop breaker
    int q;
    while((q = getchar()) != '\n' && q != EOF);

    char c;
    printf("1: F1, 2: F2, 3: F3, 4: F4\n5: F1Var, 6: F2Var, 7: F3Var, 8: F4Var\nn: Execute n times\n");
    printf("Button input: ");

    scanf("%c", &c);

    switch(c) {
      // out
      case 'e': exit(0); break;

      // shortpress
      case '1': _vfdco_hid_button_set(150, BUTTON_F1); break;
      case '2': _vfdco_hid_button_set(150, BUTTON_F2); break;
      case '3': _vfdco_hid_button_set(150, BUTTON_F3); break;
      case '4': _vfdco_hid_button_set(150, BUTTON_F4); break;

      // longpress
      case '5': _vfdco_hid_button_set(800, BUTTON_F1); break;
      case '6': _vfdco_hid_button_set(800, BUTTON_F2); break;
      case '7': _vfdco_hid_button_set(800, BUTTON_F3); break;
      case '8': _vfdco_hid_button_set(800, BUTTON_F4); break;

      // run n times
      case 'n': {
        int r;
        while((r = getchar()) != '\n' && r != EOF);

        int n;
        printf("Loop count: ");
        scanf("%d", &n);

        execute_n_times = n;
      }
    }
  }
}

int random_generated(int min, int max) {
  return (rand() % (max + 1 - min)) + min;
}


int main(void) {

  #ifndef TEST3
  vfdco_clr_init(6);
  vfdco_display_init(6);
  #endif

  #ifdef TEST0
  test0();
  #endif

  #ifdef TEST1
  // test1();
  #endif

  #ifdef TEST2
  test2();
  #endif

  #ifdef TEST3
  test3();
  #endif

  return 0;
}
