#include <stdio.h>
#include <time.h>


#include "main.h"
#include "text.h"
#include "rect.h"
#include "day_cycle_phase.h"



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

}

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

