#include <stdio.h>
 

#include "main.h"
#include "circle.h"
#include "text.h"

#define HOURS 24
#define SEGMENTS_PER_HOUR 2

uint8_t phase[3][HOURS*SEGMENTS_PER_HOUR]={
//     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
//	{  0,255,  0,  0,  0,  0,  0,218,255,150,  0,117,255,255,255,161,  0,  0,150,204,214, 97,  0,  0},//red
//	{  0,  0,  0,255,  0,  0,  0,125, 67, 79,178,254,255,191, 58, 83,108,185,150, 92,  0,  0,  0, 75},//green
//	{  0,  0,  0,  0,  0,255,  0,  0, 67,255,226, 14,  0, 47,255,255,255,  0,  0,  0,  0,146,150,  0} //blue


//    { 14, 55, 95, 85,  0,  0, 90,218,255,150,  0,117,255,255,255,161,  0,  0,150,204,214, 97,  0,  0},//red
//    { 14, 21,  0,  0,  0,135, 90,125, 67, 79,178,254,255,191, 58, 83,108,185,150, 92,  0,  0,  0, 75},//green
//    {  0,  0,  0,  0,  0,135, 90,125, 67, 79,178,254,255,191, 58, 83,108,185,150, 92,  0,  0,  0, 75} //blue
	
	
	
//       { 55, 55,75, 75,  0,  0,105,175,155,105,125, 65,215,255,255,175,  0,  0,125,155,175, 95,  0,  0},//red
//       { 75, 55,  0,  0,  0,175,105,105,  0,  0,125,255,215,175,125, 95,  0,135,125,105,  0,  0,  0, 75},//green
//       {  0,  0,  0, 95,175,  0,  0,  0,  0,110,255, 65,  0,100,125,255,175,  0,  0,  0,  0,125, 75,  0} //blue
 	            
	

//	{ 34, 72,116, 92, 71,  0,  0,136,213,255,150,  0,117,255,255,206,113,  0,162,178,230,130, 41, 31},//red
//	{ 34, 31,  0, 50,  0,  0,128,133,113, 67, 79,178,254,255,113, 67,113,208,162,104,  0,  0,  0, 95},//green
//	{  0,  0, 19,176, 29,200,  0,  0,  0, 67,255,226, 14, 63,203,255,255,  0,  0,  0,  0,188,190, 34} //blue
	
//     0       1       2       3       4       5       6       7       8       9      10      11      12      13      14      15      16      17      18      19      20      21      22      23
//	{  0, 25, 45, 50, 75, 65, 60, 40,  0,  0,  0, 65,100, 95,140,180,165,140,120, 95,  0,  0,  0,125,255,255,255,225,255,180,140,105, 35,  0,  0, 70, 95,100,105,110,110, 80, 52, 25,  0,  0,  0, 25},//red
//	{  0, 25, 30, 20,  0,  0,  0,  0, 20, 90,110,102,100, 85, 80, 60,  0,  0,  0, 30,120,170,250,255,255,185,155,120, 75,  0,  0, 17, 35,110,120,105, 95, 85, 65, 40,  0,  0,  0,  0,  0, 45, 45, 35},//green
//	{  0,  0,  0,  5, 10, 55, 90,105,115, 95,  0,  0,  0,  0,  0,  0, 95,140,175,210,205,145,  0, 30,  0,  0,  0, 20, 75,155,160,165,175,105,  0,  0,  0,  0,  0,  0,  5, 50, 72, 70, 60, 25,  0,  0} //blue
	
//	   0       1       2       3       4       5       6       7       8       9      10      11      12      13      14      15      16      17      18      19      20      21      22      23
	{  0, 25, 50, 50, 55, 50,  0,  0,  0,  0,  0, 65,100,142,165,140,120, 95,  0,  0,  0,  0,  0,125,255,255,255,200,140,105, 37, 10,  0,  0,  0, 55, 75, 90,110, 90, 75, 65, 50,  0,  0,  0,  0, 20},//red
	{  0, 30,  0,  0,  0,  0, 20, 57, 73, 90,110,102,100, 87, 60,  0,  0, 30,120,170,200,225,250,255,255,155, 90,  0,  0, 17, 37, 75,100,110,110, 90, 75, 50,  0,  0,  0,  0,  0, 25, 40, 45, 45, 40},//green
	{  0,  0,  8, 28, 50, 75,100, 78, 73, 40,  0,  0,  0,  0, 95,140,175,210,205,200,200,100,  0, 30,  0,100, 90,165,160,165,175,135,100, 50,  0,  0,  0,  0,  5, 30, 40, 55, 60, 65, 40, 25,  0,  0} //blue



/*{18,21,25,28,25,4,6,8,10,2,14,28,66,70,74,76,65,22,26,29,32,5,28,46,95,95,93,91,74,25,26,26,25,4,20,32,66,62,57,53,39,6,6,5,5,1,3,6}, // red
{1,3,5,9,79,13,17,20,23,23,24,23,10,20,32,55,255,39,43,45,47,45,43,39,20,37,53,80,255,42,43,42,42,37,33,27,10,16,20,26,165,18,17,15,13,10,7,4}, // green
{0,12,24,40,136,20,13,7,3,3,4,4,2,47,71,102,255,52,36,22,11,10,10,9,5,65,91,121,255,56,36,20,8,7,6,5,2,41,56,71,189,26,13,5,1,1,1,1} // blue
*/
	
	
};

