#ifndef LED_COLOR_HPP
#define LED_COLOR_HPP

#include <stdint.h>
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#define    PORT        (PORTB)          // Digital pin's port
#define    PORT_PIN    (PORTB5)         // Digital pin's bit position
#define    NUM_BITS    (8)              // Const 8
#endif

class LED_Color {
private:
  #ifndef DEBUG
    uint32_t      t_f;
  #endif
public:
  uint8_t       NUM_RGB;
  uint8_t       NUM_BYTES;
  uint8_t       *rgb_arr;
  uint8_t       *target_arr;
  LED_Color(uint8_t num_pixel, uint8_t bpp);
  ~LED_Color();
  void          render();
};

class LED_Color_Mode {
protected:
  LED_Color             *l;
  uint8_t               NUM_RGB;
  uint8_t               NUM_BYTES;
  uint8_t               *rgb_arr;
  uint8_t               *target_arr;
  void                  ledSmoothWrite();
  void                  ledDirectWrite(uint8_t *ledTarget);
public:
  LED_Color_Mode(LED_Color *l);
  virtual ~LED_Color_Mode();
  virtual void F3();
  virtual void F3var();
  virtual void loop() = 0;
  virtual void hello() = 0;
};

#endif
