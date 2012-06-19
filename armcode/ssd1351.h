#ifndef __SSD1351_H__
#define __SSD1351_H__

#include "main.h"

#define LED_WIDTH 128
#define LED_HEIGHT 128

#define I6800
#define C262K


typedef struct
{
  uint16_t width;         // LCD width in pixels (default orientation)
  uint16_t height;        // LCD height in pixels (default orientation)
} lcdProperties_t;

/*=========================================================================
    SPI modes
    -----------------------------------------------------------------------
    The OLED supports both 3-pin and 4-pin SPI modes

    3-PIN MODE      Saves one GPIO pin (D/C) by adding the D/C bit before
                    every transfer, meaning that 9 bits are sent every frame
                    instead of 8.  You have slightly slower performance
                    but use less pins. Requires 3 GPIO pins (SCK, SID, RST)

    4-PIN MODE      Uses a normal SPI interface with 8-bits per transfer,
                    plus a dedicated D/C pin to indicate if this is a
                    command or data byte. Requires 4 GPIO pins (SCK, SID,
                    RST, DC).

    To select one of the SPI modes, make sure the BS0/1 pins are correctly
    set on the OLED display and uncomment the appropriate define below.
    -----------------------------------------------------------------------*/
  //  #define SSD1351_BUS_SPI3
    #define SSD1351_BUS_SPI4
/*=========================================================================*/

// Macros for control line state

/*
#define SET_SCK	(LPC_GPIO0->DATA |= (1<<10))	
#define CLR_SCK	(LPC_GPIO0->DATA &= ~(1<<10))
#define SET_SID	(LPC_GPIO0->DATA |= (1<<9))	
#define CLR_SID	(LPC_GPIO0->DATA &= ~(1<<9))
#define SET_CS	(LPC_GPIO0->DATA |= (1<<11))	
#define CLR_CS	(LPC_GPIO0->DATA &= ~(1<<11))
#define SET_DC	(LPC_GPIO1->DATA |= (1<<0))	
#define CLR_DC	(LPC_GPIO1->DATA &= ~(1<<0))
#define SET_RST	(LPC_GPIO1->DATA |= (1<<1))	
#define CLR_RST	(LPC_GPIO1->DATA &= ~(1<<1))
*/

/*#define SET_SCK	(LPC_GPIO2->MASKED_ACCESS[(1<<6)] = (1<<6))	
#define CLR_SCK	(LPC_GPIO2->MASKED_ACCESS[(1<<6)] = 0)
#define SET_SID	(LPC_GPIO2->MASKED_ACCESS[(1<<7)] = (1<<7))	
#define CLR_SID	(LPC_GPIO2->MASKED_ACCESS[(1<<7)] = 0)
*/
/*
#define SET_CS	(LPC_GPIO0->MASKED_ACCESS[(1<<8)] = (1<<8))	
#define CLR_CS	(LPC_GPIO0->MASKED_ACCESS[(1<<8)] = 0)
*/
#define SET_DC	(LPC_GPIO3->MASKED_ACCESS[(1<<0)] = (1<<0))	
#define CLR_DC	(LPC_GPIO3->MASKED_ACCESS[(1<<0)] = 0)
#define SET_RST	(LPC_GPIO0->MASKED_ACCESS[(1<<7)] = (1<<7))	
#define CLR_RST	(LPC_GPIO0->MASKED_ACCESS[(1<<7)] = 0)


#ifdef I6800
#define SET_E (LPC_GPIO0->MASKED_ACCESS[(1<<11)] = (1<<11))
#define CLR_E (LPC_GPIO0->MASKED_ACCESS[(1<<11)] = 0)
#define SET_D0 (LPC_GPIO2->MASKED_ACCESS[(1<<5)] = (1<<5))
#define CLR_D0 (LPC_GPIO2->MASKED_ACCESS[(1<<5)] = 0)
#define SET_D1 (LPC_GPIO2->MASKED_ACCESS[(1<<11)] = (1<<11))
#define CLR_D1 (LPC_GPIO2->MASKED_ACCESS[(1<<11)] = 0)
#define SET_D2 (LPC_GPIO2->MASKED_ACCESS[(1<<2)] = (1<<2))
#define CLR_D2 (LPC_GPIO2->MASKED_ACCESS[(1<<2)] = 0)
#define SET_D3 (LPC_GPIO2->MASKED_ACCESS[(1<<10)] = (1<<10))
#define CLR_D3 (LPC_GPIO2->MASKED_ACCESS[(1<<10)] = 0)
#define SET_D4 (LPC_GPIO2->MASKED_ACCESS[(1<<9)] = (1<<9))
#define CLR_D4 (LPC_GPIO2->MASKED_ACCESS[(1<<9)] = 0)
#define SET_D5 (LPC_GPIO2->MASKED_ACCESS[(1<<4)] = (1<<4))
#define CLR_D5 (LPC_GPIO2->MASKED_ACCESS[(1<<4)] = 0)
#define SET_D6 (LPC_GPIO2->MASKED_ACCESS[(1<<3)] = (1<<3))
#define CLR_D6 (LPC_GPIO2->MASKED_ACCESS[(1<<3)] = 0)
#define SET_D7 (LPC_GPIO2->MASKED_ACCESS[(1<<8)] = (1<<8))
#define CLR_D7 (LPC_GPIO2->MASKED_ACCESS[(1<<8)] = 0)
#endif


