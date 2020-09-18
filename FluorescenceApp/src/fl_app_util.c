#include "../FluorescenceV3/Commons/vfdco_util.h"
#include <stdint.h>

/**
 * @tableofcontents SHARED_UTILITIES
 * @brief Shared functions
 */
volatile uint8_t _rrx = 0;
volatile uint8_t _rry = 0;
volatile uint8_t _rrz = 0;
volatile uint8_t _rra = 1;
/**
 * @brief <= 7 Bit Xorshift randomizer between [0 .. 2^(bits) - 1]]
 * @param bits Max. 7 bit!
 * @return uint8_t Random number
 */
uint8_t vfdco_util_random(uint8_t bits) {
  if(!bits) return 0;
  uint8_t _rrt = _rrx ^ (_rrx >> 1);
  _rrx = _rry;
  _rry = _rrz;
  _rrz = _rra;
  _rra = _rrz ^ _rrt ^ (_rrz >> 3) ^ (_rrt << 1);
  return _rra & ((1 << bits) - 1);
}
