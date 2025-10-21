/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 05/21/2025

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**
  ******************************************************************************
  * @file     vfdco_sensors.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     21-05-2025
  * @brief    TODO
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_SENSORS_UNIT_DEFINITIONS
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VFDCO_SENSORS_H
#define VFDCO_SENSORS_H

#include <stdint.h>

/** Begin of:
 * @tableofcontents SECTION_SENSORS_UNIT_DEFINITIONS
**/
// Temperature enum: sensors_temperature_unit_t
typedef enum {
  TEMPERATURE_UNIT_CELSIUS = 0,       // European
  TEMPERATURE_UNIT_FAHRENHEIT = 1,    // American
} sensors_temperature_unit_t;

// Pressure enum: sensors_pressure_unit_t
typedef enum {
  PRESSURE_UNIT_HPA = 0,              // European
  PRESSURE_UNIT_INHG = 1,             // American
} sensors_pressure_unit_t;

/** Begin of:
 * @tableofcontents SECTION_SENSORS_TYPE
 * @brief Classes temperature_t, humidity_t, pressure_t, tilt_t
**/
typedef struct {
    int16_t celsius_x10;     // e.g. 235 = 23.5 °C
    int16_t fahrenheit_x10;  // e.g. 745 = 74.5 °F
} vfdco_temperature_t;

typedef struct {
    uint8_t humidity_pct;    // 0–100 %RH, 1% resolution
} vfdco_humidity_t;

typedef struct {
    uint16_t hpa_x10;        // e.g. 10132 = 1013.2 hPa
    uint16_t inhg_x100;      // e.g. 2992 = 29.92 inHg
} vfdco_pressure_t;

typedef struct {
    int8_t pitch_deg;   // e.g. -2° pitch (forward/back)
    int8_t roll_deg;    // e.g. +5° tilt (left/right)
} vfdco_tilt_t;

void vfdco_sensors_get_temperature(vfdco_temperature_t *temp);
void vfdco_sensors_get_humidity(vfdco_humidity_t *hum);
void vfdco_sensors_get_pressure(vfdco_pressure_t *pres);
void vfdco_sensors_get_tilt(vfdco_tilt_t *tilt);

// It is assumed that each sensor is separate even if some of them are combined like BME280 for temperature, humidity and pressure
void vfdco_sensors_init_temperature();
void vfdco_sensors_init_humidity();
void vfdco_sensors_init_pressure();
void vfdco_sensors_init_tilt();

#endif // VFDCO_SENSORS_H

#ifdef __cplusplus
}
#endif
