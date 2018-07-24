#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_LEDCOLOR_H
#define _VFDCOLLECTIVE_LEDCOLOR_H

#include <stdint.h>

// Virtual table of common render functions
struct vfdco_led_vtable {
  void                      (*vfdco_led_render)(void *self);
};

// LED objects. Every object has a FSM that will change its state after the interval has elapsed
typedef struct vfdco_led {
  uint16_t                  vfdco_led_fsm_interval;
  void                      (*vfdco_led_set_led_fsm_interval)(void *self, uint16_t interval);
  struct vfdco_led_vtable   vt;
} vfdco_led_t;





#endif

#ifdef __cplusplus
}
#endif
