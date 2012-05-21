#include "main.h"
#include "stdint.h"

//http://www.nxp.com/documents/user_manual/UM10375.pdf

//http://www.sparkfun.com/datasheets/LCD/NL128128C-EIF-2.pdf



uint32_t msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
}

void delay_ms(uint32_t ms) {
	uint32_t now = msTicks;
	while ((msTicks-now) < ms);
}

int main(void) {
	
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);


	//configure all connections to the oled as output
	LPC_GPIO0->DIR |= (1<<6)|(1<<7)|(1<<8)|(1<<11);				// D7  D6  D2  CS	
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

	lcdInit();
	while (1)
	{
		lcdFillRGB(0,200,0);
		delay_ms(100);

	}
}

