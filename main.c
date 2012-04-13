#include <inttypes.h>
#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>


#include "main.h"
#include "ssd1351.h"
#include "circle.h"
#include "text.h"
/*
 *
 * http://arduino.cc/en/Main/ArduinoBoardNano
 *
 *
 * http://www.sparkfun.com/datasheets/LCD/oled_carrier_datasheet.pdf
 * http://www.sparkfun.com/datasheets/LCD/OLED-CarrierBoard-v24.pdf
 *
 * http://www.sparkfun.com/datasheets/LCD/SSD1339_1.pdf
 *
 *
 * d0 =CLK
 * d1 =MOSI
 *
 * The serial interface consists of serial clock SCLK, serial data SDIN, D/C#, CS#. In this mode, D0 acts as SCLK, D1 acts as SDIN. For the unused data pins, D2 should be left open. D3 to D7, E and R/W pins can be connected to external ground.
 *
 *During data writing, an additional NOP command should be inserted before the CS# goes high (Refer to Figure 7. 
 *(nop = DC:low
 *
 */


// http://www.mikrocontroller.net/attachment/86536/ODM100.c
// http://forum.sparkfun.com/viewtopic.php?f=7&t=24238



// :::!! http://www.sparkfun.com/datasheets/LCD/NL128128C-EIF-2.pdf
// :::!! http://aitendo2.sakura.ne.jp/aitendo_data/product_img2/product_img/oled/UG-2828GDEDF11/SSD1351.pdf

int main(void)
{

	lcdInit();
	
/*	while(1)
	{
				lcdFillRGB(200,0,0);
				lcdFillRGB(0,200,0);
				lcdFillRGB(0,0,200);
	}*/


	while(1)
	{
		lcdFillRGB(0,0,0);
		for(uint8_t x = 0;x < 32;x++)
		{
				draw_filledCircle(64,64,28,x<<3,254-(x<<3),0);
				draw_text(5,5,"<<</>>",255,0,0);
				draw_text(90,5,"chaos",0,255,0);
				draw_text(5,110,"23",0,0,255);
				draw_text(90,110,"42",255,255,0);
		}
		for(uint8_t x = 0;x < 128;x+=8)
		{
			for(uint8_t y = 0; y < 128; y+=8)
			{
				lcdFillRGB(0,y*2,x*2);
				//fi
			//setLedXY(x,y,x*2,y*2,0);
			}
		}
		for(uint8_t x = 0;x < 128;x++)
		{
			for(uint8_t y = 0; y < 128; y++)
			{
				setLedXY(x,y,0,x*2,y*2);
			}
		}
		
	}
}



