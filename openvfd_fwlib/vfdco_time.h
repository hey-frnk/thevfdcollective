#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_IV11RENDER_H
#define _VFDCOLLECTIVE_IV11RENDER_H

#include <stdint.h>
#include "vfdco_time.h"

typedef struct vfdco_time {
  uint8_t hr, min, sec;
} vfdco_time_t;

typedef struct vfdco_date {
  uint8_t d, m;
  uint16_t yr;
} vfdco_date_t;

#endif

#ifdef __cplusplus
}
#endif
