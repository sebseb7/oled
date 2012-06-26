#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <time.h>

#include "wuline.h"
#include "circle.h"
#include "text.h"


uint8_t a = 0;
int8_t b = 1;



static uint8_t tick(void) {

	draw_filledCircle(64,64,70,0,0,0);
	draw_filledCircle(64,64,62,a,255-a,0);

	a+=b;
	if(a==255)
		b = -1;
	if(a==0)
		b = 1;

	return 0;
}


static void key(uint8_t key) {

}

static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, key,10, 450);
}


