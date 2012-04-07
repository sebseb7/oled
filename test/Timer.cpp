/*	Timer.cpp
*
*	Timer for ATmega644 running at 8 MHz
*	Uses Timer2
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Util.h"
#include "Timer.h"

/*	Timer settings
*
*	WGM (waveform generation mode) = CTC (Clear Timer on Compare Match)
*	CS (clock select): choose prescale to result in 125 kHz (8 Mhz / 64)
*	OCR (top/clear value) = 125 for match every 1 ms
*/

/* this counts the number of milliseconds since Timer/Counter
   was initialized (volatile to make sure loops work!) */
static volatile ulong s_msTimer = 0;

ulong MsCur();

/* sets up Timer/Counter */
void InitTimer()
{
	/* start counting at 0 */
	TCNT2 = 0;

	TCCR2A = 0x02;	// 00 00 xx 10 (COMA, COMB, low 2 bits WGM)
	TCCR2B = 0x04;	// 00 xx 0 100 (misc, high bit WGM, CS: 8 MHz / 64 = 125 kHz)
	OCR2A = 125;	// number of ticks in 1 ms

	/* set Compare Match Interrupt Enable */
	TIMSK2 |= (1 << OCIE2A);

	/* set I-bit in SREG (global enable for interrupts) */
	sei();
}

/* waits (pauses) for the specified number of milliseconds */
void WaitMs(uint ms)
{
	ulong msStart;

	msStart = MsCur();
	while (MsCur() - msStart <= ms)
		;
}

/* returns number of ms since Timer initialized */
ulong MsCur()
{
	char sregSav;
	ulong ms;

	/* disable (delay) interrupts while we read
	   s_msTimer since it's 4 bytes and we don't
	   want it changing during the read! */
	sregSav = SREG;	// cache register holding Global Interrupt Flag
	cli();			// clear Global Interrupt Flag
	ms = s_msTimer;
	SREG = sregSav;	// restore register holding Global Interrupt Flag

	return ms;
}

/* this interrupt happens every ms, when the counter (TCNTn)
   reaches the specified OCRn value */
ISR(TIMER2_COMPA_vect)
{
	++s_msTimer;
}
