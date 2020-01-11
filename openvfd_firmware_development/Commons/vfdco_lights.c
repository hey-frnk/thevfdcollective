/**
  ******************************************************************************
  * @file     vfdco_color_mode.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains implementations for color modes
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"
#include "../vfdco_display.h"
#include <stdlib.h>
#ifdef DEBUG
#include <stdio.h>
#endif

#define MESSAGE_LENGTH 6
#define MESSAGE_SHORT   250
#define MESSAGE_LONG    1000

/*static const hsl_t Static_Colors_Special[NUM_STATIC_COLOR_SPECIAL] = {
  {.h =   0,   .s =   0,   .l =   0}, // Off (Black)
  {.h =   0,   .s =   0,   .l = 255}  // White
};*/

static const uint8_t Static_Color_Hues[NUM_STATIC_COLOR_HUES] = {
  // As always the sat is 255, lightness 127, only memorize hues
    0, // Red      , also Red --> Blue
   85, // Green    , also Red --> Green
  160, // Blue,    , also Blue --> Green
   25, // Yellow
    5, // Orange
  128, // Cyan
  240, // Magenta
  205  // Purple
};

/* static const int8_t Static_Color_Presets[NUM_STATIC_COLOR_PRESETS] = {
   -17, // Difference to get from green to blue with num_rgb == 6
   -17, // Difference to get from red to green
   -17  // Difference to get from green to blue
}; */

/*static const uint8_t Static_Color_Rainbow_Lightness[NUM_STATIC_COLOR_RAINBOWS] = {
  127,
  170
};
static const uint8_t Static_Color_Rainbow_Saturation[NUM_STATIC_COLOR_RAINBOWS] = {
  255,
  196
};*/

// Caution: G-R-B ordering (due to legacy copy & paste, sorry)
static const uint8_t Static_Color_Rainbows[][6 * 3] =
{
  {  0, 200, 255,  // Rainbow colors!
     0,   0, 255,
   255,   0,   0,
   128, 255,   0,
    30, 255,   0,
     0, 255,   0
  },
  {128, 255, 255,  // Pastel rainbow!
   128,  50, 255,
   255,   0, 128,
   255, 128, 128,
   255, 255, 128,
   100, 255, 128
  },
  {255,   0,   0,  // Green to blue!
   240,   0,  64,
   216,   0, 128,
   128,   0, 216,
    64,   0, 240,
     0,   0, 255
  },
  {  0, 255,   3,  // Red to blue!
     0, 255,  10,
     0, 240,  25,
     0, 200,  80,
     0, 100, 150,
     0,  50, 255
  },
  {  3, 255,   0,  // Red to green!
    30, 255,   0,
    60, 240,   0,
   100, 180,   0,
   180, 180,   0,
   255,  20,   0
 }
};

// ---- LED Resistor preset GRB     0: Off     1: Brown        2: Red       3: Orange     4: Yellow      5: Green     6: Blue      7: Purple      8: Gray       9: White
static const uint8_t Time_Code_Colors[][3] =  {{0, 0, 0}, {128, 255, 64}, {0, 255, 0}, {30, 255, 0}, {125, 255, 0}, {255, 0, 0}, {0, 0, 255}, {0, 200, 255}, {40, 40, 60}, {255, 255, 255}};

static inline void _target_RGB(uint8_t *tp, uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
	tp[4 * index] = g;
	tp[4 * index + 1] = r;
	tp[4 * index + 2] = b;
	tp[4 * index + 3] = 0;
}
static inline void _target_RGBW(uint8_t *tp, uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	_target_RGB(tp, index, r, g, b);
	tp[4 * index + 3] = w;
}
static inline void _target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i = 0; i < num_rgb; ++i) _target_RGB(tp, i, r, g, b);
}
static inline void _target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint8_t i = 0; i < num_rgb; ++i) _target_RGBW(tp, i, r, g, b, w);
}

static inline void _minimize_difference(uint8_t *target_arr) {
	uint8_t dt = 0;
	for(uint8_t i = 0; i < num_bytes; i++) { // Sorry for the ll-access
		if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
		else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
		else ++dt;
	}
	if(dt != num_bytes) vfdco_clr_render();
}

