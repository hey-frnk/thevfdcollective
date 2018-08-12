/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 * TABLE OF CONTENTS. Just type in this to navigate
 * - @search LED_METHODS
 * - @search STATIC_METHODS
 * - @search SPECTRUM_METHODS
 * - @search LED_CONSTRUCTORS
 */

#include <stdlib.h>
#include "vfdco_ledcolor.h"
#include "vfdco_color.h"

/**
  * @brief  Parent class polymorphic functions
  * @search LED_METHODS
  */
static void _vfdco_led_render(void *self) { ((vfdco_led_t *)self)->vt.vfdco_led_render(self); }
static void _vfdco_led_update_fsm(void *self) { tptr->vt.vfdco_led_update_fsm(self); }
static void _vfdco_led_set_fsm(void *self, uint16_t state) { tptr->vt.vfdco_led_set_fsm(self, state); }
static void _vfdco_led_set_led_fsm_interval(vfdco_led_t *self, uint16_t interval) { self->fsminterval = interval; }

static void _vfdco_led_smoothWrite(vfdco_led_t *self) {
  for(uint8_t i = 0; i < self->ledarrsize; i++){
    if(self->vfdco_led_current[i] < self->vfdco_led_target[i]) self->vfdco_led_current[i]++;
    else if(self->vfdco_led_current[i] > self->vfdco_led_target[i]) self->vfdco_led_current[i]--;
  }
}


/**
  * @brief  Static color methods. class led_staticolor : public led
  * @search STATIC_METHODS
  */
void _vfdco_led_staticolor_render(void *self) {
  vfdco_led_staticolor_t *tptr = (vfdco_led_staticolor_t *)self;

  // at every render, just smooth write
  tptr->super.vfdco_led_smoothWrite(tptr->super);

  // TODO: Output!!
}

void _vfdco_led_staticolor_update_fsm(void *self) {
  vfdco_led_color_t *tptr = ((vfdco_led_staticolor_t *)self)->super;
  ++tptr->fsmstate;
  if(tptr->fsmstate == tptr->maxstate + 1) tptr->fsmstate = 0;
  // Multi color?
  if(tptr->fsmstate > VFDCO_COLOR_NUM_SSCLR_OFFSET) {
    for(uint8_t i = 0; i < tptr->ledarrsize; i++) tptr->vfdco_led_target[i] = vfdco_color_msclr[fsmstate - VFDCO_COLOR_NUM_SSCLR][i];
  } else { // Else save some RAM
    for(uint8_t i = 0; i < tptr->ledarrsize; i += 3) {
      tptr->vfdco_led_target[i] = vfdco_color_ssclr[fsmstate][0];
      tptr->vfdco_led_target[i + 1] = vfdco_color_ssclr[fsmstate][1];
      tptr->vfdco_led_target[i + 2] = vfdco_color_ssclr[fsmstate][2];
    }
  }
}

void _vfdco_led_staticolor_set_fsm(void *self, uint16_t state) {}


/**
  * @brief  Spectrum fade methods. class led_spectrumfade : public led
  * @search SPECTRUM_METHODS
  */
void _vfdco_led_spectrumfade_render(void *self) {
  vfdco_led_spectrumfade_t *tptr = (vfdco_led_spectrumfade_t *)self;

  // TODO: Output!!
}

void _vfdco_led_spectrumfade_update_fsm(void *self) {
  vfdco_led_spectrumfade_t *iptr = (vfdco_led_spectrumfade_t *)self;
  vfdco_led_color_t *tptr = iptr->super;

  ++tptr->fsmstate;
  if(tptr->fsmstate == tptr->maxstate + 1) tptr->fsmstate = 0;

  uint32_t phase = vfdco_color_hsl2rgb((uint8_t)tptr->fsmstate, iptr->saturation, iptr->luminance);
  for(uint8_t i = 0; i < tptr->ledarrsize; i += 3){
    tptr->vfdco_led_target[i] = (uint8_t)((phase >> 16) & 0xFF);     // G
    tptr->vfdco_led_target[i + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    tptr->vfdco_led_target[i + 2] = (uint8_t)(phase & 0xFF);         // B
  }
}

void _vfdco_led_spectrumfade_set_fsm(void *self, uint16_t state) {}
void _vfdco_led_spectrumfade_setLuminance(vfdco_led_spectrumfade_t *self, uint8_t luminance) { self->luminance = luminance; }
void _vfdco_led_spectrumfade_setSaturation(vfdco_led_spectrumfade_t *self, uint8_t saturation) {self->saturation = saturation; }


/**
  * @brief  Virtual tables and constructor/destructors
  * @search LED_CONSTRUCTORS
  */
struct vfdco_led_vtable staticolor_vt = {&_vfdco_led_staticolor_render, &_vfdco_led_staticolor_update_fsm, &_vfdco_led_staticolor_set_fsm};
struct vfdco_led_vtable spectrumfade_vt = {&_vfdco_led_spectrumfade_render, &_vfdco_led_spectrumfade_update_fsm, &_vfdco_led_spectrumfade_set_fsm};

void vfdco_led_init(vfdco_led_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl) {
  self->numclr = numclr;
  self->numpxl = numpxl;
  self->ledarrsize = numclr * numpxl;
  self->fsmstate = 0;
  self->maxstate = maxstate;
  self->fsminterval = interval;

  self->vfdco_led_render = _vfdco_led_render;
  self->vfdco_led_update_fsm = _vfdco_led_update_fsm;
  self->vfdco_led_set_fsm = vfdco_led_set_fsm;

  self->vfdco_led_set_led_fsminterval = _vfdco_led_set_led_fsm_interval;
  self->vfdco_led_smoothWrite = _vfdco_led_smoothWrite;

  self->vfdco_led_target = calloc(ledArrSize, sizeof(uint8_t));
  self->vfdco_led_current = calloc(ledArrSize, sizeof(uint8_t));
}

void vfdco_led_staticolor_init(vfdco_led_staticolor_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl) {
  // super.init(...)
  vfdco_led_init(self->super, interval, maxstate, numclr, numpxl);
  self->super.vt = &staticolor_vt;
}

void vfdco_led_spectrumfade_init(vfdco_led_spectrumfade_t *self, uint16_t interval, uint16_t maxstate, uint8_t numclr, uint8_t numpxl) {
  // super.init(...)
  vfdco_led_init(self->super, interval, maxstate, numclr, numpxl);
  self->super.vt = &spectrumfade_vt;
  self->vfdco_led_spectrumfade_setLuminance = _vfdco_led_spectrumfade_setLuminance;
  self->vfdco_led_spectrumfade_setSaturation = _vfdco_led_spectrumfade_setSaturation;
}


void vfdco_led_free(vfdco_led_t *self) {
  free(self->vfdco_led_target);
  free(self->vfdco_led_current);
}

void vfdco_led_staticolor_free(vfdco_led_staticolor_t *self) { vfdco_led_free(self->super); }
void vfdco_led_spectrumfade_free(vfdco_led_spectrumfade_t *self) { vfdco_led_free(self->super); }

// ***
