#include "ssd1351.h"
//#include "ssp.h"

static lcdProperties_t ssd1351Properties = { 128, 128 };

/*************************************************/
/* Private Methods                               */
/*************************************************/

#define CMD(c)        do {  CLR_DC; ssd1351SendByte( c, 1 );  } while (0)
#define DATA(c)       do {  SET_DC; ssd1351SendByte( c, 0 );  } while (0);
#define DELAY(mS)     do { delay_ms(mS); } while(0);


uint16_t port2_table[256] = 
{
	0,32,2048,2080,4,36,2052,2084,1024,1056,3072,3104,1028,1060,3076,
	3108,512,544,2560,2592,516,548,2564,2596,1536,1568,3584,3616,1540,
	1572,3588,3620,16,48,2064,2096,20,52,2068,2100,1040,1072,3088,3120,
	1044,1076,3092,3124,528,560,2576,2608,532,564,2580,2612,1552,1584,
	3600,3632,1556,1588,3604,3636,8,40,2056,2088,12,44,2060,2092,1032,
	1064,3080,3112,1036,1068,3084,3116,520,552,2568,2600,524,556,2572,
	2604,1544,1576,3592,3624,1548,1580,3596,3628,24,56,2072,2104,28,60,
	2076,2108,1048,1080,3096,3128,1052,1084,3100,3132,536,568,2584,2616,
	540,572,2588,2620,1560,1592,3608,3640,1564,1596,3612,3644,256,288,
	2304,2336,260,292,2308,2340,1280,1312,3328,3360,1284,1316,3332,3364,
	768,800,2816,2848,772,804,2820,2852,1792,1824,3840,3872,1796,1828,
	3844,3876,272,304,2320,2352,276,308,2324,2356,1296,1328,3344,3376,
	1300,1332,3348,3380,784,816,2832,2864,788,820,2836,2868,1808,1840,
	3856,3888,1812,1844,3860,3892,264,296,2312,2344,268,300,2316,2348,
	1288,1320,3336,3368,1292,1324,3340,3372,776,808,2824,2856,780,812,
	2828,2860,1800,1832,3848,3880,1804,1836,3852,3884,280,312,2328,2360,
	284,316,2332,2364,1304,1336,3352,3384,1308,1340,3356,3388,792,824,
	2840,2872,796,828,2844,2876,1816,1848,3864,3896,1820,1852,3868,3900
};



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
//	LPC_SSP0->DR = byte;
//	while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
//	uint8_t Dummy = LPC_SSP0->DR;
#ifndef I6800
 
  int8_t i;

  // Make sure clock pin starts high
  SET_SCK;

#if defined SSD1351_BUS_SPI3
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
#endif

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
	//  while ( LPC_SSP0->SR & SSPSR_BSY );
	//SSP_Send( 0, &byte, 1);
#endif

#ifdef I6800

	SET_E;

	LPC_GPIO2->DATA = port2_table[byte];

	__ASM volatile ("nop");

	CLR_E;

#endif

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


	//Reset the LCD


	SET_RST;
	DELAY(20);
	CLR_RST;
	DELAY(200);
	SET_RST;
	DELAY(20);

	
	
	CMD(SSD1351_CMD_SETCOMMANDLOCK);
	DATA(0x12);                               // Unlocked to enter commands
	CMD(SSD1351_CMD_SETCOMMANDLOCK);
	DATA(0xB1);                               // Make all commands accessible 
	CMD(SSD1351_CMD_SLEEPMODE_DISPLAYOFF);
	CMD(SSD1351_CMD_SETFRONTCLOCKDIV);
	DATA(0xf0);//was F1
	CMD(SSD1351_CMD_SETMUXRRATIO);
	DATA(0x7f);//7f
	CMD(SSD1351_CMD_COLORDEPTH);

#ifdef C262K
	DATA(0xB4);
#endif
#ifndef C262K
	DATA(0x74);                               // Bit 7:6 = 65,536 Colors, Bit 3 = BGR or RGB
