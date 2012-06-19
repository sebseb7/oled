#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define LED_WIDTH	128
#define LED_HEIGHT	128

// multiple of 15
#define ZOOM 5

#define SIMULATOR

#define ATTRIBUTES	__attribute__((constructor));

typedef uint8_t (*tick_fun)(void);
typedef uint8_t (*key_fun)(uint8_t);

int sdlpause;

void registerAnimation(tick_fun tick,key_fun key, uint16_t t, uint16_t duration);
void registerApp(tick_fun tick,uint16_t t);
void setLedXY(uint8_t x,uint8_t  y, uint8_t r,uint8_t g,uint8_t b);

#endif

