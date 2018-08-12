/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 * TABLE OF CONTENTS. Just type in this to navigate
 * - @search
 */

#include "vfdco_displaymodes.h"
#include "vfdco_render.h"

static void _vfdco_displaymodes_show(void *self) { ((vfdco_displaymodes_t *)self)->vt.vfdco_displaymodes_show(self); }

void _vfdco_displaymodes_regular_show(void *self) {
  vfdco_displaymodes_t *tptr = ((vfdco_displaymodes_regular_t *)self)->super;
  vfdco_render_time(tptr->t, 0x00);
}

struct vfdco_displaymodes_vtable _regular_vt = {&};

void vfdco_displaymodes_init(vfdco_displaymodes_t *self, vfdco_time_t *t) {
  self->vfdco_displaymodes_show = _vfdco_displaymodes_show;
  self->t = t;
}

void vfdco_displaymodes_regular_init(vfdco_displaymodes_regular_t *self, vfdco_time_t *t) {
  vfdco_displaymodes_init(self->super, t);
  self->super.vt = &_regular_vt;
}
