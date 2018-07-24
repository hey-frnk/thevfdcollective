#include "vfdco_ledcolor.h"

static void _vfdco_led_render(void *self) {
  ((vfdco_led_t *)self)->vt.vfdco_led_render(self);
}
