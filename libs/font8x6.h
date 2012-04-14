#ifndef _FONT8X6_H
#define _FONT8X6_H

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
typedef unsigned char prog_uint8_t;
#endif

extern prog_uint8_t font8x6[][6];

#endif
