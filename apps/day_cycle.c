#include <stdio.h>


#include "main.h"
#include "circle.h"
#include "text.h"

uint8_t phase[3][24]={
//     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
	{100,  0,100,100,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,100, 10, 10, 10, 10},//red
	{  0,  0,  0,  0,100,100,100,100,100,  0,  0,  0,  0,  0,  0,255,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,100,150,200,250,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue
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

	draw_number(50,62,hour,2,'0',255,255,255);
	draw_text(  62,62,":",255,255,255);
	draw_number(68,62,minute,2,'0',255,255,255);

	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, 10, 450);
}


