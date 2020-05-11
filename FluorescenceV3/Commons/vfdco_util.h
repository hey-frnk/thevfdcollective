#include <stdint.h>

/**
 * @brief <= 7 Bit Xorshift randomizer between [0 .. 2^(bits) - 1]]
 * @param bits Max. 7 bit!
 * @return uint8_t Random number
 */
uint8_t vfdco_util_random(uint8_t bits);

