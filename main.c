

#include "main.h"
#include "ssd1351.h"
#include "circle.h"
#include "text.h"
#include "usart.h"
#include "avr/interrupt.h"
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
	USART0_Init();

	sei();

	lcdInit();

	uint8_t data = 0;
				
				lcdFillRGB(0,255,0);

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	while(1)
	{
		if(USART0_Getc_nb(&data))
		{
			if(data == 49)
			{
				r+=4;
				USART0_putc(82);
				USART0_putc(((r-(r%100))/100)+48);
				USART0_putc((((r-(r%10))/10)%10)+48);
				USART0_putc((r%10)+48);
				USART0_putc(10);
			}
			if(data == 50)
			{
				r-=4;
				USART0_putc(82);
				USART0_putc(((r-(r%100))/100)+48);
				USART0_putc((((r-(r%10))/10)%10)+48);
				USART0_putc((r%10)+48);
				USART0_putc(10);
			}
			if(data == 51)
			{
				g+=4;
				USART0_putc(71);
				USART0_putc(((g-(g%100))/100)+48);
				USART0_putc((((g-(g%10))/10)%10)+48);
				USART0_putc((g%10)+48);
				USART0_putc(13);
			}
			if(data == 52)
			{
				g-=4;
				USART0_putc(71);
				USART0_putc(((g-(g%100))/100)+48);
				USART0_putc((((g-(g%10))/10)%10)+48);
				USART0_putc((g%10)+48);
				USART0_putc(13);
			}
			if(data == 53)
			{
				b+=4;
				USART0_putc(66);
				USART0_putc(((b-(b%100))/100)+48);
				USART0_putc((((b-(b%10))/10)%10)+48);
				USART0_putc((b%10)+48);
				USART0_putc(13);
			}
			if(data == 54)
			{
				b-=4;
				USART0_putc(66);
				USART0_putc(((b-(b%100))/100)+48);
				USART0_putc((((b-(b%10))/10)%10)+48);
				USART0_putc((b%10)+48);
				USART0_putc(13);
			}
				lcdFillRGB(r,g,b);
		}
	}

/*
	while(1)
		{
			setLedXY(0,0,255,255,255);
			for(uint8_t i = 0;i<127;i++)
			{
				setLedXY(i+1,0,i*2,0,0);
				setLedXY(i+1,1,i*2,0,0);
				setLedXY(i+1,2,i*2,0,0);
				setLedXY(i+1,3,i*2,0,0);
				
				setLedXY(i+1,4,0,i*2,0);
				setLedXY(i+1,5,0,i*2,0);
				setLedXY(i+1,6,0,i*2,0);
				setLedXY(i+1,7,0,i*2,0);
				
				setLedXY(i+1,8,0,0,i*2);
				setLedXY(i+1,9,0,0,i*2);
				setLedXY(i+1,10,0,0,i*2);
				setLedXY(i+1,11,0,0,i*2);
			}
		}*/
			
	


		/*		while(1)
				{
				lcdFillRGB(255,0,0);_delay_ms(2000);
				lcdFillRGB(24,0,0);_delay_ms(2000);
				lcdFillRGB(0,0,23);_delay_ms(2000);
				lcdFillRGB(0,15,0);_delay_ms(2000);
				lcdFillRGB(0,255,0);_delay_ms(2000);
				lcdFillRGB(0,0,0);_delay_ms(2000);
				
				}*//*_delay_ms(1000);
				lcdFillRGBdither(0,0,0,0,68,0,1);_delay_ms(1000);
				lcdFillRGBdither(0,0,0,0,68,0,2);_delay_ms(1000);
				lcdFillRGBdither(0,0,0,0,68,0,3);_delay_ms(1000);
				lcdFillRGB(0,68,0);_delay_ms(1000);
				lcdFillRGBdither(0,68,0,0,0,0,1);_delay_ms(1000);
				lcdFillRGBdither(0,68,0,0,0,0,2);_delay_ms(1000);
				lcdFillRGBdither(0,68,0,0,0,0,3);_delay_ms(1000);
	}*/
/*	while(1)
	{
				lcdFillRGB(200,0,0);
				lcdFillRGB(0,200,0);
				lcdFillRGB(0,0,200);
	}*/


	while(1)
	{
		for(uint8_t x = 0;x < 255;x++)
		{
				draw_filledCircle(64,64,28,x,254-(x),0);
				draw_text(5,5,"<<</>>",255,0,0);
				draw_text(90,5,"chaos",0,255,0);
				draw_text(5,110,"23",0,0,255);
				draw_text(90,110,"42",255,255,0);
		}
		for(uint8_t x = 0;x < 128;x+=32)
		{
			//16 fps
			for(uint8_t y = 0; y < 128; y+=2)
			{
				lcdFillRGB(0,y*2,x*2);
				lcdFillRGBdither(0,y*2,x*2,0,(y+1)*2,x*2,1);
				lcdFillRGBdither(0,y*2,x*2,0,(y+1)*2,x*2,2);
				lcdFillRGBdither(0,y*2,x*2,0,(y+1)*2,x*2,3);
				lcdFillRGB(0,(y+1)*2,x*2);
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