/**
  * @brief  Implementation of virtual functions Light_Pattern::VTable (static void _Light_Pattern_F3)
**/
static inline vfdco_hid_action_status_t _Light_Pattern_F3(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3) return BUTTON_ACTION_NOT_PERFORMED;
 unsafe_self->VTable.F3(unsafe_self);
 return BUTTON_ACTION_PERFORMED;
}
static inline vfdco_hid_action_status_t _Light_Pattern_F3Var(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return BUTTON_ACTION_NOT_PERFORMED;
 unsafe_self->VTable.F3Var(unsafe_self);
 return BUTTON_ACTION_PERFORMED;
}
static inline void _Light_Pattern_Update(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
}
static inline void _Light_Pattern_Delete(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Delete(unsafe_self);
}
static inline void _Light_Pattern_Hello(struct Light_Pattern *unsafe_self) {
  if(!unsafe_self->VTable.Hello) return;
  unsafe_self->VTable.Hello();
}

/**
  * @brief  Constructor of Light_Pattern class
**/
void Light_Pattern_Init(struct Light_Pattern *self) {
  self->F3 = _Light_Pattern_F3;
  self->F3Var = _Light_Pattern_F3Var;
  self->Update = _Light_Pattern_Update;
  self->Hello = _Light_Pattern_Hello;
  self->Delete = _Light_Pattern_Delete;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Update (static void _Light_Pattern_Static_Update)
 **/
static void _Light_Pattern_Static_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  if(Time_Event_Update(&self->t)) {
    _minimize_difference(self->target_arr);
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3 (static void _Light_Pattern_Static_F3)
 **/
static void _Light_Pattern_Static_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;

  self->position++;
  if(self->position >= NUM_STATIC_T4) self->position = 0;

	#ifdef DEBUG
  printf("%d ", self->position);
	#endif

  if(self->position < NUM_STATIC_T1) {
    // Single Color Special
    switch(self->position) {
      case 0: _target_all_RGB   (self->target_arr,   0,   0,   0     ); break;
      case 1: _target_all_RGBW  (self->target_arr,   0,   0,  96, 255); break;
      case 2: _target_all_RGBW  (self->target_arr,   0,   0,   0, 255); break;
    }
  } else if(self->position < NUM_STATIC_T2) {
    // Single Color
    uint32_t target_color = _led_color_hsl2rgb(Static_Color_Hues[self->position - NUM_STATIC_T1], 255, 127);
    _target_all_RGB(self->target_arr, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);

  } // else if (self->position < NUM_STATIC_T3) {
    // Rainbows
    /*uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[0] + 2;
      i_h += i * 40; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, Static_Color_Rainbow_Saturation[t_pos], Static_Color_Rainbow_Lightness[t_pos]);
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }*/

  /*}*/ else { // < T4
    // Multicolor, just use legacy colors, they are hand crafted and look better
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < num_rgb; ++i) {
      _target_RGB(self->target_arr, i, Static_Color_Rainbows[t_pos][3 * i + 1], Static_Color_Rainbows[t_pos][3 * i], Static_Color_Rainbows[t_pos][3 * i + 2]);
    }
    /*
    uint8_t t_pos = self->position - NUM_STATIC_T3;

    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[t_pos] + 2;
      i_h -= i * 15; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, 255, 127);  // Get target RGB
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }
    */
  }

  char k[6] = {'C', 'H', 'A', 'N', 'G', 'E'};
  vfdco_display_render_message(k, 0, 200);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Hello (static void _Light_Pattern_Static_Hello)
 **/
static inline void _Light_Pattern_Static_Hello(void) {
  char k[6] = {'C', ' ', ' ', 'C', 'L', 'R'};
  vfdco_display_render_message(k, 0, MESSAGE_LONG);
}

static inline void _Light_Pattern_Static_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  free(self->target_arr);
  free(self);
}

