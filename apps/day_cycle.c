#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"


#include <string.h>

#include<sys/time.h>

#include <stdio.h>
#include <time.h>


#include "main.h"
#include "circle.h"
#include "text.h"
#include "day_cycle_phase.h"

#include <fcntl.h>
#include <termios.h>
#include<unistd.h>
#include<sys/time.h>

#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#include <errno.h>
#endif

int tty_fd;

int8_t offset[3][HOURS*SEGMENTS_PER_HOUR]={
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//red
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue

	
};

	uint8_t last_r = 0;
	uint8_t last_g = 0;
	uint8_t last_b = 0;


uint8_t minute = 0;
uint8_t hour = 0;

uint8_t interva12 = 1;

uint8_t initialized = 0;

static void usb_init(void)
{
	struct termios tio;
	
	//memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	printf("1ok\n");
#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
	tty_fd=open("/dev/cu.usbmodem411", O_RDWR | O_NONBLOCK);      
	printf("ok\n");
#else
	tty_fd=open("/dev/ttyACM0", O_RDWR | O_NONBLOCK);      
#endif
	cfsetospeed(&tio,B115200);            // 115200 baud
	cfsetispeed(&tio,B115200);            // 115200 baud
#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
	speed_t speed = 115200;
	if ( ioctl( tty_fd,	 IOSSIOSPEED, &speed ) == -1 )
	{
		printf( "Error %d calling ioctl( ..., IOSSIOSPEED, ... )\n", errno );
	}
#else
#endif 
	tcsetattr(tty_fd,TCSANOW,&tio);

}

static void print_offsets()
{
	FILE *file; 
	file = fopen("../apps/day_cycle_phase.h","w+");

	#define SIZE 0x100

	//time_t t;
	//char buffer[SIZE];
	//struct tm ltime;

	//t = time (0);
	//localtime_r (& t, & ltime);

	fprintf(file,"#define HOURS 24\n");
    fprintf(file,"#define SEGMENTS_PER_HOUR 2\n\n");

    fprintf(file,"uint8_t phase[3][HOURS*SEGMENTS_PER_HOUR]={\n\n");
	
//	   0       1       2       3       4       5       6       7       8       9      10      11      12      13      14      15      16      17      18      19      20      21      22      23

	
//	strftime (buffer, SIZE, "%A, %e of %B %Y , %H:%M", & ltime);
//	fprintf (file,"%s\n", buffer);

	for(uint8_t c = 0 ; c < 3 ; c++)
	{
		fprintf(file,"{");
		printf("{");
		for(uint8_t i = 0 ; i < HOURS*SEGMENTS_PER_HOUR;i++)
		{
			printf("%003i, ",offset[c][i]+phase[c][i]);
			fprintf(file,"%i, ",(uint8_t)(offset[c][i]+phase[c][i]));
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
	fprintf(file,"};\n");
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

	if(initialized == 0)
	{
		initialized = 1;
		usb_init();
	}

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

						if((last_r != r)||(last_g != g)||(last_b != b))
						{
							if(r == 0xff)
								r = 0xfe;
							if(g == 0xff)
								g = 0xfe;
							if(b == 0xff)
								b = 0xfe;
							
							char cmd1[] = {0xff}; 
							write(tty_fd,&cmd1,1);
							usleep(1000);
							char cmd1a[] = {0x0}; 
							write(tty_fd,&cmd1a,1);
							usleep(1000);
							
							char cmd2[] = {r}; 
							write(tty_fd,&cmd2,1);
							usleep(1000);
							
							char cmd3[] = {g}; 
							write(tty_fd,&cmd3,1);
							usleep(1000);
							
							char cmd4[] = {b}; 
							write(tty_fd,&cmd4,1);
							usleep(1000);
						}
						
						last_r = r;
						last_g = g;
						last_b = b;
	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, key, 10, 450);
}

