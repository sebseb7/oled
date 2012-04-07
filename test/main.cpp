/*	main.cpp
*
*	Sample code for SparkFun Graphic OLED Color Display 128x128 (LCD-00712)
*	controlled with an Atmel ATmega644 AVR controller.
*
*	See OLED.cpp for details on hardware connections to OLED.
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "Util.h"
#include "Timer.h"
#include "Comm.h"
#include "OLED.h"

void TestOLED();
void TestReadStatus();
void TestReadPixel();
void FindEdges();
void DrawEdges();
void DrawFullRect();
void ScrollDisplay();
void DrawCircles();
void DrawColorBars();
void FillScreenPixels();

int main()
{
	InitTimer();
	InitComm();

	PrintLn("hello, world!");

	InitOLED();
	
	TestOLED();

	return 0;
}

void TestOLED()
{
	/* if your display isn't working, this is a good place to start -- see
	   if you can read the command register */
	TestReadStatus();

	EnableFill(true);

//	FindEdges();
	DrawEdges();
	DrawFullRect();
	ScrollDisplay();
//	DrawColorBars();
	FillScreenPixels();
	DrawCircles();
	
	TestReadPixel();
}

void TestReadStatus()
{
	uchar b;
	char szBuf[17];
	
	b = ReadStatus();
	snprintf(szBuf, sizeof(szBuf), "cur: %x", b);
	PrintLn(szBuf);

	Sleep(true);
	b = ReadStatus();
	snprintf(szBuf, sizeof(szBuf), "sleep: %x", b);
	PrintLn(szBuf);
	
	Sleep(false);
	b = ReadStatus();
	snprintf(szBuf, sizeof(szBuf), "wake: %x", b);
	PrintLn(szBuf);
}

void TestReadPixel()
{
	int i;
	CLR clr;
	char szBuf[17];
	
	SetPixelBox(6, 6, 9, 9, false);
	
	for (i = 0; i < 16; ++i)
	{
		clr = ReadPixel();
		snprintf(szBuf, sizeof(szBuf), "(%02x, %02x, %02x)",
			RFromClr(clr), GFromClr(clr), BFromClr(clr));
		Print(szBuf);
		if (i % 4 == 3)
			PrintLn(0);
	}
}

/* draws lines at perimeter to help identify which rows & cols are visible */
void FindEdges()
{
	int r, c;

	ClearScreen();

	for (r = 0; r <= 10; r += 2)
		DrawLine(0, r, 131, r, clrRed);
	for (r = 131; r >= 121; r -= 2)
		DrawLine(0, r, 131, r, clrMagenta);
	for (c = 0; c <= 10; c += 2)
		DrawLine(c, 0, c, 131, clrCyan);
	for (c = 131; c >= 121; c -= 2)
		DrawLine(c, 0, c, 131, clrYellow);

	WaitMs(1000);
}

/* draws lines in first and last row and column, based on constants defined in OLED.h */
void DrawEdges()
{
	ClearScreen();

	DrawLine(0, 0, xMost, 0, clrRed);
	DrawLine(0, yLast, xMost, yLast, clrMagenta);
	DrawLine(0, 0, 0, yMost, clrCyan);
	DrawLine(xLast, 0, xLast, yMost, clrYellow);

	WaitMs(1000);
}

/* draws a rectangle (outline and fill), then inverts; this is another
   way to verify that the first and last row and col are set correctly;
   after inverting, you should see a red line all the way around the
   perimeter, with no gray outside it */
void DrawFullRect()
{
	ClearScreen();
	
	/* set all pixels, even off-screen */
	DrawRectangle(0, 0, xMost, yMost, clrGray, clrGray);

	DrawRectangle(0, 0, xLast, yLast, clrCyan, clrBlue);

	WaitMs(1000);
	InvertDisplay();
	WaitMs(1000);

#ifdef NO	// this can help determine the correct start line (used in init)
	SetStartLine(1);	WaitMs(1000);
	SetStartLine(2);	WaitMs(1000);
	SetStartLine(3);	WaitMs(1000);
	SetStartLine(4);	WaitMs(1000);
	SetStartLine(0);	WaitMs(1000);
	SetStartLine(131);	WaitMs(1000);
	SetStartLine(130);	WaitMs(1000);
	SetStartLine(129);	WaitMs(1000);
	SetStartLine(128);	WaitMs(1000);
	SetStartLine(127);	WaitMs(1000);
	SetStartLine(126);	WaitMs(1000);
	
	SetStartLine(128);	// put back to the right place!
#endif

	NormalDisplay();
}

void ScrollDisplay()
{
	int i;
	int d = 40;
	int ms = 10;	// delay

	for (i = 1; i < d; ++i)
	{
		SetRowOffset(i);
		WaitMs(ms);
	}
	while (--i >= 0)
	{
		SetRowOffset(i);
		WaitMs(ms);
	}
	for (i = 131; i > 131 - d; --i)
	{
		SetRowOffset(i);
		WaitMs(ms);
	}
	while (++i <= 131)
	{
		SetRowOffset(i);
		WaitMs(ms);
	}
	SetRowOffset(0);
}
	
/* draw 100 random circles (inspired by SparkFun's sample code) */
void DrawCircles()
{
	int i;
	
	ClearScreen();
	
	for (i = 0; i < 100; ++i)
	{
		DrawCircle(
			rand() % dxScreen,
			rand() % dyScreen,
			rand() % 32 + 10,
			ClrFromRGB(rand(), rand(), rand()),
			ClrFromRGB(rand(), rand(), rand()));
	}

	WaitMs(1000);
}

void DrawColorBars()
{
	int r, c;

	ClearScreen();

	SetPixelBox(0, 0, 39, 127, true);
	for(r = 0; r < 256; r += 2)
		for (c = 0; c < 40; ++c)
			DrawPixel(ClrFromRGB(0, r, 0));

	SetPixelBox(44, 0, 83, 127, true);
	for(r = 0; r < 256; r += 2)
		for (c = 0; c < 40; ++c)
			DrawPixel(ClrFromRGB(r, 0, 0));

	SetPixelBox(88, 0, 127, 127, true);
	for(r = 0; r < 256; r += 2)
		for (c = 0; c < 40; ++c)
			DrawPixel(ClrFromRGB(0, 0, r));

	WaitMs(1000);
}

/* uses direct writing to RAM to set each pixel on the screen;
   draws color swatches */
void FillScreenPixels()
{
	int r, c;

	ClearScreen();

	/* max resolution is 6 bits per pixel, so low 2 bits are ignored;
	   changing by 4 moves to next distinct value */
	   
	SetPixelBox(0, 0, 63, 63, true);
	for (r = 0; r < 256; r += 4)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(r, c, 0));

	SetPixelBox(64, 0, 127, 63, true);
	for (r = 0; r < 256; r += 4)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(r, 0, c));

	SetPixelBox(0, 64, 63, 127, true);
	for (r = 0; r < 256; r += 4)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(0, c, r));

	SetPixelBox(64, 64, 127, 79, true);
	for (r = 0; r < 16; ++r)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(c, 0, 0));
	SetPixelBox(64, 80, 127, 95, true);
	for (r = 0; r < 16; ++r)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(0, c, 0));
	SetPixelBox(64, 96, 127, 111, true);
	for (r = 0; r < 16; ++r)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(0, 0, c));
	SetPixelBox(64, 112, 127, 127, true);
	for (r = 0; r < 16; ++r)
		for (c = 0; c < 256; c += 4)
			DrawPixel(ClrFromRGB(c, c, c));

	WaitMs(1000);
}