/**
  * @brief  Constructor of Light_Pattern_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self) {
  Light_Pattern_Init(&self->super);
  self->t = Time_Event_Init(SINGLE_COLOR_FADE_SPEED);

  self->target_arr = (uint8_t *)calloc(num_bytes, sizeof(uint8_t));
  self->position = 0;

  struct Light_Pattern_VTable _static_vtable = {
    .F3 = _Light_Pattern_Static_F3,
    .F3Var = NULL,
    .Update = _Light_Pattern_Static_Update,
    .Hello = _Light_Pattern_Static_Hello,
		.Delete = _Light_Pattern_Static_Delete
  };
  self->super.VTable = _static_vtable;
}





/** Begin of:
  * @toc SECTION_LIGHT_PATTERN_SPECTRUM
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Spectrum::Update (static void _Light_Pattern_Spectrum_Update)
 **/
static void _Light_Pattern_Spectrum_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;

  self->spectrum_fader->Next(self->spectrum_fader);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3 (static void _Light_Pattern_Spectrum_F3)
**/
static void _Light_Pattern_Spectrum_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = self->color[0];

  // char message[MESSAGE_LENGTH];
  if(_color->l == LIGHTNESS_M) {
    _color->l = LIGHTNESS_H;
  }
  else if(_color->l == LIGHTNESS_H) {
    _color->l = LIGHTNESS_L;
  }
  else{
    _color->l = LIGHTNESS_M;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3Var (static void _Light_Pattern_Spectrum_F3Var)
**/
static void _Light_Pattern_Spectrum_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = self->color[0];

  // char message[MESSAGE_LENGTH];
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
  }
  else{
    _color->s = SATURATION_H;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::Hello (static void _Light_Pattern_Spectrum_Hello)
**/
static inline void _Light_Pattern_Spectrum_Hello(void) {
  char k[6] = {'C', ' ', 'F', 'A', 'D', 'E'};
  vfdco_display_render_message(k, 0, MESSAGE_LONG);
}

static inline void _Light_Pattern_Spectrum_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  free(self->color[0]);
  free(self->color);
  self->spectrum_fader->Delete(self->spectrum_fader);
  free(self);
}

/**
* @brief  Constructor of Light_Pattern_Spectrum class
**/
void Light_Pattern_Spectrum_Init(struct Light_Pattern_Spectrum *self) {
  Light_Pattern_Init(&self->super);

  self->color = (hsl_t **)calloc(1, sizeof(hsl_t *));
  self->color[0] = HSL_Init(0, SATURATION_H, LIGHTNESS_M);

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  self->spectrum_fader = (struct LED_Color *)LED_Color_Fader_Init(
    SPECTRUM_FADE_SPEED,         // Timer interval
    LED_COLOR_BLEND_MODE_NORMAL, // Pixel blend setting.
    0,                           // Pixel index to start
    LED_COLOR_REPEAT_FOREVER,    // Fade N cycles
    1,                           // Number of HSL colors
    self->color,                 // Array of HSL colors
    6,                           // Number of chained pixels
    0                            // Hue difference between chained pixels
  );

  // Skip fade in
  ((struct LED_Color_Fader *)self->spectrum_fader)->state = LED_COLOR_STATE_ACTIVE;

  struct Light_Pattern_VTable _spectrum_vtable = {
    .F3 = _Light_Pattern_Spectrum_F3,
    .F3Var = _Light_Pattern_Spectrum_F3Var,
    .Update = _Light_Pattern_Spectrum_Update,
    .Hello = _Light_Pattern_Spectrum_Hello,
    .Delete = _Light_Pattern_Spectrum_Delete
  };
  self->super.VTable = _spectrum_vtable;
}




/** Begin of:
* @toc SECTION_LIGHT_PATTERN_rainbow
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Update (static void _Light_Pattern_Rainbow_Update)
**/
static void _Light_Pattern_Rainbow_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;

  self->rainbow_fader->Next(self->rainbow_fader);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3 (static void _Light_Pattern_Rainbow_F3)
