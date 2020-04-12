#include <Arduino.h>
#include "vfdco_mic.h"

#ifdef _MIC_IMPLEMENTATION
#error "An implementation of the microphone driver already exists!"
#endif
#define _MIC_IMPLEMENTATION

#define    MIC_PIN    A0    // ATMEGA: 23   Microphone Input Pin, MIN

const uint8_t LED20_samplesCount = 8;                     // Empirical sample acquisition array length
uint16_t LED20_samples[LED20_samplesCount] = {0};         // Empirical sample acquisition array allocation
uint8_t LED20_sampleIndex = 0;                            // Buffer push sample index
const uint8_t LED20_analogSamples = 128;                  // How many samples per reading
const uint8_t LED20_sense = 2;                            // Noise threshold
const float LED20_linFactor = -0.50;                      // VU meter log mapping adjust: linFactor in [-1 ... inf], -1 <= lF < 0: log, 0: lin, > 0: exp

// Initializer
void vfdco_mic_init() {
    analogReference(DEFAULT);
    pinMode(MIC_PIN, INPUT);
}

inline uint16_t LED20_mapUnsigned16(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  if(in_max - in_min == 0) return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Dynamically adjust sensitivity according to amplitude (sort-of-AGC)
uint16_t LED20_lLevelArr_vPush(uint16_t amplitudeIn) {
    // Queue push in
    if(LED20_sampleIndex < LED20_samplesCount) LED20_samples[LED20_sampleIndex++] = amplitudeIn;
    else{
        LED20_samples[LED20_sampleIndex = 0] = amplitudeIn;
        LED20_sampleIndex++;
    }

    // Control loop of maintaining healthy amplitude
    uint16_t lMin = 1023;
    uint16_t lMax = 0;
    for(uint8_t i = 0; i < LED20_samplesCount; i++) {
        if(LED20_samples[i] < lMin) lMin = LED20_samples[i];
        if(LED20_samples[i] > lMax) lMax = LED20_samples[i];
    }

    if(lMax - lMin < LED20_sense) return 0;
    uint16_t amplitudeAmplified = LED20_mapUnsigned16(amplitudeIn, lMin, lMax, 0, 255);
    return amplitudeAmplified;
}

// Read in level
uint8_t vfdco_mic_read_level(void) {
    uint16_t dMicRead = 512;
    uint16_t dPrevSample = 512;

    // Obtain amplitude
    uint16_t dMicMax = 0;
    uint16_t dMicMin = 1023;
    for(uint8_t i = 0; i < LED20_analogSamples; i++) {
        dPrevSample = dMicRead;
        dMicRead = analogRead(MIC_PIN);
        // Digital low pass filtering
        dMicRead = (dMicRead >> 2) + (dPrevSample >> 1) + (dPrevSample >> 2);

        // Get minimum and maximum
        if(dMicRead > dMicMax) dMicMax = dMicRead;
        else if(dMicRead < dMicMin) dMicMin = dMicRead;
    }

    float dMicA = (float)(LED20_lLevelArr_vPush(dMicMax - dMicMin));
    // Serial.println(dMicA);
    uint8_t u = dMicA ? (uint8_t)(roundf(powf(dMicA / 255.0, LED20_linFactor) * dMicA)) : 0;
    return (uint8_t)(roundf(u / 42.5));
}