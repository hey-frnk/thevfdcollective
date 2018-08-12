/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_LEDCOLOR_H
#define _VFDCOLLECTIVE_LEDCOLOR_H

#define NUM_COLORS 3 // R G B W
#define NUM_PIXELS 6 // 6 LEDs

#include <stdint.h>

// Virtual table of common render functions
struct vfdco_led_vtable {
  void                      (*vfdco_led_render)(void *self);
  void                      (*vfdco_led_update_fsm)(void *self);
  void                      (*vfdco_led_set_fsm)(void *self, uint16_t state);
};

// LED objects. Every object has a FSM that will change its state after the interval has elapsed
typedef struct vfdco_led {
  uint16_t                  fsminterval, fsmstate, maxstate;
  uint8_t                   *vfdco_led_target, *vfdco_led_current, ledarrsize, numclr, numpxl;
  void                      (*vfdco_led_render)(void *self),
                            (*vfdco_led_update_fsm)(void *self),
                            (*vfdco_led_set_fsm)(void *self, uint16_t state);

                            (*vfdco_led_smoothWrite)(void *self),
                            (*vfdco_led_set_led_fsm_interval)(struct vfdco_led *self, uint16_t interval);

  struct vfdco_led_vtable   *vt;
} vfdco_led_t;

// Static Color
typedef struct vfdco_led_staticolor {
  vfdco_led_t               super;
} vfdco_led_staticolor_t;

// Spectrum fade
typedef struct vfdco_led_spectrumfade {
  vfdco_led_t               super;
  uint8_t                   luminance, saturation;
  void                      (*vfdco_led_spectrumfade_setLuminance)(struct vfdco_led_spectrumfade *self, uint8_t luminance),
                            (*vfdco_led_spectrumfade_setSaturation)(struct vfdco_led_spectrumfade *self, uint8_t saturation);
} vfdco_led_spectrumfade_t;


void vfdco_led_init(vfdco_led_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl);
void vfdco_led_free(vfdco_led_t *self);

void vfdco_led_staticolor_init(vfdco_led_staticolor_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl);
void vfdco_led_staticolor_free(vfdco_led_staticolor_t *self);

void vfdco_led_spectrumfade_init(vfdco_led_spectrumfade_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl);
void vfdco_led_spectrumfade_free(vfdco_led_spectrumfade_t *self);


#endif

#ifdef __cplusplus
}
#endif
