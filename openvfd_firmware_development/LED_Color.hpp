#ifndef LED_COLOR_HPP
#define LED_COLOR_HPP

#include <stdint.h>

class LED_Color {
private:
  uint8_t       PORT;
  uint8_t       PORT_PIN;
  #ifndef DEBUG
    uint32_t      t_f;
  #endif
public:
  uint8_t       NUM_RGB;
  uint8_t       NUM_BYTES;
  uint8_t       *rgb_arr;
  uint8_t       *target_arr;
  LED_Color(uint8_t num_pixel, uint8_t bpp, uint8_t pin);
  ~LED_Color();
  void          render();
};

class LED_Color_Mode {
private:
  LED_Color             *l;
protected:
  uint8_t               NUM_RGB;
  uint8_t               NUM_BYTES;
  uint8_t               *rgb_arr;
  uint8_t               *target_arr;
  void                  ledSmoothWrite();
  void                  ledDirectWrite(uint8_t *ledTarget);
public:
  LED_Color_Mode(LED_Color *l);
  virtual void F2();
  virtual void F2var();
  virtual void F3();
  virtual void F3var();
  virtual ~LED_Color_Mode();
  virtual void loop() = 0;
};

#endif
