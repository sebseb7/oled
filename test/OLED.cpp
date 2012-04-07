/*	OLED.cpp
*
*	Sample code for SparkFun Graphic OLED Color Display 128x128 (LCD-00712)
*	controlled with an Atmel ATmega644 AVR controller.
*
*	Using the SparkFun carrier board,
*	- put jumpers on JP1 and JP2 (to generate 12V)
*	- set BS1 to GND (jumper toward the OLED)
*	- set BS2 to power (jumper toward the connector)
*	- power the board (Vcc) with 3.3V (2.4-3.5V range allowable according to spec sheet)
*	- leave EN disconnected (or connect to high)
*	- look at the 'OLED interface' macros below for pin assignments (or change as desired)
*
*	Uses the 6800-series MPU parallel interface
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "Util.h"
#include "Timer.h"
#include "OLED.h"

/*
*	OLED interface
*/

/* data */
#define SetData(x)	(PORTA = (x))		// A7-A0 is OLED D7-D0
#define GetData()	(PINA)
#define GetData8()	(PINC & 0x80)		// C7 is OLED D8
#define Data8High()	(PORTC |= 0x80)
#define Data8Low()	(PORTC &= ~0x80)
#define SetupDataOutput()	{ DDRA = 0xff; DDRC |= 0x80; }
#define SetupDataInput()	{ PORTA = 0; DDRA = 0; PORTC &= ~0x80; DDRC &= ~0x80; }

/* control */
#define EHigh()		(PORTC |= 0x40)		// C6
#define ELow()		(PORTC &= ~0x40)
#define RWHigh()	(PORTC |= 0x20)		// C5
#define RWLow()		(PORTC &= ~0x20)
#define CSHigh()	(PORTC |= 0x10)		// C4
#define CSLow()		(PORTC &= ~0x10)
#define DCHigh()	(PORTC |= 0x08)		// C3
#define DCLow()		(PORTC &= ~0x08)
#define ResetHigh()	(PORTC |= 0x04)		// C2
#define ResetLow()	(PORTC &= ~0x04)

#define InitOLEDLines() { SetupDataOutput(); DDRC |= 0x7c; }

/* OLED Commands */
enum
{
	ocConfig = 0xad,	// 1 data byte (see doc) [1000 1_1_ => 1000 1110 = 0x8e]
		/* bit 0: 0 = ext Vcc; 1 = DC-DC converter for Vcc
		   bit 1: 0 = ext Vcomh; 1 = internal Vcomh (set level w/ cmd 0xBE)
		   bit 2: 0 = ext Vpa/b/c; 1 = internal pre-charge V (set via 0xBB)
		   (def't are 1; need 0 for bit 0 for OLED board */
	ocSettings = 0xa0,	// 1 data byte (see doc) [bit3=1 for 9-bit bus] [bit7:6=01 for 65k color (def't)]

	ocColAddr = 0x15,	// 2 data bytes: start, end addr (range=0-131; def=0,131)
	ocRowAddr = 0x75,	// 2 data bytes: start, end addr (range=0-131; def=0,131)

	ocWriteRAM = 0x5c,	// enable MCU write to RAM
	ocReadRAM = 0x5d,	// enable MCU read from RAM

	ocDispAllOff = 0xa4,
	ocDispAllOn = 0xa5,		// all pixels have GS15
	ocDispNormal = 0xa6,	// default
	ocDispInverse = 0xa7,	// GS0->GS63, GS1->GS62, ...

	ocSleepOn = 0xae,		// (display off)
	ocSleepOff = 0xaf,		// (display on)

	ocStartLine = 0xa1,	// 1 data byte: vertical scroll by RAM (range=0-131; def=0)
	ocOffset = 0xa2,	// 1 data byte: vertical scroll by Row (range=0-131; def=0)

	ocPowerSaving = 0xb0,	// 1 data byte (see doc) [00: internal VSL, normal (def't); 0x12: power saving internal VSL]
	
