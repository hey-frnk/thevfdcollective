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

static const uint8_t Static_Color_Rainbow_Lightness[NUM_STATIC_COLOR_RAINBOWS] = {
  127,
  170
};
static const uint8_t Static_Color_Rainbow_Saturation[NUM_STATIC_COLOR_RAINBOWS] = {
  255,
  196
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
static inline void _Light_Pattern_F3(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3) return;
 unsafe_self->VTable.F3(unsafe_self);
}
static inline void _Light_Pattern_F3Var(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return;
 unsafe_self->VTable.F3Var(unsafe_self);
}
static inline void _Light_Pattern_Update(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
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

  } else if (self->position < NUM_STATIC_T3) {
    // Rainbows
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[0] + 2;
      i_h += i * 40; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, Static_Color_Rainbow_Saturation[t_pos], Static_Color_Rainbow_Lightness[t_pos]);
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }

  } else { // < T4
    // Multicolor
    uint8_t t_pos = self->position - NUM_STATIC_T3;

    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[t_pos] + 2;
      i_h -= i * 15; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, 255, 127);  // Get target RGB
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }
  }

  char k[6] = {'C', 'H', 'A', 'N', 'G', 'E'};
  vfdco_display_render_message(k, 0, 1000);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Hello (static void _Light_Pattern_Static_Hello)
 **/
static inline void _Light_Pattern_Static_Hello(void) {
  char k[6] = {'S', 'T', 'A', 'T', 'I', 'C'};
  vfdco_display_render_message(k, 0, 1000);
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
	char k[6] = {'T', 'I', 'C', 'O', 'D', 'E'};
  vfdco_display_render_message(k, 0, 1000);
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


// Hello World
