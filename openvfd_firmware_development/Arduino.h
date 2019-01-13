#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARDUINO_SIM_H
#define ARDUINO_SIM_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define PA 1
#define PB 2
#define PC 3
#define PD 4

#define TRUE 1
#define FALSE 0

#define NUM_DIGITS_V 6

#define _BV(bit) (1 << (bit))
#define digitalPinToBitMask(P)    (digital_pin_to_bit_mask_PGM[P])
#define digitalPinToPort(P)       (digital_pin_to_port_PGM[P])
#define PROGMEM
#define displayWrite(mode, mask, delay, msg) {printf("Messaging with: \n"); for(uint8_t i = 0; i < 6; ++i) printf("%c", msg[i]); printf("\n");}
#define pgm_read_byte_near(p) (*(p))
#define analogRead(p) (rand() % 256)

#define SHORTPRESS 1
#define LONGPRESS 2

const uint8_t digital_pin_to_bit_mask_PGM[] = {
        _BV(0), /* 0, port D */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(0), /* 8, port B */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(0), /* 14, port C */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
};

const uint8_t digital_pin_to_port_PGM[] = {
        PD, /* 0 */
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PB, /* 8 */
        PB,
        PB,
        PB,
        PB,
        PB,
        PC, /* 14 */
        PC,
        PC,
        PC,
        PC,
        PC,
};

#endif

#ifdef __cplusplus
}
#endif
