#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "wuline.h";
#include "circle.h";

uint8_t randr(uint8_t start,uint8_t end);

static uint8_t tick(void) {

	draw_filledCircle(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255));

	draw_line(randr(0,128),randr(0,128),randr(0,128),randr(0,128),randr(0,255),randr(0,255),randr(0,255));
	return 0;
}

static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, 1, 450);
}

uint8_t randr(uint8_t start,uint8_t end)
{
	return start + rand() % ( end - start );
}


