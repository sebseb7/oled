#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "wuline.h"
#include "circle.h"
#include "text.h"

uint16_t randr(uint16_t start,uint16_t end);

static uint8_t tick(void) {

	draw_filledCircle(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255));
	//draw_filledCircleSlice(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255),randr(0,360),randr(0,360));
	draw_line(randr(0,128),randr(0,128),randr(0,128),randr(0,128),randr(0,255),randr(0,255),randr(0,255));
	draw_text(randr(0,128),randr(0,128),"hallo",randr(0,255),randr(0,255),randr(0,255));


	return 0;
}

static void key(uint8_t key) {}

static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick,key, 1, 450);
}

uint16_t randr(uint16_t start,uint16_t end)
{
	return start + rand() % ( end - start );
}


