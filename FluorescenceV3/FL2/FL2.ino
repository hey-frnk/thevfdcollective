/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 04/25/2020

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
  * @file     FL2.ino
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-March-2020
  * @brief    This is the main Arduino file of Fluorescence by The VFD Collective, V2.0/V2.1 HW
  ******************************************************************************
 **/

#include "vfdco_clock_routines.h"

void setup() {
  // Code is run at startup to initialize all clock parameters and drivers
  vfdco_clock_initializer(0);
}

void loop() {
  // Code is call repeatedly to update all clock parameters in designated routines
  // WoHOo hOw FuN iS tHhIis??
  vfdco_clock_routine();
}
