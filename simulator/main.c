#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"

#include<unistd.h>
#include<sys/time.h>

#include <fcntl.h>
#include <termios.h>
#include <string.h>


#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#include <errno.h>
#endif


int tty_fd;
int sdlpause = 0;

unsigned long long int get_clock(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long long int)tv.tv_usec + 1000000*tv.tv_sec;
}


int leds[LED_HEIGHT][LED_WIDTH][4];
int interval;
tick_fun tick_fp;
key_fun key_fp;
SDL_Surface* screen;

void setLedXY(uint8_t x, uint8_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
	leds[y][x][3] = 1;
}
void getLedXY(uint8_t x, uint8_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
}

void registerAnimation(tick_fun tick,key_fun key, uint16_t t, uint16_t ignore)
{
	tick_fp = tick;
	key_fp = key;

	assert(t > 0);
	// 122Hz / tick
	interval = 1000000 / 122 * t;
}


int main(int argc, char *argv[]) {
	int x, y;

        struct termios tio;
 
 
        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;
 
        tty_fd=open("/dev/cu.usbmodem471", O_RDWR | O_NONBLOCK);      
        cfsetospeed(&tio,B115200);            // 115200 baud
        cfsetispeed(&tio,B115200);            // 115200 baud
#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
		speed_t speed = 38400;
		if ( ioctl( tty_fd,	 IOSSIOSPEED, &speed ) == -1 )
		{
			printf( "Error %d calling ioctl( ..., IOSSIOSPEED, ... )\n", errno );
		}
#warning osx

#else
#endif 
        tcsetattr(tty_fd,TCSANOW,&tio);
	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			leds[y][x][0]=0;
			leds[y][x][1]=0;
			leds[y][x][2]=0;
			leds[y][x][3]=1;
		}
	}

	srand(time(NULL));

	screen = SDL_SetVideoMode(LED_WIDTH * ZOOM + ZOOM / 15, LED_HEIGHT * ZOOM + ZOOM / 15,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM+(ZOOM/15), LED_HEIGHT*ZOOM+(ZOOM/15) };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0x20,0x20,0x20));


	int last_r = 0;
	int last_g = 0;
	int last_b = 0;


	int running = 1;
	unsigned long long int startTime = get_clock();
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							if(sdlpause == 0)
							{
								sdlpause = 1;
							}
							else{
								sdlpause = 0;
							}
							break;
						case SDLK_1:
							key_fp(1);
							break;
						case SDLK_2:
							key_fp(2);
							break;
						case SDLK_3:
							key_fp(3);
							break;
						case SDLK_4:
							key_fp(4);
							break;
						case SDLK_5:
							key_fp(5);
							break;
						case SDLK_6:
							key_fp(6);
							break;
						case SDLK_7:
							key_fp(7);
							break;
						case SDLK_8:
							key_fp(8);
							break;
						case SDLK_9:
							key_fp(9);
							break;
						case SDLK_0:
							key_fp(0);
							break;
							
						default: break;
					}
				default: break;
			}
		}

		running &= !tick_fp();

	//	char cmd1[] = {49,49,51}; 
	//	write(tty_fd,&cmd1,3);
		
		
		
		
		
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				if(leds[y][x][3] == 1)
				{

					SDL_Rect rect = { ZOOM*x+(ZOOM/15), ZOOM*( y )+(ZOOM/15), ZOOM-(ZOOM/15), ZOOM-(ZOOM/15) };
					SDL_FillRect(
						screen, 
						&rect, 
						SDL_MapRGB(screen->format, leds[y][x][0],leds[y][x][1],leds[y][x][2])
					);
					leds[y][x][3] = 0;
			
					if((x==40)&&(y==40))
					{
						int r = leds[y][x][0];
						int g = leds[y][x][1];
						int b = leds[y][x][2];
				
//						r=18;
//						g=32;
//						b=18;


/*						r += 17;
						g += 31;
						b += 17;
						
						if(r>245)
						{
							r=245;
						}
						if(g>245)
						{
							g=245;
						}
						if(b>245)
						{
							b=245;
						}
*/					
					
						//printf("%x %x %x\n",1|((r>>2)<<2),2|((g>>2)<<2),3|((b>>2)<<2));
					
//						char cmd1[] = {1|((r>>2)<<2),2|((g>>2)<<2),3|((b>>2)<<2)}; 
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
						
					}


				}

			}
		}

		startTime+=interval;
		int delay = startTime-get_clock();
		if(delay > 0)
			usleep(delay);
		
		SDL_Flip(screen);
	}

	SDL_Quit();
	return 0;
}

