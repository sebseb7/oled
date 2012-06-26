#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "font8x6.h"
#include "main.h"

void draw_char(int x,int y, char text, uint8_t r,uint8_t g, uint8_t b)
{
	text-=32;
	int i;
	for (i = 0; i < 6; i++)
	{
#ifdef __AVR__
		char ch = pgm_read_byte(&font8x6[(int)text][i]);
#else
		char ch =                font8x6[(int)text][i];
#endif

		int j;
		for (j = 0; j < 8; j++)
		{
			if(ch & (1<<j))
			{
				setLedXY(x+i,y+j+4,r,g,b);
			}
		}
	}
}
void draw_text(uint8_t x, uint8_t y, const char *text, uint8_t r,uint8_t g,uint8_t b)
{
	while (*text)
	{
		draw_char(x,y,*text,r,b,g);
		x+=6;
		text++;
	}

}


void draw_number(uint8_t x, uint8_t y, int32_t number, uint8_t length, uint8_t pad, uint8_t r, uint8_t g , uint8_t b)
{

	char s[10];
	sprintf(s, "%d", number);
	int len = strlen(s);

	if (length < len) {
		int i;
		for (i = 0; i < length; i++) {
			draw_char(x, y, '*', r,g,b);
			x+=6;
		}
		return;
	}
	int i;
	for (i = 0; i < length - len; i++) {
		draw_char(x, y, pad, r,g,b);
		x+=6;
	}
	draw_text(x, y, (char*)s, r,g,b);

}

