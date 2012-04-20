#include <stdio.h>


#include "main.h"
#include "circle.h"
#include "text.h"

uint8_t phase[3][24]={
//     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
	{  0, 34, 95, 98, 50,  0,131,210,255,221,210,160,255,255,255,121,163,  0,131,152,172, 98,  0,  0},//red
	{  0, 20,  0,  0, 50,124,131,125,132,171,210,255,255,229,199,171,163,173,131, 91,  0,  0,  0, 26},//green
	{  0,  0,  0,164,255,  0,  0,  0,132,255,255,160,  0,184,199,255,255,  0,  0,  0,  0,164,170,  0} //blue
};


uint8_t minute = 0;
uint8_t hour = 0;


static uint8_t tick(void) {

	minute++;
	if(minute == 60)
	{
		minute=0;
		hour++;
	}
	if(hour==24)
		hour=0;

	uint8_t next_hour = hour+1;
	if(next_hour == 24)
		next_hour=0;

	uint8_t r = (phase[0][hour]*(60-minute)/60.0)+(phase[0][next_hour]*(minute)/60.0);
	uint8_t g = (phase[1][hour]*(60-minute)/60.0)+(phase[1][next_hour]*(minute)/60.0);
	uint8_t b = (phase[2][hour]*(60-minute)/60.0)+(phase[2][next_hour]*(minute)/60.0);

	
	draw_filledCircle(64,64,70,0,0,0);
	draw_filledCircle(64,64,62,r,g,b);

	printf("%03i %03i %03i\n",r,g,b);

	draw_number(50,58,hour,2,'0',255,255,255);
	draw_text(  62,58,":",255,255,255);
	draw_number(68,58,minute,2,'0',255,255,255);

	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, 10, 450);
}


