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

#ifndef _VFDCOLLECTIVE_COLOR_H
#define _VFDCOLLECTIVE_COLOR_H

#include "vfdco_config.h"
#include "vfdco_ledcolor.h"

#define   VFDCO_COLOR_NUM_SSCLR         10
#define   VFDCO_COLOR_NUM_SSCLR_OFFSET  (VFDCO_COLOR_NUM_SSCLR - 1)
#define   VFDCO_COLOR_NUM_MSCLR         5

static uint8_t const vfdco_color_ssclr[VFDCO_COLOR_SSCLR_OFFSET][3] =  { // Single static colors
                                     {  0,   0,   0}, // Off! ("Off")
                                     {255, 255, 255}, // White ("On")
                                     {  0, 255,   0}, // Red ("Red")
                                     {255,   0,   0}, // Green ("GRN := Green")
                                     {  0,   0, 255}, // Blue ("Blue")
                                     {125, 255,   0}, // Yellow ("YELO = Yellow")
                                     { 30, 255,   0}, // Orange ("ORNG = Orange")
                                     {255,   0, 128}, // Cyan ("Cyan")
                                     {  0, 255, 170}, // Magenta ("PRED := Purple Red")
                                     {  0, 200, 255}};// Purple ("PRPL := Purple")

static uint8_t const vfdco_color_msclr[][PIXEL_ARRAY_SIZE] = {
                                    {  0, 200, 255,  // Rainbow colors!
                                       0,   0, 255,
                                     255,   0,   0,
                                     128, 255,   0,
                                      30, 255,   0,
                                       0, 255,   0},

                                    {128, 255, 255,  // Pastel rainbow!
                                     128,  50, 255,
                                     255,   0, 128,
                                     255, 128, 128,
                                     255, 255, 128,
                                     100, 255, 128},

                                    {255,   0,   0,  // Green to blue!
                                     240,   0,  64,
                                     216,   0, 128,
                                     128,   0, 216,
                                      64,   0, 240,
                                       0,   0, 255},

                                    {  0, 255,   3,  // Red to blue!
                                       0, 255,  10,
                                       0, 240,  25,
                                       0, 200,  80,
                                       0, 100, 150,
                                       0,  50, 255},

                                    {  3, 255,   0,  // Red to green!
                                      30, 255,   0,
                                      60, 240,   0,
                                     100, 180,   0,
                                     180, 180,   0,
                                     255,  20,   0}
                                     };

// Color wheel color transform
uint32_t vfdco_color_hsl2rgb(uint8_t hue, uint8_t sat, uint8_t lum);

#endif

#ifdef __cplusplus
}
#endif