**/
static void _Light_Pattern_Rainbow_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;

  struct LED_Color_Fader *_f = (struct LED_Color_Fader *)self->rainbow_fader;

  // char message[MESSAGE_LENGTH];
  if(_f->chain_huediff == 10) {
    _f->chain_huediff = 21;
  }
  else if(_f->chain_huediff == 21) {
    _f->chain_huediff = 42;
  }
  else{
    _f->chain_huediff = 10;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3Var (static void _Light_Pattern_Rainbow_F3Var)
**/
static void _Light_Pattern_Rainbow_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  hsl_t *_color = self->color[0];

  // char message[MESSAGE_LENGTH];
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
  }
  else{
    _color->s = SATURATION_H;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Hello (static void _Light_Pattern_Rainbow_Hello)
**/
static inline void _Light_Pattern_Rainbow_Hello(void) {
  char k[6] = {'C', 'C', 'R', 'O', 'S', 'S'};
  vfdco_display_render_message(k, 0, MESSAGE_LONG);
}

static inline void _Light_Pattern_Rainbow_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  free(self->color[0]);
  free(self->color);
  self->rainbow_fader->Delete(self->rainbow_fader);
  free(self);
}

/**
* @brief  Constructor of Light_Pattern_Rainbow class
**/
void Light_Pattern_Rainbow_Init(struct Light_Pattern_Rainbow *self) {
  Light_Pattern_Init(&self->super);

  self->color = (hsl_t **)calloc(1, sizeof(hsl_t *));
  self->color[0] = HSL_Init(0, SATURATION_H, LIGHTNESS_M);

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  self->rainbow_fader = (struct LED_Color *)LED_Color_Fader_Init(
    SPECTRUM_FADE_SPEED,         // Timer interval
    LED_COLOR_BLEND_MODE_NORMAL, // Pixel blend setting.
    0,                           // Pixel index to start
    LED_COLOR_REPEAT_FOREVER,    // Fade N cycles
    1,                           // Number of HSL colors
    self->color,                 // Array of HSL colors
    6,                           // Number of chained pixels
    10                           // Hue difference between chained pixels
  );

  // Skip fade in
  ((struct LED_Color_Fader *)self->rainbow_fader)->state = LED_COLOR_STATE_ACTIVE;

  struct Light_Pattern_VTable _rainbow_vtable = {
    .F3 = _Light_Pattern_Rainbow_F3,
    .F3Var = _Light_Pattern_Rainbow_F3Var,
    .Update = _Light_Pattern_Rainbow_Update,
    .Hello = _Light_Pattern_Rainbow_Hello,
    .Delete = _Light_Pattern_Rainbow_Delete
  };
  self->super.VTable = _rainbow_vtable;
}







/** Begin of:
* @toc SECTION_LIGHT_PATTERN_CHASE
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Update (static void _Light_Pattern_Chase_Update)
**/
static void _Light_Pattern_Chase_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)self->chase_fader;
  if(_chaser->state != LED_COLOR_STATE_CYCLIC_RECOVERY) self->chase_fader->Next(self->chase_fader);

  // If second has changed
  if(self->flip_timer->s != self->flip_timer_previous_second) {
    self->flip_timer_previous_second = self->flip_timer->s;

    self->color->h += 27;

    // Reset state variables
    _chaser->pk_state = 0;
    _chaser->chase_pos = 0;

    // Flip direction at every end
    if(self->chase_mode == 2) {
      if(self->flip_timer->s & 0x01) {
        _chaser->start_pos = 0;
        _chaser->chase_mode = LED_COLOR_CHASER_MODE_LR_LINEAR;
      } else {
        _chaser->start_pos = num_rgb - 1;
        _chaser->chase_mode = LED_COLOR_CHASER_MODE_RL_LINEAR;
      }
    }
  }
 }

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3 (static void _Light_Pattern_Chase_F3)
* @function  Switches between L-R, R-L and second flip mode
**/
static void _Light_Pattern_Chase_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  if(self->chase_mode == 0) { // L->R to R->L
    self->chase_mode = 1;
    struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)self->chase_fader;
    _chaser->start_pos = num_rgb - 1;
    _chaser->chase_mode = LED_COLOR_CHASER_MODE_RL_LINEAR;

  } else if(self->chase_mode == 1) { // To flipper
    self->chase_mode = 2;

  } else {
    self->chase_mode = 0;
    struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)self->chase_fader;
    _chaser->start_pos = 0;
    _chaser->chase_mode = LED_COLOR_CHASER_MODE_LR_LINEAR;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3Var (static void _Light_Pattern_Chase_F3Var)
