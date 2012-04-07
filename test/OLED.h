/*	OLED.h
*
*	Header file for OLED
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#pragma once

/* leave ONE of these lines uncommented; use this to select the number of colors available */
//#define COLOR_262k
#define COLOR_65k
//#define COLOR_256

/*
coordinate system
	upper left corner is (0,0)
	positive x is to the right
	positive y is down
variable names
	dx = "delta x" (or "difference x") which is width
	dy = "delta y" which is height
	clr = color
*/

/* geometry of the screen and memory */
const int dxScreen = 128;	// number of columns on screen
const int dyScreen = 128;	// number of rows on screen
const int dxRAM = 132;		// number of cols stored in RAM (including off-screen area)
const int dyRAM = 132;		// number of rows stored in RAM (including off-screen area)

/* 'last' values are largest values that are visible on the screen */
const int xLast = dxScreen-1;
const int yLast = dyScreen-1;
/* 'most' values are largest valid values (e.g. for drawing) */
const int xMost = dxRAM-1;
const int yMost = dyRAM-1;


/* CoLoRs */
typedef ulong CLR;
/* build a CLR from specified RGB values (0-255 each) */
#define ClrFromRGB(r, g, b) ( ((((ulong)r)&0xff) << 16) | ((((ulong)g)&0xff) << 8) | (((ulong)b)&0xff) )
/* extract red, green, and blue components from a CLR */
#define RFromClr(clr) ((uchar)((clr&0xff0000)>>16))
#define GFromClr(clr) ((uchar)((clr&0x00ff00)>>8))
#define BFromClr(clr) ((uchar)(clr&0x0000ff))

const CLR clrBlack = ClrFromRGB(0, 0, 0);
const CLR clrWhite = ClrFromRGB(0xff, 0xff, 0xff);

const CLR clrRed = ClrFromRGB(0xff, 0, 0);
const CLR clrGreen = ClrFromRGB(0, 0xff, 0);
const CLR clrBlue = ClrFromRGB(0, 0, 0xff);
const CLR clrCyan = ClrFromRGB(0, 0xff, 0xff);
const CLR clrMagenta = ClrFromRGB(0xff, 0, 0xff);
const CLR clrYellow = ClrFromRGB(0xff, 0xff, 0);
const CLR clrGray = ClrFromRGB(0x7f, 0x7f, 0x7f);


void InitOLED();
void Sleep(int fSleep);
void ClearScreen();
void InvertDisplay();
void NormalDisplay();	// restore normal operation after invert

void EnableFill(int fEnable);	// enable/disable fill for drawing (rect, circle)
void DrawLine(uchar x1, uchar y1, uchar x2, uchar y2, CLR clr);
void DrawRectangle(uchar xLeft, uchar yTop, uchar xRight, uchar yBottom, CLR clrOutline, CLR clrFill);
void DrawCircle(uchar x, uchar y, uchar r, CLR clrOutline, CLR clrFill);

/* call SetPixelBox (with fWrite = true) to set a bounding box for all the pixels
   to output, then call DrawPixel repeatedly to fill it */
void SetPixelBox(uchar xLeft, uchar yTop, uchar xRight, uchar yBottom, int fWrite);
void DrawPixel(CLR clr);

/* shift the display so the specified row is at the top of the screen */
void SetRowOffset(uchar b);

/* set the start line for the display; the code in this project uses
   this property to adjust so that the first row displayed is row 0;
   so, you probably want to use SetRowOffset for a scrolling effect */
void SetStartLine(uchar b);		// WARNING: you really don't want to use this!

uchar ReadStatus();		// read status register
CLR ReadPixel();		// read pixel value; call SetPixelBox first (with fWrite = false)