#endif
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
	CMD(SSD1351_CMD_ENHANCEDDRIVINGSCHEME);
	DATA(0xA4);
	DATA(0x00);
	DATA(0x00);
	CMD(SSD1351_CMD_SETSEGMENTLOWVOLTAGE);
	DATA(0xA0);                               // Enable External VSL
	DATA(0xB5);
	DATA(0x55);
	CMD(SSD1351_CMD_SETPRECHARGEVOLTAGE);
	DATA(0x17);
	CMD(SSD1351_CMD_SETVCOMHVOLTAGE);
	DATA(0x05);
	CMD(SSD1351_CMD_SETCONTRAST);
	DATA(0xa0);
	DATA(0xc8);
	DATA(0xb0);
	CMD(SSD1351_CMD_MASTERCONTRAST);
	DATA(0x0F);                               // Maximum contrast
	CMD(SSD1351_CMD_SETSECONDPRECHARGEPERIOD);
	DATA(0x01);//was 0x01
	CMD(SSD1351_CMD_SETDISPLAYMODE_RESET);


	// Use default grayscale for now to save flash space (1k), but here are
	// the values if someone wants to change them ...
/*	CMD(SSD1351_CMD_GRAYSCALELOOKUP);

DATA(0x02);	 // Gray Scale Level 1
DATA(0x03);	 // Gray Scale Level 2
DATA(0x04);	 // Gray Scale Level 3
DATA(0x05);	 // Gray Scale Level 4

DATA(0x06);	 // Gray Scale Level 5
DATA(0x07);	 // Gray Scale Level 6
DATA(0x08);	 // Gray Scale Level 7
DATA(0x09);	 // Gray Scale Level 8

DATA(0x0A);	 // Gray Scale Level 9
DATA(0x0B);	 // Gray Scale Level 10
DATA(0x0C);	 // Gray Scale Level 11
DATA(0x0D);	 // Gray Scale Level 12

DATA(0x0E);	 // Gray Scale Level 13
DATA(0x0F);	 // Gray Scale Level 14

DATA(0x10);	 // Gray Scale Level 15
DATA(0x11);	 // Gray Scale Level 16
DATA(0x12);	 // Gray Scale Level 17
DATA(0x13);	 // Gray Scale Level 18
DATA(0x15);	 // Gray Scale Level 19
DATA(0x17);	 // Gray Scale Level 20
DATA(0x19);	 // Gray Scale Level 21
DATA(0x1B);	 // Gray Scale Level 22
DATA(0x1D);	 // Gray Scale Level 23
DATA(0x1F);	 // Gray Scale Level 24
DATA(0x21);	 // Gray Scale Level 25
DATA(0x23);	 // Gray Scale Level 26
DATA(0x25);	 // Gray Scale Level 27
DATA(0x27);	 // Gray Scale Level 28
DATA(0x2A);	 // Gray Scale Level 29
DATA(0x2D);	 // Gray Scale Level 30
DATA(0x30);	 // Gray Scale Level 31
DATA(0x33);	 // Gray Scale Level 32
DATA(0x36);	 // Gray Scale Level 33
DATA(0x39);	 // Gray Scale Level 34
DATA(0x3C);	 // Gray Scale Level 35
DATA(0x3F);	 // Gray Scale Level 36
DATA(0x42);	 // Gray Scale Level 37
DATA(0x45);	 // Gray Scale Level 38
DATA(0x48);	 // Gray Scale Level 39
DATA(0x4C);	 // Gray Scale Level 40
DATA(0x50);	 // Gray Scale Level 41
DATA(0x54);	 // Gray Scale Level 42
DATA(0x58);	 // Gray Scale Level 43
DATA(0x5C);	 // Gray Scale Level 44
DATA(0x60);	 // Gray Scale Level 45
DATA(0x64);	 // Gray Scale Level 46
DATA(0x68);	 // Gray Scale Level 47
DATA(0x6C);	 // Gray Scale Level 48
DATA(0x70);	 // Gray Scale Level 49
DATA(0x74);	 // Gray Scale Level 50
DATA(0x78);	 // Gray Scale Level 51
DATA(0x7D);	 // Gray Scale Level 52
DATA(0x82);	 // Gray Scale Level 53
DATA(0x87);	 // Gray Scale Level 54
DATA(0x8C);	 // Gray Scale Level 55
DATA(0x91);	 // Gray Scale Level 56
DATA(0x96);	 // Gray Scale Level 57
DATA(0x9B);	 // Gray Scale Level 58
DATA(0xA0);	 // Gray Scale Level 59
DATA(0xA5);	 // Gray Scale Level 60
DATA(0xAA);	 // Gray Scale Level 61
DATA(0xAF);	 // Gray Scale Level 62
DATA(0xB4);	 // Gray Scale Level 63
*/
/*

	DATA(0x00);
	DATA(0x01);
	DATA(0x02);
	DATA(0x03);
	DATA(0x04);
	DATA(0x05);
	DATA(0x06);
	DATA(0x07);
	DATA(0x08);
	DATA(0x09);
	DATA(0x0a);
	DATA(0x0b);
	DATA(0x0c);
	DATA(0x0d);
	DATA(0x0e);
	DATA(0x0f);
	DATA(0x10);
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
*/
	// Clear screen
	lcdFillRGB(0,0,0);

	// Turn the display on
	CMD(SSD1351_CMD_SLEEPMODE_DISPLAYON);  
	return;
}