	ocSetPeriod = 0xb1,	// 1 data byte: low nibble: phase 1 (reset) period [4]; high nibble: phase 2 [7](pre-charge) period; 1-16DCLK
	ocClock = 0xb3,		// 1 data byte: low nibble: divide by DIVSET+1 [0]; high nibble: osc freq [9? d?]

	ocGrayPulseWidth = 0xb8,	// 32 bytes (see doc)
	ocLinearLUT = 0xb9,		// reset to default (linear grayscale) Look Up Table (PW1=1, PW2=3, PW3=5 ... PW63=125)

	ocPrechargeV = 0xbb,	// 3 bytes: colors A, B, C; def't=0x1c; 0: 0.51*Vref, 1f: 0.84*Vref, 0x80: Vcomh
	ocSetVcomh = 0xbe,		// 1 byte: 0: 0.51*Vref, 1f: 0.84*Vref (def't)
	ocMUXRatio = 0xca,	// 1 byte: 16MUX-132MUX (range=15-131; def=131)
	ocNOP = 0xe3,
	ocLock = 0xfd,		// 1 byte: 0x12=unlock (def't); 0x16=lock

	ocColorContrast = 0xc1,	// 3 bytes: colors A, B, C; def't=0x80 (sets current)
	ocContrast = 0xc7,		// 1 byte: reduce output current (all colors) to (low nibble+1)/16 [def't=0f]

	ocDrawLine = 0x83,	// 6: colStart, rowStart, colEnd, rowEnd, color1, color2 (start < end < 132)
	ocDrawRect = 0x84,	// 8: colStart, rowStart, colEnd, rowEnd, line1, line2, fill1, fill2 (start < end < 132)
	ocDrawCirc = 0x86,	// 7: colCenter, rowCenter, radius, line1, line2, fill1, fill2
	ocCopy = 0x8a,		// 6: colStart, rowStart, colEnd, rowEnd, colNew, rowNew (start < end < 132)
	ocDimWin = 0x8c,	// 4: colStart, rowStart, colEnd, rowEnd (start < end < 132)
	ocClearWin = 0x8e,	// 4: colStart, rowStart, colEnd, rowEnd (start < end < 132)
	ocDrawSettings = 0x92,
	ocHScroll = 0x96,
	ocStartScroll = 0x9f,
	ocStopScroll = 0x9e,
};

static void SetControlState();
static void Write(int fCmd, uchar b);
static void WriteClrCmd(CLR clr);
static ushort Read(int fCmd);

#define WriteCmd(b)  Write(true, b)
#define WriteData(b) Write(false, b)

void InitOLED()
{
	InitOLEDLines();

	/* get ready for communication */
	SetControlState();
	
	/* give OLED a little time to react to power on */
	WaitMs(20);

	/* send reset */
	ResetLow();
	/* docs don't specify time required for reset, but parallel comms interface
	   indicates that it generally only needs 60ns at level (1 op = 62.5ns at 16 MHz) */
	asm("nop");
	asm("nop");
	ResetHigh();
	WaitMs(10);	// wait for reset to complete

	WriteCmd(ocLock);	// make sure not locked
	WriteData(0x12);

	WriteCmd(ocConfig);
	WriteData(0x8e);	// 1000 1110 -- external Vcc, internal Vcomh, Vpa/b/c

	/* settings command
	   bit values, descriptions from low to high (see OLED driver doc for details):
	   cc11 d100 -- increment horiz, SEG0=col0, reverse color CBA (RGB), [d = data bus size]
	                 COM n-1 to 0 (0 row top), enable COM split, [c = color depth]
	   note that reversing colors is helpful for the 256-color case since that means
	     that blue (not red) gets the 2-bit section (R/G are 3-bit)
	*/
	WriteCmd(ocSettings);
#if defined(COLOR_262k)		// 262k color (9-bit): 1011 1100
	WriteData(0xbc);
#elif defined(COLOR_65k)	// 65k color (8-bit): 0111 0100
	WriteData(0x74);
#elif defined(COLOR_256)	// 256 color (8-bit): 0011 0100
	WriteData(0x34);
#else
#error 'color depth not defined; see OLED.h'
#endif

	WriteCmd(ocPowerSaving);
//	WriteData(0x15);	// external VSL, power saving
	WriteData(0x12);	// internal VSL, power saving
//	WriteData(0x05);	// external VSL, normal
//	WriteData(0);		// internal VSL, normal [default]

	/* set the start line for the display; the code in this project uses
	   this property to adjust so that the first row displayed is row 0;
	   scrolling should be done using the Offset property */
	WriteCmd(ocStartLine);
	WriteData(128);	// otherwise the first row is 4... (131 => -1, so 132 - 4 = 128)

#ifdef DEFAULTS
	/* the documented default values should be fine for these, so we don't
	   need to set them specifically; they're listed here since they are
	   assumed to be set to the defaults */

	WriteCmd(ocMUXRatio);
	WriteData(131);

	WriteCmd(ocOffset);
	WriteData(0);

	WriteCmd(ocDispNormal);
#endif

	/* clear screen */
	ClearScreen();

	/* make sure display is on */
	Sleep(false);
}