**/
static void _Light_Pattern_Chase_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  if(self->diff_color.h == 0) {
    self->diff_color.h = 6;
  } else {
    self->diff_color.h = 0;
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Hello (static void _Light_Pattern_Chase_Hello)
**/
static inline void _Light_Pattern_Chase_Hello(void) {
  char k[6] = {'C', ' ', 'C', 'H', 'F', 'D'};
  vfdco_display_render_message(k, 0, MESSAGE_LONG);
}

static inline void _Light_Pattern_Chase_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;
  free(self->color);
  self->chase_fader->Delete(self->chase_fader);
  free(self);
}

/**
* @brief  Constructor of Light_Pattern_Chase class
**/
void Light_Pattern_Chase_Init(struct Light_Pattern_Chase *self, vfdco_time_t *time, uint_fast8_t chase_mode) {
  Light_Pattern_Init(&self->super);

  self->color = HSL_Init(0, SATURATION_H, LIGHTNESS_M);
  hsl_d_t _hsld = {.h = 0, .s = 0, .l = 0};
  self->diff_color = _hsld;

  self->flip_timer = time;
  self->chase_mode = chase_mode;

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  self->chase_fader = (struct LED_Color *)LED_Color_Chaser_Init(
    2,
    LED_COLOR_BLEND_MODE_NORMAL,
    (chase_mode == 1) ? num_rgb - 1 : 0, // 1: start right, else start left (usual)
    LED_COLOR_REPEAT_FOREVER, // Repeat N times
    num_rgb,
    self->color,
    &self->diff_color,
    18,
    LED_COLOR_CHASER_NON_PRESERVING,
    (chase_mode == 1) ? LED_COLOR_CHASER_MODE_RL_LINEAR : LED_COLOR_CHASER_MODE_LR_LINEAR
  );

  struct Light_Pattern_VTable _chase_vtable = {
    .F3 = _Light_Pattern_Chase_F3,
    .F3Var = _Light_Pattern_Chase_F3Var,
    .Update = _Light_Pattern_Chase_Update,
    .Hello = _Light_Pattern_Chase_Hello,
    .Delete = _Light_Pattern_Chase_Delete
  };
  self->super.VTable = _chase_vtable;
}





/** Begin of:
  * @toc SECTION_LED_COLOR_RESISTOR
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Update (static void _LED_Color_Resistor_Update)
 **/
static void _Light_Pattern_Time_Code_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Time_Code *self = (struct Light_Pattern_Time_Code *)unsafe_self;

	if(Time_Event_Update(&self->clock)) {
		// Dereference const (read only) variables
	  const uint8_t h = self->time->h;
	  const uint8_t m = self->time->m;
	  const uint8_t s = self->time->s;
		uint8_t *target_arr = self->target_arr;

		uint8_t digit_values[num_rgb];
		digit_values[0] = s % 10;
		digit_values[1] = s / 10;
		digit_values[2] = m % 10;
		digit_values[3] = m / 10;
		digit_values[4] = h % 10;
		digit_values[5] = h / 10;

		for(uint8_t i = 0; i < num_rgb; i++)
			_target_RGB(target_arr, i, Time_Code_Colors[digit_values[i]][1], Time_Code_Colors[digit_values[i]][0], Time_Code_Colors[digit_values[i]][2]);

		_minimize_difference(target_arr);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Hello (static void _LED_Color_Resistor_Hello)
 **/
static inline void _Light_Pattern_Time_Code_Hello(void) {
	char k[6] = {'C', 'T', 'C', 'O', 'D', 'E'};
  vfdco_display_render_message(k, 0, MESSAGE_LONG);
}

static inline void _Light_Pattern_Time_Code_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Time_Code *self = (struct Light_Pattern_Time_Code *)unsafe_self;
  free(self->target_arr);
  free(self);
}

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void Light_Pattern_Time_Code_Init(struct Light_Pattern_Time_Code *self, vfdco_time_t *time_instance) {
  Light_Pattern_Init(&self->super);
	self->clock = Time_Event_Init(1);

	self->target_arr = (uint8_t *)calloc(num_bytes, sizeof(uint8_t));
  self->time = time_instance;

  struct Light_Pattern_VTable _time_code_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _Light_Pattern_Time_Code_Update,
    .Hello = _Light_Pattern_Time_Code_Hello,
		.Delete = _Light_Pattern_Time_Code_Delete
  };
  self->super.VTable = _time_code_vtable;
}


