#include "stdio.h"
#include "main.h"


void draw_Rect(
	unsigned int x, unsigned int y,
	unsigned int h, unsigned int w,
	uint8_t r,
	uint8_t g,
	uint8_t b )
{
	uint8_t i,j;

	for(i=0;i<h;i++)
	{
		setLedXY(i+x,y,r,g,b);
		setLedXY(i+x+1,y+w,r,g,b);
	}
	for(j=0;j<w;j++)
	{
		setLedXY(x,j+y+1,r,g,b);
		setLedXY(x+h,j+y,r,g,b);
	}
}


void draw_filledRect(
	unsigned int x, unsigned int y,
	unsigned int h, unsigned int w,
	uint8_t r,
	uint8_t g,
	uint8_t b )
{
	uint8_t i,j;

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			setLedXY(i+x,j+y,r,g,b);
		}
	}
}


