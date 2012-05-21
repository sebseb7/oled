#include "ssd1351.h"
#include "ssp.h"

static lcdProperties_t ssd1351Properties = { 128, 128 };

/*************************************************/
/* Private Methods                               */
/*************************************************/

#define CMD(c)        do { SET_CS; CLR_CS; CLR_DC; ssd1351SendByte( c, 1 ); CLR_CS; } while (0)
//#define DATA(c)       do { SET_CS; CLR_CS; SET_DC; ssd1351SendByte( c, 0 ); SET_CS; } while (0);
#define DATA(c)       do {  SET_DC; ssd1351SendByte( c, 0 );  } while (0);
#define DELAY(mS)     do { delay_ms(mS); } while(0);

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
	//while ( (LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
	LPC_SSP0->DR = byte;
	while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
	uint8_t Dummy = LPC_SSP0->DR;
 
	//  while ( LPC_SSP0->SR & SSPSR_BSY );
	//SSP_Send( 0, &byte, 1);
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

  	SSP_IOConfig(0);	
  	SSP_Init(0);			

	//Reset the LCD


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
	DATA(0xF0);//was F1
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
	DATA(0xB4);

	// Clear screen
	lcdFillRGB(0,0,0);

	// Turn the display on
	CMD(SSD1351_CMD_SLEEPMODE_DISPLAYON);  
	return;
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

void lcdFillRGB2(uint8_t r,uint8_t g,uint8_t b)
{
	uint8_t i,j;
	//uint8_t x,y;
	ssd1351SetCursor(0, 0);

	uint8_t r_d = (r&7)>>1;
	uint8_t g_d = g&3;
	uint8_t b_d = (b&7)>>1;

	uint8_t r2=0;
	uint8_t r3=0;
	uint8_t r4=0;
	uint8_t g2=0;
	uint8_t g3=0;
	uint8_t g4=0;
	uint8_t b2=0;
	uint8_t b3=0;
	uint8_t b4=0;

	if(r_d > 0)
		r2=8;
	if(r_d > 1)
		r3=8;
	if(r_d > 2)
		r4=8;
	if(g_d > 0)
		g2=4;
	if(g_d > 1)
		g3=4;
	if(g_d > 2)
		g4=4;
	if(b_d > 0)
		b2=8;
	if(b_d > 1)
		b3=8;
	if(b_d > 2)
		b4=8;





	uint8_t data1 = (r&0xF8) | (g>>5);
	uint8_t data2 = (b>>3) | ((g>>2)<<5);
	uint8_t data3 = ((r+r2)&0xF8) | ((g+g2)>>5);
	uint8_t data4 = ((b+b2)>>3) | (((g+g2)>>2)<<5);
	uint8_t data5 = ((r+r3)&0xF8) | ((g+g3)>>5);
	uint8_t data6 = ((b+b3)>>3) | (((g+g3)>>2)<<5);
	uint8_t data7 = ((r+r4)&0xF8) | ((g+g4)>>5);
	uint8_t data8 = ((b+b4)>>3) | (((g+g4)>>2)<<5);



	/*	for (x=1; x<=ssd1351Properties.width ;x++)
		{
		for (y=1; y<= ssd1351Properties.height;y++)
		{
		DATA(data1);
		DATA(data2);
		}
		}*/

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
			DATA(data5);
			DATA(data6);
			DATA(data7);
			DATA(data8);
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

