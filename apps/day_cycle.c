#include <stdio.h>
#include <time.h>


#include "main.h"
#include "circle.h"
#include "text.h"
#include "day_cycle_phase.h"



int8_t offset[3][HOURS*SEGMENTS_PER_HOUR]={
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//red
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue

	
};



uint8_t minute = 0;
uint8_t hour = 0;

uint8_t interva12 = 1;

static void print_offsets()
{
	FILE *file; 
	file = fopen("saved_colors.txt","a+");

	#define SIZE 0x100

	time_t t;
	char buffer[SIZE];
	struct tm ltime;

	t = time (0);
	localtime_r (& t, & ltime);
	
	strftime (buffer, SIZE, "%A, %e of %B %Y , %H:%M", & ltime);
	fprintf (file,"%s\n", buffer);

	for(uint8_t c = 0 ; c < 3 ; c++)
	{
		fprintf(file,"{");
		printf("{");
		for(uint8_t i = 0 ; i < HOURS*SEGMENTS_PER_HOUR;i++)
		{
			printf("%003i, ",offset[c][i]+phase[c][i]);
			fprintf(file,"%003i, ",offset[c][i]+phase[c][i]);
		}
		if(c ==2)
		{
			printf("}\n");
			fprintf(file,"}\n");
		}else
		{
			printf("},\n");
			fprintf(file,"},\n");
		}

		
	}
	printf("\n");
	fprintf(file,"\n");
	fclose(file); 
	
	for(uint8_t c = 0 ; c < 3 ; c++)
	{
		for(uint8_t i = 0 ; i < HOURS*SEGMENTS_PER_HOUR;i++)
		{
			printf("%i, ",offset[c][i]);
		}
		printf("\n");
	}
	printf("\n");
}

static void key(uint8_t key) {

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
		if(minute>=30)
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
	if(minute >= (60/SEGMENTS_PER_HOUR))
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

