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

#ifndef _VFDCOLLECTIVE_DISPLAYMODES_H
#define _VFDCOLLECTIVE_DISPLAYMODES_H

#include "vfdco_config.h"
#include "vfdco_time.h"

typedef enum VFDCO_DOTMODES {
  DOT_MODE_OFF        = 0,
  DOT_MODE_REGULAR    = 1,
  DOT_MODE_SECFLIP    = 2
} VFDCO_DOTMODES_t;

struct vfdco_displaymodes_vtable {
  void                (*vfdco_displaymodes_show)(void *self);
};

typedef struct vfdco_displaymodes {
  struct vfdco_displaymodes_vtable    *vt;
  void                                (*vfdco_displaymodes_show)(void *self);
  vfdco_time_t                        *t;
  VFDCO_DOTMODES_t                    dm;
} vfdco_displaymodes_t;

typedef struct vfdco_displaymodes_regular {
  vfdco_displaymodes_t                super;
} vfdco_displaymodes_regular_t;

typedef struct vfdco_displaymodes_dfade {
  vfdco_displaymodes_t                super;
} vfdco_displaymodes_dfade_t;

void vfdco_displaymodes_init(vfdco_displaymodes_t *self, vfdco_time_t *t);
void vfdco_displaymodes_regular_init(vfdco_displaymodes_regular_t *self, vfdco_time_t *t);
void vfdco_displaymodes_dfade_init(vfdco_displaymodes_dfade_t *self, vfdco_time_t *t);

#endif

#ifdef __cplusplus
}
#endif
