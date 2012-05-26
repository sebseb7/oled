#include "main.h"
#include "stdint.h"

//http://www.nxp.com/documents/user_manual/UM10375.pdf
#include "plasma.h"

volatile uint32_t delay_ticks = 0;

void SysTick_Handler(void) {
	if(delay_ticks > 0) delay_ticks--;
}

void delay_ms(uint32_t ms) {
	delay_ticks = ms;
	while (delay_ticks > 0);
}

int main(void) {
	
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
  
  	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);


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

	uint32_t x = 1;
	uint32_t y = 1;

	delay_ms(10);
	lcdInit();
	delay_ms(10);
	
	while(1)
	{
		tick();
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

	}
}