/** Begin of:
* @toc SECTION_LIGHT_PATTERN_COP
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Cop::Update (static void _Light_Pattern_Cop_Update)
**/
static void _Light_Pattern_Cop_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Cop *self = (struct Light_Pattern_Cop *)unsafe_self;

  if(Time_Event_Update(&self->clock)) {
    if(self->state < 13) self->state++;
    else if(self->state == 13) self->state = 0;

    // b | r fill
    if(self->state == 0) {
      for(uint_fast8_t i = 0;              i < (num_rgb >> 1); ++i) vfdco_clr_set_RGBW(i, 255,   0,   0, 0);
      for(uint_fast8_t i = (num_rgb >> 1); i < num_rgb;        ++i) vfdco_clr_set_RGBW(i,   0,   0, 255, 0);
    }
    // off fill
    else if(self->state == 5) vfdco_clr_set_all_RGBW(0, 0, 0, 0);
    // b | r fill
    else if(self->state == 6) {
      for(uint_fast8_t i = 0;              i < (num_rgb >> 1); ++i) vfdco_clr_set_RGBW(i, 255,   0,   0, 0);
      for(uint_fast8_t i = (num_rgb >> 1); i < num_rgb;        ++i) vfdco_clr_set_RGBW(i,   0,   0, 255, 0);
    }
    // r | b fill
    else if(self->state == 7) {
      for(uint_fast8_t i = 0;              i < (num_rgb >> 1); ++i) vfdco_clr_set_RGBW(i,   0,   0, 255, 0);
      for(uint_fast8_t i = (num_rgb >> 1); i < num_rgb;        ++i) vfdco_clr_set_RGBW(i, 255,   0,   0, 0);
    }
    // white fill
    else if(self->state == 12) vfdco_clr_set_all_RGBW(255, 255, 255, 0);

    // r | b fill
    else if(self->state == 13) {
      for(uint_fast8_t i = 0;              i < (num_rgb >> 1); ++i) vfdco_clr_set_RGBW(i,   0,   0, 255, 0);
      for(uint_fast8_t i = (num_rgb >> 1); i < num_rgb;        ++i) vfdco_clr_set_RGBW(i, 255,   0,   0, 0);
    }
    vfdco_clr_render();
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Cop::Hello (static void _Light_Pattern_Cop_Hello)
**/
static inline void _Light_Pattern_Cop_Hello(void) {
  char message[6] = {'C', ' ', ' ', 'C', 'O', 'P'};
  vfdco_display_render_message(message, 0, MESSAGE_LONG);
}

/**
* @brief  Implementation of virtual destructor Light_Pattern_Cop::~Light_Pattern_Cop (static void _Light_Pattern_Cop_Delete)
**/
static inline void _Light_Pattern_Cop_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Cop *self = (struct Light_Pattern_Cop *)unsafe_self;
  free(self);
}

/**
* @brief  Constructor of Light_Pattern_Cop class Light_Pattern_Cop::Light_Pattern_Cop (static void Light_Pattern_Cop_Init)
**/
void Light_Pattern_Cop_Init(struct Light_Pattern_Cop *self) {
  Light_Pattern_Init(&self->super);

  self->clock = Time_Event_Init(COP_FADE_SPEED);
  self->state = 0;

  struct Light_Pattern_VTable _vtable = {
    .F3      = NULL,
    .F3Var   = NULL,
    .Update  = _Light_Pattern_Cop_Update,
    .Hello   = _Light_Pattern_Cop_Hello,
    .Delete  = _Light_Pattern_Cop_Delete
  };
  self->super.VTable = _vtable;
}


// Hello World