uint8_t offset[3][HOURS*SEGMENTS_PER_HOUR]={
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//red
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue

	
};



uint8_t minute = 0;
uint8_t hour = 0;

uint8_t interva12 = 1;

static void print_offsets()
{
	for(uint8_t c = 0 ; c < 3 ; c++)
	{
		for(uint8_t i = 0 ; i < HOURS+SEGMENTS_PER_HOUR;i++)
		{
			printf("%i, ",offset[c][i]);
		}
		printf("\n");
	}
	printf("\n");
}

static uint8_t key(uint8_t key) {

	if(key == 1)
	{
		minute = 0;
	}
	if(key == 2)
	{
		if(minute<30)
		{
			minute+=30;
		}
		else
		{
			minute-=30;
			hour++;
			if(hour==HOURS)
				hour=0;
			
		}
	}
	if(key == 3)
	{
		if(minute>30)
		{
			minute-=30;
		}
		else
		{
			minute+=30;
			if(hour == 0)
				hour = HOURS;
			hour--;
		}
	}
	if(key == 4)
	{
		if(interva12 == 1)
		{
			interva12 = 5;
		}else{
			interva12 = 1;
		}
	
	}
	
	uint8_t index = hour*SEGMENTS_PER_HOUR;
	if(minute >= (60/SEGMENTS_PER_HOUR))
	{
			index++;
	}
	
	
	if(key == 5)
	{
		offset[0][index]++;
	}
	if(key == 6)
	{
		offset[0][index]--;
	}
	if(key == 7)
	{
		offset[1][index]++;
	}
	if(key == 8)
	{
		offset[1][index]--;
	}
	if(key == 9)
	{
		offset[2][index]++;
	}
	if(key == 0)
	{
		offset[2][index]--;
	}
	print_offsets();
}


static uint8_t tick(void) {

	if(sdlpause == 0)
	{
		minute+=interva12;
	}
	if(minute > 59)
	{
		minute=0;
		hour++;
	}
	if(hour==HOURS)
		hour=0;


	uint8_t index = hour*SEGMENTS_PER_HOUR;
	uint8_t index2 = minute;
	if(minute > (60/SEGMENTS_PER_HOUR))
	{
			index++;
			index2-=(60/SEGMENTS_PER_HOUR);
	}

	uint8_t next_index = index+1;
	if(next_index == HOURS*SEGMENTS_PER_HOUR)
		next_index=0;

	

	uint8_t r = (phase[0][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(phase[0][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));
	uint8_t g = (phase[1][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(phase[1][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));
	uint8_t b = (phase[2][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(phase[2][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));
	
	r += (offset[0][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(offset[0][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));
	g += (offset[1][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(offset[1][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));
	b += (offset[2][index]*((60/SEGMENTS_PER_HOUR)-index2)/(60.0/SEGMENTS_PER_HOUR))+(offset[2][next_index]*(index2)/(60.0/SEGMENTS_PER_HOUR));

	
	draw_filledCircle(64,64,70,0,0,0);
	draw_filledCircle(64,64,62,r,g,b);

	//printf("hour=%i min=%i r=%03i g=%03i b=%03i\n",hour,minute,r,g,b);

	draw_number(20,30,r,3,'0',255,255,255);
	draw_number(50,30,g,3,'0',255,255,255);
	draw_number(80,30,b,3,'0',255,255,255);
	
	draw_number(50,100,hour,2,'0',255,255,255);
	draw_text(  62,100,":",255,255,255);
	draw_number(68,100,minute,2,'0',255,255,255);

	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, key, 10, 450);
}

