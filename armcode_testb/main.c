#include "main.h"
#include "stdint.h"

//http://www.nxp.com/documents/user_manual/UM10375.pdf
#include "plasma.h"
#include "circle.h"
#include "wuline.h"
#include "text.h"
  
  
  #include "usb/usb.h"
  #include "usb/usbcore.h"
  #include "usb/usbhw.h"
  #include "usb/cdcuser.h"
  #include "usb/cdc_buf.h"
#define UART_RXBUFSIZE 25

volatile static uint8_t rxbuf0[UART_RXBUFSIZE];
volatile static uint8_t *volatile rxhead0, *volatile rxtail0;

volatile uint32_t delay_ticks = 0;
// found it in the internet...
static unsigned int my_rand(void) {
    static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
    unsigned int b;
    b  = ((z1 << 6) ^ z1) >> 13;
    z1 = ((z1 & 4294967294U) << 18) ^ b;
    b  = ((z2 << 2) ^ z2) >> 27;
    z2 = ((z2 & 4294967288U) << 2) ^ b;
    b  = ((z3 << 13) ^ z3) >> 21;
    z3 = ((z3 & 4294967280U) << 7) ^ b;
    b  = ((z4 << 3) ^ z4) >> 12;
    z4 = ((z4 & 4294967168U) << 13) ^ b;
    return (z1 ^ z2 ^ z3 ^ z4);
}

unsigned int rand_int(unsigned int limit) {
    return my_rand() % limit;
}
uint16_t randr(uint16_t start,uint16_t end)
{
	return start + rand_int(0xffff) % ( end - start );
}

void SysTick_Handler(void) {
	if(delay_ticks > 0) delay_ticks--;
}

void delay_ms(uint32_t ms) {
	delay_ticks = ms;
	while (delay_ticks > 0);
}

uint8_t getUsbChar(uint8_t *c)
{
    if (rxhead0==rxtail0) return 0;
    *c = *rxtail0;
    if (++rxtail0 == (rxbuf0 + UART_RXBUFSIZE)) rxtail0 = rxbuf0;

    return 1;
}

