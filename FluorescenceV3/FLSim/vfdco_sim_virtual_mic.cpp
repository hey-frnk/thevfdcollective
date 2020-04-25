#include "../Commons/vfdco_mic.h"

#ifdef _MIC_IMPLEMENTATION
#error "An implementation of the microphone driver already exists!"
#endif
#define _MIC_IMPLEMENTATION

// Initializer
void vfdco_mic_init() {
    return;
}

uint8_t _vfdco_mic_random06() {
    return vfdco_util_random(3) % 7;
}

// Read in level
uint8_t vfdco_mic_read_level(void) {
    uint8_t y0 = _vfdco_mic_random06();
    // uint8_t y1 = ((y0 << 1) + y0 + _vfdco_mic_random06()) >> 2;
    // uint8_t y2 = ((y1 << 1) + y1 + _vfdco_mic_random06()) >> 2;
    return _vfdco_mic_random06() > 4 ? y0 : 0;
}