void Sleep(int fSleep)
{
	WriteCmd(fSleep ? ocSleepOn : ocSleepOff);
	WaitMs(5);	// give it time to finish
}

void ClearScreen()
{
	WriteCmd(ocClearWin);
	WriteData(0);
	WriteData(0);
	WriteData(xMost);
	WriteData(yMost);
	WaitMs(5);	// give it time to finish
}

void InvertDisplay()
{
	WriteCmd(ocDispInverse);
	WaitMs(5);	// give it time to finish
}

/* restore normal operation after invert */
void NormalDisplay()
{
	WriteCmd(ocDispNormal);
	WaitMs(5);	// give it time to finish
}

void EnableFill(int fEnable)
{
	WriteCmd(ocDrawSettings);
	/* other bits in this command affect how copy works; defaults are 0 */
	WriteData(fEnable ? 0x01 : 0);
}

void DrawLine(uchar x1, uchar y1, uchar x2, uchar y2, CLR clr)
{
	WriteCmd(ocDrawLine);
	WriteData(x1);
	WriteData(y1);
	WriteData(x2);
	WriteData(y2);
	WriteClrCmd(clr);
	WaitMs(5);	// give it time to do the drawing
}

void DrawRectangle(uchar xLeft, uchar yTop, uchar xRight, uchar yBottom, CLR clrOutline, CLR clrFill)
{
	WriteCmd(ocDrawRect);
	WriteData(xLeft);
	WriteData(yTop);
	WriteData(xRight);
	WriteData(yBottom);
	WriteClrCmd(clrOutline);
	WriteClrCmd(clrFill);
	WaitMs(5);	// give it time to do the drawing
}

void DrawCircle(uchar x, uchar y, uchar r, CLR clrOutline, CLR clrFill)
{
	WriteCmd(ocDrawCirc);
	WriteData(x);	// x center
	WriteData(y);	// y center
	WriteData(r);	// radius
	WriteClrCmd(clrOutline);
	WriteClrCmd(clrFill);
	WaitMs(5);	// give it time to do the drawing
}

void SetPixelBox(uchar xLeft, uchar yTop, uchar xRight, uchar yBottom, int fWrite)
{
	WriteCmd(ocColAddr);
	WriteData(xLeft);
	WriteData(xRight);
	WriteCmd(ocRowAddr);
	WriteData(yTop);
	WriteData(yBottom);
	if (fWrite)
		WriteCmd(ocWriteRAM);
	else
	{
		WriteCmd(ocReadRAM);
		(void)Read(false);	// dummy read
	}
}

/* shift the display so the specified row is at the top of the screen */
void SetRowOffset(uchar b)
{
	WriteCmd(ocOffset);
	WriteData(b);
	WaitMs(5);
}

/* set the start line for the display; the code in this project uses
   this property to adjust so that the first row displayed is row 0;
   so, you probably want to use SetRowOffset for a scrolling effect */
void SetStartLine(uchar b)
{
	WriteCmd(ocStartLine);
	WriteData(b);
	WaitMs(5);
}

