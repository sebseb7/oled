#include "main.h"
#include "stdint.h"
#include "plasma.h"

//http://www.nxp.com/documents/user_manual/UM10375.pdf

//http://www.sparkfun.com/datasheets/LCD/NL128128C-EIF-2.pdf



volatile uint32_t msTicks = 0;

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
  
  	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);


	//configure all connections to the oled as output
	LPC_GPIO0->DIR |= (1<<7)|(1<<11);				// RES E
	LPC_GPIO1->DIR |= (1<<7)|(1<<10);				// REG_EN R/W
	LPC_GPIO2->DIR |= (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<8)|(1<<9)|(1<<10)|(1<<11);		// D2 D6 D5 D0 D8 D7 D4 D3 D1
	LPC_GPIO3->DIR |= (1<<0);									// D/C
	
	//set all pins that do not default to GPIO to GPIO
	LPC_IOCON->R_PIO0_11 |= 1;

	//set REG_EN to VCC
	LPC_GPIO1->DATA |= (1<<7);
	
// 0 i i i 1 i i i i n 0 1 1 0 i 1 0

	//set D8 to GND
	LPC_GPIO2->DATA &= ~(1<<6);
	//set E to GND
	LPC_GPIO0->DATA &= ~(1<<11);
	//set R/W to GND
	LPC_GPIO1->DATA &= ~(1<<10);

		delay_ms(50);
		lcdInit();
		delay_ms(50);
	while (1)
	{
		
		lcdFillRGB(0,255,0);
	//	LPC_GPIO2->DATA |= (1<<5);
	//	LPC_GPIO1->DATA |= (1<<7);
		delay_ms(50);
		lcdFillRGB(255,0,0);
	//	LPC_GPIO2->DATA &= ~(1<<5);
	//	LPC_GPIO1->DATA &= ~(1<<7);
		delay_ms(50);
	//	tick();
	}
}

