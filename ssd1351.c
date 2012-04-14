#include "ssd1351.h"
#include <util/delay.h>

static lcdProperties_t ssd1351Properties = { 128, 128 };

/*************************************************/
/* Private Methods                               */
/*************************************************/

#define CMD(c)        do { SET_CS; CLR_CS; CLR_DC; ssd1351SendByte( c, 1 ); CLR_CS; } while (0)
//#define DATA(c)       do { SET_CS; CLR_CS; SET_DC; ssd1351SendByte( c, 0 ); SET_CS; } while (0);
#define DATA(c)       do {  SET_DC; ssd1351SendByte( c, 0 );  } while (0);
#define DELAY(mS)     do { _delay_ms(mS); } while(0);

/**************************************************************************/
/*! 
    @brief Simulates an SPI write using GPIO.  
    
    @param[in]  byte
                The byte to send
    @param[in]  command
                1 if this is a command, 0 if it is data
*/
/**************************************************************************/
void ssd1351SendByte(uint8_t byte, uint8_t command)
{
  int8_t i;

  // Make sure clock pin starts high
  SET_SCK;
	
	
	SPDR = byte; 
	while(!(SPSR & (1<<SPIF)));

	return;
/*#if defined SSD1351_BUS_SPI3

  // Prepend D/C bit (cmd = 0, data = 1)
  CLR_SCK;
  if (command)
  {
    CLR_SID;
  }
  else
  {
    SET_SID;
  }
  SET_SCK;
#endif*/

  // Write from MSB to LSB
  for (i=7; i>=0; i--) 
  {
    // Set clock pin low
    CLR_SCK;
    // Set data pin high or low depending on the value of the current bit
    if (byte & (1 << i))
    {
      SET_SID;
    }
    else
    {
      CLR_SID;
    }
    // Set clock pin high
    SET_SCK;
  }
}

/**************************************************************************/
/*! 
    @brief  Sets the cursor to the specified X/Y position
*/
/**************************************************************************/
void ssd1351SetCursor(uint8_t x, uint8_t y)
{
  if ((x >= ssd1351Properties.width) || (y >= ssd1351Properties.height))
    return;

  CMD(SSD1351_CMD_WRITERAM);
  CMD(SSD1351_CMD_SETCOLUMNADDRESS);
  DATA(x);                            // Start Address
  DATA(ssd1351Properties.width-1);    // End Address (0x7F)

  CMD(SSD1351_CMD_SETROWADDRESS);
  DATA(y);                            // Start Address
  DATA(ssd1351Properties.height-1);   // End Address (0x7F)
  CMD(SSD1351_CMD_WRITERAM);
}

/*************************************************/
/* Public Methods                                */
/*************************************************/

/**************************************************************************/
/*! 
    @brief  Configures any pins or HW and initialises the LCD controller
*/
/**************************************************************************/
void lcdInit(void)
{
  // Set all pins to output



	//SCK
	DDRB |= (1<<PORTB5);
	//SID
	DDRB |= (1<<PORTB3);
	//RST
	DDRC |= (1<<PORTC2);
	//CS
	DDRC |= (1<<PORTC4);

#if !defined SSD1351_BUS_SPI3
	//DC
	DDRC |= (1<<PORTC3);
#endif


	DDRB |= (1<<PORTB2);
	SPCR |= (1<<SPE)|(1<<MSTR);
	SPSR |= (1<<SPI2X);
  // Reset the LCD
  SET_RST;
  DELAY(20);
  CLR_RST;
  DELAY(200);
  SET_RST;
  DELAY(20);

  // Disable pullups

//  do we need this ? 
//  SSD1351_DISABLEPULLUPS();

  CMD(SSD1351_CMD_SETCOMMANDLOCK);
  DATA(0x12);                               // Unlocked to enter commands
  CMD(SSD1351_CMD_SETCOMMANDLOCK);
  DATA(0xB1);                               // Make all commands accessible 
  CMD(SSD1351_CMD_SLEEPMODE_DISPLAYOFF);
  CMD(SSD1351_CMD_SETFRONTCLOCKDIV);
  DATA(0xF1);//was F1
  CMD(SSD1351_CMD_SETMUXRRATIO);
  DATA(0x7f);//7f
  CMD(SSD1351_CMD_COLORDEPTH);
  DATA(0x74);                               // Bit 7:6 = 65,536 Colors, Bit 3 = BGR or RGB
  CMD(SSD1351_CMD_SETCOLUMNADDRESS);
  DATA(0x00);
  DATA(0x7F);
  CMD(SSD1351_CMD_SETROWADDRESS);
  DATA(0x00);
  DATA(0x7F);
  CMD(SSD1351_CMD_SETDISPLAYSTARTLINE);
  DATA(0x00);
  CMD(SSD1351_CMD_SETDISPLAYOFFSET);
  DATA(0x00);
  CMD(SSD1351_CMD_SETGPIO);
  DATA(0x00);                               // Disable GPIO pins
  CMD(SSD1351_CMD_FUNCTIONSELECTION);
  DATA(0x01);                               // External VDD (0 = External, 1 = Internal)
  CMD(SSD1351_CMD_SETPHASELENGTH);
  DATA(0x32);
  CMD(SSD1351_CMD_SETSEGMENTLOWVOLTAGE);
  DATA(0xA0);                               // Enable External VSL
  DATA(0xB5);
  DATA(0x55);
  CMD(SSD1351_CMD_SETPRECHARGEVOLTAGE);
  DATA(0x17);
  CMD(SSD1351_CMD_SETVCOMHVOLTAGE);
  DATA(0x05);
  CMD(SSD1351_CMD_SETCONTRAST);
  DATA(0xC8);
  DATA(0x80);
  DATA(0xC8);
  CMD(SSD1351_CMD_MASTERCONTRAST);
  DATA(0x0F);                               // Maximum contrast
  CMD(SSD1351_CMD_SETSECONDPRECHARGEPERIOD);
  DATA(0x01);
  CMD(SSD1351_CMD_SETDISPLAYMODE_RESET);

  // Use default grayscale for now to save flash space (1k), but here are
  // the values if someone wants to change them ...
    CMD(SSD1351_CMD_GRAYSCALELOOKUP);
	
/*	for(uint8_t i = 1;i < 64; i++)
	{
	    SET_CS; CLR_CS; SET_DC; ssd1351SendByte( i+12, 0 );SET_CS;
	}
*/
    DATA(0x05);
    DATA(0x06);
    DATA(0x07);
    DATA(0x08);
    DATA(0x09);
    DATA(0x0a);
    DATA(0x0b);
    DATA(0x0c);
    DATA(0x0D);
    DATA(0x0E);
    DATA(0x0F);
    DATA(0x10);
    DATA(0x11);
    DATA(0x12);
    DATA(0x13);
    DATA(0x14);
    DATA(0x15);
    DATA(0x16);
    DATA(0x18);
    DATA(0x1a);
    DATA(0x1b);
    DATA(0x1C);
    DATA(0x1D);
    DATA(0x1F);
    DATA(0x21);
    DATA(0x23);
    DATA(0x25);
    DATA(0x27);
    DATA(0x2A);
    DATA(0x2D);
    DATA(0x30);
    DATA(0x33);
    DATA(0x36);
    DATA(0x39);
    DATA(0x3C);
    DATA(0x3F);
    DATA(0x42);
    DATA(0x45);
    DATA(0x48);
    DATA(0x4C);
    DATA(0x50);
    DATA(0x54);
    DATA(0x58);
    DATA(0x5C);
    DATA(0x60);
    DATA(0x64);
    DATA(0x68);
    DATA(0x6C);
    DATA(0x70);
    DATA(0x74);
    DATA(0x78);
    DATA(0x7D);
    DATA(0x82);
    DATA(0x87);
    DATA(0x8C);
    DATA(0x91);
    DATA(0x96);
    DATA(0x9B);
    DATA(0xA0);
    DATA(0xA5);
    DATA(0xAA);
    DATA(0xAF);
    DATA(0xBF);

  // Clear screen
  lcdFillRGB(0,0,0);

  // Turn the display on
  CMD(SSD1351_CMD_SLEEPMODE_DISPLAYON);  
}