uchar ReadStatus()
{
	return (uchar)Read(true);
}


/*
*	low-level functions that communicate with OLED
*/

static void SetControlState()
{
	ELow();		// not enabled
	CSHigh();	// not selected
	RWLow();	// write by default
}

static void Write(int fCmd, uchar b)
{
	/* (RW starts low, E starts low, CS# starts high) */

	fCmd ? DCLow() : DCHigh();		// set data bit for command or data

	SetData(b);
	
	EHigh();
	CSLow();
	asm("nop");		// hold state for >= 60ns (1 instruction = 125ns for 8MHz uC)
	CSHigh();
	ELow();
}

/* write a color using the 2-byte representation used by drawing commands */
static void WriteClrCmd(CLR clr)
{
	/* 8 bits / color stored in CLR, but only need 5 (R/B) or 6 (G); use high bits */
	// RRRRRGGG GGGBBBBB
	WriteData( ((clr & 0xf80000 ) >> 16) | ((clr & 0x00e000 ) >> 13) );
	WriteData( ((clr & 0x001c00 ) >> 5) | ((clr & 0x0000f8) >> 3) );
}

/* write a color to RAM using the format corresponding to the color depth and bus width */
/* Note: requires call to SetPixelBox prior to this call */
void DrawPixel(CLR clr)
{
#if defined(COLOR_262k)
	// R RRRRRGGG  G GGBBBBBB
	(clr & 0x800000) ? Data8High() : Data8Low();
	WriteData( ((clr & 0x7c0000 ) >> 15) | ((clr & 0x00e000 ) >> 13) );
	(clr & 0x001000) ? Data8High() : Data8Low();
	WriteData( ((clr & 0x000c00 ) >> 4) | ((clr & 0x0000fc) >> 2) );
#elif defined(COLOR_65k)
	// RRRRRGGG GGGBBBBB
	WriteClrCmd(clr);
#elif defined(COLOR_256)
	// RRRGGGBB
	WriteData(((clr & 0xe00000) >> 16) | ((clr & 0x00e000) >> 11) | ((clr & 0x0000c0) >> 6) );
#else
#error 'color depth not defined'
#endif
}

static ushort Read(int fCmd)
{
	ushort w;
	
	/* (RW starts low, E starts low, CS# starts high) */

	RWHigh();	// read operation
	SetupDataInput();

	fCmd ? DCLow() : DCHigh();		// set data bit for command or data

	EHigh();
	CSLow();
	asm("nop");		// hold state for >= 120ns (1 instruction = 125ns for 8MHz uC)
	asm("nop");		// just to be sure...
	w = GetData();
#ifdef COLOR_262k
	if (GetData8())
		w |= 0x0100;
#endif
	CSHigh();
	ELow();

	RWLow();	// (default is write)
	SetupDataOutput();
	
	return w;
}

/* Note: requires call to SetPixelBox prior to this call */
CLR ReadPixel()
{
	uchar r, g, b;
	
#if defined(COLOR_262k)
	// R RRRRRGGG  G GGBBBBBB
	ushort w1 = Read(false);
	ushort w2 = Read(false);
	r = (w1 & 0x01f8) >> 1;
	g = ((w1 & 0x07) << 5) | ((w2 & 0x01c0) >> 4);
	b = (w2 & 0x3f) << 2;
#elif defined(COLOR_65k)
	// RRRRRGGG GGGBBBBB
	uchar x1 = (uchar)Read(false);
	uchar x2 = (uchar)Read(false);
	r = x1 & 0xf8;
	g = ((x1 & 0x07) << 5) | ((x2 & 0xe0) >> 3);
	b = (x2 & 0x1f) << 3;
#elif defined(COLOR_256)
	// RRRGGGBB
	uchar x = (uchar)Read(false);
	r = x & 0xe0;
	g = (x & 0x1c) << 3;
	b = (x & 0x03) << 6;
#else
#error 'color depth not defined'
#endif

	return ClrFromRGB(r, g, b);
}
