#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../vfdco_led.h"

#include "../vfdco_lights.h"
#include "../vfdco_display.h"
#include "../vfdco_hid.h"
#include "../vfdco_gui.h"
#include "../vfdco_clock_routines.h"
#include "../vfdco_serialization.h"

// cd documents/github/thevfdcollective/openvfd_firmware_development
/*
gcc vfdco_led_tester.c ../Commons/vfdco_lights.c ../Commons/vfdco_clock_routines.c ../Commons/vfdco_gui.c vfdco_fakedisplay.c vfdco_hid.c vfdco_fakeled.c vfdco_time.c -DDEBUG -std=c11 -Wall -o vfdco_led_tester
*/

#define ever (;;)

// #define FADE_TESTER
#define SIZE_TESTER
#define LOOP_TESTER
// #define SERIALIZE_TESTER

// Fader Tester
void test0() {
  struct LED_Color_Fader f1;
  LED_Color_Fader_Init(&f1, 20, HSL_Init(0, 255, 127), HSL_Init(0, 0, 0), 0);
  for(;;) {
    int q;
    while((q = getchar()) != '\n' && q != EOF);
    char c;
    printf("Button input: ");
    scanf("%c", &c);
    if(c <= 'm' && c > '0') for(uint8_t i = 0; i < (c - '0'); ++i) if(!LED_Color_Fader_Next(&f1)) exit(1);
    switch(c) {
      case 'E': exit(0); break;
      case 'F': break;
      case 'n': if(!LED_Color_Fader_Next(&f1)) exit(1); break;
      default: break;
    }
  }
}

// Size tester
void test1() {
  printf("LED_Color_Fader: %lu\n",               sizeof(struct LED_Color_Fader));

  printf("Container_Light_Pattern_t: %lu\n",     sizeof(Light_Pattern));
  printf("Light_Pattern_Static: %lu\n",          sizeof(struct Light_Pattern_Static));
  printf("Light_Pattern_Spectrum: %lu\n",        sizeof(struct Light_Pattern_Spectrum));
  printf("Light_Pattern_Rainbow: %lu\n",         sizeof(struct Light_Pattern_Rainbow));
  printf("Light_Pattern_Chase: %lu\n",           sizeof(struct Light_Pattern_Chase));
  printf("Light_Pattern_Time_Code: %lu\n",       sizeof(struct Light_Pattern_Time_Code));
  printf("Light_Pattern_MomentsOfBliss: %lu\n",  sizeof(struct Light_Pattern_MomentsOfBliss));

  printf("GUI_Format: %lu\n",                    sizeof(GUI_Format));
  printf("GUI_Format_Time_Date_Setter: %lu\n",   sizeof(struct GUI_Format_Time_Date_Setter));
  printf("GUI_Format_Time: %lu\n",               sizeof(struct GUI_Format_Time));
  printf("GUI_Format_Date: %lu\n",               sizeof(struct GUI_Format_Date));
  printf("GUI_Format_Time_Date_Setter: %lu\n",   sizeof(struct GUI_Format_Time_Date_Setter));
  printf("GUI_Format_Stopwatch: %lu\n",          sizeof(struct GUI_Format_Stopwatch));

  // for(int i = 0; i < 8; ++i) printf("%hhu, \n", settings_offset_array[i]);
}

// Loop Tester
void test2() {
  vfdco_clock_initializer();
  int execute_n_times = 1;
  for ever {
    // WoHOo hOw FuN iS tHhIis??
  	vfdco_clock_hid_routine();
  	vfdco_clock_time_routine();
  	vfdco_clock_display_routine();
  	vfdco_clock_lights_routine();
    vfdco_clock_serialization_routine();
  	// vfdco_clock_com_routine();
    if(--execute_n_times > 0) continue;
    int q; // loop breaker
    while((q = getchar()) != '\n' && q != EOF);
    char c;
    printf("1: F1, 2: F2, 3: F3, 4: F4\n5: F1Var, 6: F2Var, 7: F3Var, 8: F4Var\nn: Execute n times\n");
    printf("Button input: ");
    scanf("%c", &c);
    switch(c) {
      case 'e': exit(0); break;
      case '1': _vfdco_hid_button_set(150, BUTTON_F1); break;
      case '2': _vfdco_hid_button_set(150, BUTTON_F2); break;
      case '3': _vfdco_hid_button_set(150, BUTTON_F3); break;
      case '4': _vfdco_hid_button_set(150, BUTTON_F4); break;
      case '5': _vfdco_hid_button_set(800, BUTTON_F1); break;
      case '6': _vfdco_hid_button_set(800, BUTTON_F2); break;
      case '7': _vfdco_hid_button_set(800, BUTTON_F3); break;
      case '8': _vfdco_hid_button_set(800, BUTTON_F4); break;
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

void test3() {
  // vfdco_write_serialized(settings_serialized, NUM_SERIALIZABLE);
}

int main(void) {

  #ifndef TEST3
  vfdco_clr_init(6);
  vfdco_display_init(6);
  #endif

  #ifdef FADE_TESTER
  test0();
  #endif

  #ifdef SIZE_TESTER
  test1();
  #endif

  #ifdef LOOP_TESTER
  test2();
  #endif

  #ifdef SERIALIZE_TESTER
  test3();
  #endif

  return 0;
}
