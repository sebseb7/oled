#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "font8x6.h"
#include "main.h"

void draw_char(int x,int y, char text, uint8_t r,uint8_t g, uint8_t b)
{
	text-=32;
	int i;
	for (i = 0; i < 6; i++)
	{
		char ch = font8x6[(int)text][i];

		int j;
		for (j = 0; j < 8; j++)
		{
			if(ch & (1<<j))
			{
				setLedXY(x+i,y-j+4,r,g,b);
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