int main(void) {
	
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
  
  	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

    CDC_Init();                     // Initialise VCOM
    USB_Init();                     // USB Initialization
    USB_Connect(1);              // USB Connect
    // Wait until USB is configured or timeout occurs
    uint32_t usbTimeout = 0; 
    while ( usbTimeout < CFG_USBCDC_INITTIMEOUT / 10 )
    {
      if (USB_Configuration) break;
      delay_ms(10);             // Wait 10ms
    usbTimeout++;
    }

	//configure all connections to the oled as output
	LPC_GPIO0->DIR |= (1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10);				//  SCK SID CS DC RES
	LPC_GPIO1->DIR |= (1<<0)|(1<<2);				// LED1 LED2
	
	//set all pins that do not default to GPIO to GPIO
	LPC_IOCON->SWCLK_PIO0_10 |= 1;        
	LPC_IOCON->R_PIO1_0 |= 1;
	LPC_IOCON->R_PIO1_2 |= 1;

#ifdef I6800
	LPC_GPIO2->DIR |= (1<<8)|(1<<3)|(1<<6);  // E	D2 D7
	LPC_GPIO1->DIR |= (1<<3)|(1<<4)|(1<<5);  // D3 D4 D5
	LPC_GPIO3->DIR |= (1<<3);  // D3 D4 D6
	LPC_IOCON->SWDIO_PIO1_3 |= 1;        
#endif

	
/*	
	LPC_GPIO0->DIR |= (1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10)|(1<<11);				// D7  D6  D2  D1 D0 CS	
	LPC_GPIO1->DIR |= (1<<0)|(1<<1)|(1<<2)|(1<<10);				// D/C RES BS1 E	
	LPC_GPIO2->DIR |= (1<<2)|(1<<5)|(1<<9)|(1<<10)|(1<<11);		// D3  D8  D5  D4  R/W
	LPC_GPIO3->DIR |= (1<<0);									// BS2
	
	//set all pins that do not default to GPIO to GPIO
	LPC_IOCON->SWCLK_PIO0_10 |= 1;        
	LPC_IOCON->R_PIO0_11 |= 1;
	LPC_IOCON->R_PIO1_0 |= 1;
	LPC_IOCON->R_PIO1_1 |= 1;
	LPC_IOCON->R_PIO1_2 |= 1;

	// BS1 & BS2 to GND
	LPC_GPIO1->DATA &= ~(1<<2);
	LPC_GPIO3->DATA &= ~(1<<0);


	//set E to GND
	LPC_GPIO1->DATA &= ~(1<<10);
	//set R/W to GND
	LPC_GPIO2->DATA &= ~(1<<11);

	//set D3 to D8 to GND
	LPC_GPIO2->DATA &= ~(1<<2);
	LPC_GPIO2->DATA &= ~(1<<10);
	LPC_GPIO2->DATA &= ~(1<<9);
	LPC_GPIO0->DATA &= ~(1<<7);
	LPC_GPIO0->DATA &= ~(1<<6);
	LPC_GPIO2->DATA &= ~(1<<5);
*/	

	//uint32_t x = 1;
	//uint32_t y = 1;

	delay_ms(10);
	lcdInit();
	delay_ms(10);


/*	while(1)
	{
		tick();
	}
*/


/* 
				  
	while(1)
	{
		int numBytesToRead, numBytesRead, numAvailByte;
		static char serBuf [32];
		CDC_OutBufAvailChar (&numAvailByte);
		if (numAvailByte > 0) {
			numBytesToRead = numAvailByte > 32 ? 32 : numAvailByte;
			x += CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
			lcdFillRGB(0,0,0);
			draw_number(50,50, x,3,'0',255-x,255-x,255-x);
		}		  
	}
*/
	uint8_t data = 0;
				
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	uint8_t stage = 10;
	
	rxhead0 = rxtail0 = rxbuf0;

/*if (CDC_DepInEmpty) {
CDC_DepInEmpty = 0;
USB_WriteEP (CDC_DEP_IN, (unsigned char *)&Buff[0], numBytesWright);
}*/

while(1)
	{
		int numAvailByte;
		CDC_OutBufAvailChar (&numAvailByte);
		if (numAvailByte > 0) {
			int numBytesToRead, numBytesRead;
			static char serBuf [32];
			numBytesToRead = numAvailByte > 32 ? 32 : numAvailByte;
			numBytesRead = CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
		
			for(int i = 0;i<numBytesRead;i++)
			{
				int diff;
				uint8_t c;
				c=serBuf[i];
				diff = rxhead0 - rxtail0;
				if (diff < 0) diff += UART_RXBUFSIZE;
				if (diff < UART_RXBUFSIZE -1)
				{
		//			lcdFillRGB(0,0,0);
		//			draw_number(50,50, c,3,'0',255,255,255);
					*rxhead0 = c;
					++rxhead0;
					if (rxhead0 == (rxbuf0 + UART_RXBUFSIZE)) rxhead0 = rxbuf0;

				}

			}
		
		}		  
		if(getUsbChar(&data))
		{
			if(data == 0xff)
			{
				stage = 0;
			}
			if(stage == 1)
			{
				r = data;
			}
			if(stage == 2)
			{
				g = data;
			}
			if(stage == 3)
			{
				b = data;
				lcdFillRGBd(r,g,b);
			}

			if(stage < 10)
				stage++;

		}
	}
/*	x=0;

	for(x=0;x<128;x++)
	{
		for(y=0;y<128;y++)
		{
			if(y < 15)
			{
				setLedXY(x,y,x<<1,0,0);
			}
			else if(y < 30)
			{
				setLedXY(x,y,x<<1,x<<1,0);
			}
			else if(y < 45)
			{
				setLedXY(x,y,0,x<<1,0);
			}
			else if(y < 60)
			{
				setLedXY(x,y,0,x<<1,x<<1);
			}
			else if(y < 75)
			{
				setLedXY(x,y,0,0,x<<1);
			}
			else if(y < 90)
			{
				setLedXY(x,y,x<<1,0,x<<1);
			}
			else if(y < 105)
			{
				setLedXY(x,y,x<<1,0,0);
			}
			else if(y < 115)
			{
				setLedXY(x,y,x<<1,x<<1,x<<1);
			}


		}




	}

	delay_ms(6000);

	while(1)
	{
		//		unsigned char cmd1[] = {1,2,3}; 

		//	int length = 3;


		for(x=0;x<255;x++)
		{
			uint8_t frame[64];
			frame[0]= 'a';
			USB_WriteEP (CDC_DEP_IN, frame, 1);
			//	   		USB_WriteEP (CDC_DEP_IN, (unsigned char *)&cmd1[0], 3);


			lcdFillRGBd(x,x,x);
			//	draw_number(50,50, x,3,'0',255-x,255-x,255-x);
			draw_text(10,10,"jkhkjhkjhkjhkjh",255,255,255);
			delay_ms(100);

		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(x,x,x);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(x,0,0);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(x,0,0);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(x,x,0);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(x,x,0);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(0,x,0);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(0,x,0);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(0,x,x);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(0,x,x);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(0,0,x);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(0,0,x);
			delay_ms(10);
		}
		for(x=0;x<255;x++)
		{
			lcdFillRGBd(x,0,x);
			delay_ms(10);
		}
		for(x=255;x>0;x--)
		{
			lcdFillRGBd(x,0,x);
			delay_ms(10);
		}

		//	draw_filledCircle(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255));
		//draw_filledCircleSlice(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255),randr(0,360),randr(0,360));
		//	draw_line(randr(0,128),randr(0,128),randr(0,128),randr(0,128),randr(0,255),randr(0,255),randr(0,255));
		//	draw_text(randr(0,128),randr(0,128),"hallo",randr(0,255),randr(0,255),randr(0,255));
		//	delay_ms(4200);
		//tick_bigsin();
		//	tick();
	}


	while(1)
	{
		//		LPC_GPIO1->DATA |= (1<<0);
		//		LPC_GPIO1->DATA &= ~(1<<2);
		lcdFillRGB(200,0,0);
		//				delay_ms(500);
		//				lcdFillRGB(0,200,0);
		//				delay_ms(500);
		//		LPC_GPIO1->DATA &= ~(1<<0);
		//		LPC_GPIO1->DATA |= (1<<2);
		lcdFillRGB(0,0,200);
		//				delay_ms(500);
	}

	uint8_t z = 0;
	while (1)
	{	
		setLedXY(x,y,x*2,y*2,z);
		x++;

		if(x==128)
		{
			x=0;
			y++;
		}
		if(y==128)
		{
			y=0;
			//			delay_ms(100);
			z+=4;
			if(z==255)
			{
				z=0;
			}
		}

		//		LPC_GPIO1->DATA |= (1<<0);
		//		LPC_GPIO1->DATA &= ~(1<<2);
		//		delay_ms(100);
		//		setLedXY(x,y,x,y,100);

		//		lcdFillRGB(200,0,0);
		//		LPC_GPIO1->DATA &= ~(1<<0);
		//		LPC_GPIO1->DATA |= (1<<2);

	}*/
}