/**************************************************************************/
/*! 
    @brief  Fills the LCD with the specified 16-bit color
*/
/**************************************************************************/
void lcdFillRGBdither(uint8_t r,uint8_t g,uint8_t b,uint8_t r2,uint8_t g2,uint8_t b2,uint8_t val)
{
	uint8_t i,j;
	ssd1351SetCursor(0, 0);

	uint8_t data1 = (r&0xF8) | (g>>5);
	uint8_t data2 = (b>>3) | ((g>>2)<<5);
	uint8_t data3 = (r2&0xF8) | (g2>>5);
	uint8_t data4 = (b2>>3) | ((g2>>2)<<5);

	if(val == 1)
	{
		for (i=1; i<=64 ;i++)
		{
			for (j=1; j<=64 ;j++)
			{
		  		DATA(data1);
				DATA(data2);
				DATA(data3);
				DATA(data4);
			}
			for (j=1; j<=64 ;j++)
			{
				DATA(data1);
				DATA(data2);
	  			DATA(data1);
				DATA(data2);
			}
		}
	}
	if(val == 2)
	{
		for (i=1; i<=64 ;i++)
		{
			for (j=1; j<=64 ;j++)
			{
		  		DATA(data1);
				DATA(data2);
				DATA(data3);
				DATA(data4);
			}
			for (j=1; j<=64 ;j++)
			{
				DATA(data3);
				DATA(data4);
	  			DATA(data1);
				DATA(data2);
			}
		}
	}
	if(val == 3)
	{
		for (i=1; i<=64 ;i++)
		{
			for (j=1; j<=64 ;j++)
			{
		  		DATA(data3);
				DATA(data4);
				DATA(data3);
				DATA(data4);
			}
			for (j=1; j<=64 ;j++)
			{
				DATA(data3);
				DATA(data4);
	  			DATA(data1);
				DATA(data2);
			}
		}
	}
}
void lcdFillRGB(uint8_t r,uint8_t g,uint8_t b)
{
	uint8_t x,y;
	ssd1351SetCursor(0, 0);

	uint8_t data1 = (r&0xF8) | (g>>5);
	uint8_t data2 = (b>>3) | ((g>>2)<<5);

	for (x=1; x<=ssd1351Properties.width ;x++)
	{
		for (y=1; y<= ssd1351Properties.height;y++)
		{
  			DATA(data1);
			DATA(data2);
		}
	}
}

/**************************************************************************/
/*! 
    @brief  Draws a single pixel at the specified X/Y location
*/
/**************************************************************************/

void setLedXY(uint8_t x, uint8_t y, uint8_t r,uint8_t g, uint8_t b)
{
  if ((x >= ssd1351Properties.width) || (y >= ssd1351Properties.height))
    return;

  ssd1351SetCursor((uint8_t)x, (uint8_t)y);

  DATA( (r&0xF8) | (g>>5) );
  DATA( (b>>3) | ((g>>2)<<5) );
}

