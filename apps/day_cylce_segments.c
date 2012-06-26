#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"
#include "rect.h"

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

int8_t last[3][HOURS*SEGMENTS_PER_HOUR]={
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//red
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue

	
};
int8_t offset[3][HOURS*SEGMENTS_PER_HOUR]={
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//red
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},//green
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0} //blue

	
};



uint8_t minute = 0;
uint8_t hour = 0;

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
							
							
							char cmd1[] = {0xff}; 
							write(tty_fd,&cmd1,1);
							usleep(1000);
							char cmd1a[] = {0x0}; 
							write(tty_fd,&cmd1a,1);
							usleep(1000);
							
							char cmd2[] = {0}; 
							write(tty_fd,&cmd2,1);
							usleep(1000);
							
							char cmd3[] = {0}; 
							write(tty_fd,&cmd3,1);
							usleep(1000);
							
							char cmd4[] = {0}; 
							write(tty_fd,&cmd4,1);
							usleep(1000);

}

static void print_offsets()
{
	FILE *file; 
	file = fopen("../apps/day_cycle_phase.h","w+");

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
			fprintf(file,"%i, ",offset[c][i]+phase[c][i]);
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
	
	uint8_t index = hour*SEGMENTS_PER_HOUR;
	if(minute >= (60/SEGMENTS_PER_HOUR))
	{
			index++;
	}
	
	
	if(key == 5)
	{
		offset[0][index]++;
		print_offsets();
	}
	if(key == 6)
	{
		offset[0][index]--;
		print_offsets();
	}
	if(key == 7)
	{
		offset[1][index]++;
		print_offsets();
	}
	if(key == 8)
	{
		offset[1][index]--;
		print_offsets();
	}
	if(key == 9)
	{
		offset[2][index]++;
		print_offsets();
	}
	if(key == 0)
	{
		offset[2][index]--;
		print_offsets();
	}
}


static uint8_t tick(void) {

	if(initialized == 0)
	{
		initialized = 1;
		usb_init();
	}



	uint8_t index = hour*SEGMENTS_PER_HOUR;
	if(minute >= (60/SEGMENTS_PER_HOUR))
	{
			index++;
	}
	
	draw_filledRect(0,0,128,128,0,0,0);

	uint8_t i = 0;
	uint8_t j = 0;
	for(i=0;i<8;i++)
	{
		for(j=0;j<3;j++)
		{
			draw_filledRect(i*16,j*2*16,16,16,
			phase[0][j*8*2+i]+offset[0][j*8*2+i],
			phase[1][j*8*2+i]+offset[1][j*8*2+i],
			phase[2][j*8*2+i]+offset[2][j*8*2+i]);

			if((j*8*2+i)==index)
			{
				draw_Rect(i*16,j*2*16,15,15,255,255,255);
			}

		}
		for(j=0;j<3;j++)
		{
			draw_filledRect((7-i)*16,((j*2)+1)*16,16,16,
			phase[0][((j*2)+1)*8+i]+offset[0][((j*2)+1)*8+i],
			phase[1][((j*2)+1)*8+i]+offset[1][((j*2)+1)*8+i],
			phase[2][((j*2)+1)*8+i]+offset[2][((j*2)+1)*8+i]);
			if((((j*2)+1)*8+i)==index)
			{
				draw_Rect((7-i)*16,((j*2)+1)*16,15,15,255,255,255);
			}
		}
	}

	for(i=0;i<48;i++)
	{
		setLedXY(i*2,127,phase[0][i]+offset[0][i],phase[1][i]+offset[1][i],phase[2][i]+offset[2][i]);
		setLedXY(i*2+1,127,phase[0][i]+offset[0][i],phase[1][i]+offset[1][i],phase[2][i]+offset[2][i]);
							
		uint8_t r = phase[0][i]+offset[0][i];
		uint8_t g = phase[1][i]+offset[1][i];
		uint8_t b = phase[2][i]+offset[2][i];
		uint8_t last_r = last[0][i];
		uint8_t last_g = last[1][i];
		uint8_t last_b = last[2][i];

		if((last_r != r)||(last_g != g)||(last_b != b))
		{	
			last[0][i]=r;
			last[1][i]=g;
			last[2][i]=b;
			
			if(r == 0xff)
				r = 0xfe;
			if(g == 0xff)
				g = 0xfe;
			if(b == 0xff)
				b = 0xfe;
			
			char cmd1[] = {0xff}; 
			write(tty_fd,&cmd1,1);
			usleep(1000);
			char cmd1a[] = {i+1}; 
			write(tty_fd,&cmd1a,1);
			usleep(1000);
			char cmd1b[] = {2}; 
			write(tty_fd,&cmd1b,1);
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
	}

	draw_number(20,113,phase[0][index]+offset[0][index],3,'0',255,255,255);
	draw_number(50,113,phase[1][index]+offset[1][index],3,'0',255,255,255);
	draw_number(80,113,phase[2][index]+offset[2][index],3,'0',255,255,255);
	
	draw_number(50-5,100,hour,2,'0',255,255,255);
	draw_text(  62-5,100,":",255,255,255);
	draw_number(68-5,100,minute,2,'0',255,255,255);

	return 0;
}



static void init(void) ATTRIBUTES
void init(void) {
	
	registerAnimation(tick, key, 10, 450);
}