void lcdFillRGBd(uint8_t r,uint8_t g,uint8_t b)
{
	uint8_t i,j;

	ssd1351SetCursor(0, 0);

#ifdef C262K
	uint8_t data1 = r>>2;
	uint8_t data2 = g>>2;
	uint8_t data3 = b>>2;
	uint8_t data4 = data1;
	uint8_t data5 = data2;
	uint8_t data6 = data3;
	uint8_t data7 = data1;
	uint8_t data8 = data2;
	uint8_t data9 = data3;
	uint8_t data10 = data1;
	uint8_t data11 = data2;
	uint8_t data12 = data3;
#endif

	if(r < 252)
	{
		if((r & 3) > 0)
		{
			data10++;
		}
		if((r & 3) > 1)
		{
			data1++;
		}
		if((r & 3) > 2)
		{
			data7++;
		}
	}
	if(g < 252)
	{
		if((g & 3) > 0)
		{
			data11++;
		}
		if((g & 3) > 1)
		{
			data2++;
		}
		if((g & 3) > 2)
		{
			data8++;
		}
	}
	if(b < 252)
	{
		if((b & 3) > 0)
		{
			data12++;
		}
		if((b & 3) > 1)
		{
			data3++;
		}
		if((b & 3) > 2)
		{
			data9++;
		}
	}

		
		
	for (i=1; i<=64 ;i++)
	{
		for (j=1; j<=64 ;j++)
		{
#ifdef C262K
			DATA(data1);
			DATA(data2);
			DATA(data3);
			DATA(data4);
			DATA(data5);
			DATA(data6);
#endif
#ifndef C262K
			DATA(data1);
			DATA(data2);
			DATA(data3);
			DATA(data4);
#endif
		}
		for (j=1; j<=64 ;j++)
		{
#ifdef C262K
			DATA(data7);
			DATA(data8);
			DATA(data9);
			DATA(data10);
			DATA(data11);
			DATA(data12);
#endif
#ifndef C262K
			DATA(data5);
			DATA(data6);
			DATA(data7);
			DATA(data8);
#endif
		}
	}
	
}
void lcdFillRGB(uint8_t r,uint8_t g,uint8_t b)
{
	uint8_t x,y;
	ssd1351SetCursor(0, 0);

#ifdef C262K

	

	uint16_t port_r = port2_table[r>>2];
	uint16_t port_g = port2_table[g>>2];
	uint16_t port_b = port2_table[b>>2];


	SET_DC;	
	for (x=1; x<=ssd1351Properties.width ;x++)
	{
		for (y=1; y<= ssd1351Properties.height;y++)
		{
			SET_E;
			LPC_GPIO2->DATA = port_r;
			__ASM volatile ("nop");
			CLR_E;
			SET_E;
			LPC_GPIO2->DATA = port_g;
			__ASM volatile ("nop");
			CLR_E;
			SET_E;
			LPC_GPIO2->DATA = port_b;
			__ASM volatile ("nop");
			CLR_E;
		}
	}
#endif
#ifndef C262K
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
#endif
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

#ifdef C262K
	DATA((r&0xFC)>>2);
	DATA((g&0xFC)>>2);
	DATA((b&0xFC)>>2);
#endif
#ifndef C262K
	DATA( (r&0xF8) | (g>>5) );
	DATA( (b>>3) | ((g>>2)<<5) );
#endif
}

