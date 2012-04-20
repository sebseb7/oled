#include <stdio.h>


#include "main.h"
#include "circle.h"
#include "text.h"

uint8_t phase[3][24]={
//     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
	{ 34, 72,142, 85, 29,  0,138,218,255,150,  0,117,255,255,255,161,  0,  0,150,204,214, 97,  0,  0},//red
	{ 34, 31,  0,  0, 29,146,135,125, 67, 79,178,254,255,191, 58, 83,108,185,150, 92,  0,  0,  0, 75},//green
	{  0,  0, 24,162,255,  0,  0,  0, 67,255,226, 14,  0, 47,255,255,255,  0,  0,  0,  0,146,150,  0} //blue
//	{  0, 66,126, 92, 50, 55,154,242,255,179,  0,180,255,255,255,206,113,  0,162,178,230,130, 41, 31},//red
//	{  0, 27,  0, 50,147,147,151,144, 87,129,218,255,255,196,113, 67,113,208,162,104,  0,  0,  0, 95},//green
//	{  0,  0, 33,176,210, 37, 22,  0, 87,217,208, 13,  0, 63,203,255,255,  0,  0,  0,  0,188,246, 34} //blue
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

/*	printf("%03i %03i %03i\n",r,g,b);

	draw_number(50,62,hour,2,'0',255,255,255);
	draw_text(  62,62,":",255,255,255);
	draw_number(68,62,minute,2,'0',255,255,255);
*/
	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, 10, 450);
}


