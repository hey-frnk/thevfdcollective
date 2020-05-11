#include "../Commons/vfdco_mic.h"
#include "../Commons/vfdco_util.h"

#ifdef _MIC_IMPLEMENTATION
#error "An implementation of the microphone driver already exists!"
#endif
#define _MIC_IMPLEMENTATION

// Initializer
inline void vfdco_mic_init() {
    return;
}

// Read in level
uint8_t vfdco_mic_read_level(void) {
    return vfdco_util_random(3) % 7;
}