/* clock 06
 * data  09
 *
 */


// SSD1351 Commands
enum
{
  SSD1351_CMD_SETCOLUMNADDRESS          = 0x15,
  SSD1351_CMD_SETROWADDRESS             = 0x75,
  SSD1351_CMD_WRITERAM                  = 0x5C, // Write data to GRAM and increment until another command is sent
  SSD1351_CMD_READRAM                   = 0x5D, // Read data from GRAM and increment until another command is sent
  SSD1351_CMD_COLORDEPTH                = 0xA0, // Numerous functions include increment direction ... see DS 
                                                // A0[0] = Address Increment Mode (0 = horizontal, 1 = vertical)
                                                // A0[1] = Column Address Remap (0 = left to right, 1 = right to left)
                                                // A0[2] = Color Remap (0 = ABC, 1 = CBA) - HW RGB/BGR switch
                                                // A0[4] = COM Scan Direction (0 = top to bottom, 1 = bottom to top)
                                                // A0[5] = Odd/Even Paid Split
                                                // A0[7:6] = Display Color Mode (00 = 8-bit, 01 = 65K, 10/11 = 262K, 8/16-bit interface only)
  SSD1351_CMD_SETDISPLAYSTARTLINE       = 0xA1,
  SSD1351_CMD_SETDISPLAYOFFSET          = 0xA2, 
  SSD1351_CMD_SETDISPLAYMODE_ALLOFF     = 0xA4, // Force entire display area to grayscale GS0
  SSD1351_CMD_SETDISPLAYMODE_ALLON      = 0xA5, // Force entire display area to grayscale GS63
  SSD1351_CMD_SETDISPLAYMODE_RESET      = 0xA6, // Resets the display area relative to the above two commands
  SSD1351_CMD_SETDISPLAYMODE_INVERT     = 0xA7, // Inverts the display contents (GS0 -> GS63, GS63 -> GS0, etc.)
  SSD1351_CMD_FUNCTIONSELECTION         = 0xAB, // Enable/Disable the internal VDD regulator
  SSD1351_CMD_SLEEPMODE_DISPLAYOFF      = 0xAE, // Sleep mode on (display off)
  SSD1351_CMD_SLEEPMODE_DISPLAYON       = 0xAF, // Sleep mode off (display on)
  SSD1351_CMD_SETPHASELENGTH            = 0xB1, // Larger capacitance may require larger delay to discharge previous pixel state
  SSD1351_CMD_ENHANCEDDRIVINGSCHEME     = 0xB2, 
  SSD1351_CMD_SETFRONTCLOCKDIV          = 0xB3, // DCLK divide ration fro CLK (from 1 to 16)
  SSD1351_CMD_SETSEGMENTLOWVOLTAGE      = 0xB4,
  SSD1351_CMD_SETGPIO                   = 0xB5,
  SSD1351_CMD_SETSECONDPRECHARGEPERIOD  = 0xB6,
  SSD1351_CMD_GRAYSCALELOOKUP           = 0xB8,
  SSD1351_CMD_LINEARLUT                 = 0xB9,
  SSD1351_CMD_SETPRECHARGEVOLTAGE       = 0xBB,
  SSD1351_CMD_SETVCOMHVOLTAGE           = 0xBE,
  SSD1351_CMD_SETCONTRAST               = 0xC1,
  SSD1351_CMD_MASTERCONTRAST            = 0xC7,
  SSD1351_CMD_SETMUXRRATIO              = 0xCA,
  SSD1351_CMD_NOP3                      = 0xD1,
  SSD1351_CMD_NOP4                      = 0xE3,
  SSD1351_CMD_SETCOMMANDLOCK            = 0xFD,
  SSD1351_CMD_HORIZONTALSCROLL          = 0x96,
  SSD1351_CMD_STOPMOVING                = 0x9E,
  SSD1351_CMD_STARTMOVING               = 0x9F  
};

void lcdFillRGB(uint8_t r,uint8_t g,uint8_t b);
void lcdFillRGB2(uint8_t r,uint8_t g,uint8_t b);
void lcdFillRGBdither(uint8_t r,uint8_t g,uint8_t b,uint8_t r2,uint8_t g2, uint8_t b2,uint8_t val);
void lcdFillRGBd(uint8_t r,uint8_t g,uint8_t b);
void lcdInit(void);
void setLedXY(uint8_t x, uint8_t y, uint8_t r,uint8_t g, uint8_t b);
void ssd1351SetCursor(uint8_t x, uint8_t y);
void ssd1351SendByte(uint8_t byte, uint8_t command);
extern uint16_t port2_table[64];

#endif
