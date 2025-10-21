#include "../../Commons/vfdco_audio.h"
#include "../../Commons/vfdco_util.h"

#ifdef _MIC_IMPLEMENTATION
#error "An implementation of the microphone driver already exists!"
#endif
#define _MIC_IMPLEMENTATION

static uint32_t tone_end_time_ms = 0;
static uint_fast8_t speaker_active = 0;
static uint16_t current_frequency = 0;

// Initializer
inline void vfdco_audio_init() {
    return;
}

// Read in level
uint8_t vfdco_audio_mic_read_level(void) {
    return vfdco_util_random(3) % 7;
}


void vfdco_audio_speaker_play(uint16_t frequency_hz) {
  if(frequency_hz == 0) return;
  current_frequency = frequency_hz;
  speaker_active = 1;

  // TODO: Enable PWM output on speaker pin
}

void vfdco_audio_speaker_stop(void) {
  speaker_active = 0;
  // TODO: Disable PWM or pull pin low
}

uint_fast8_t vfdco_audio_speaker_is_active(void) {
  return speaker_active;
}