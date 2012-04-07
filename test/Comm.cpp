/*	Comm.cpp
*
*	Serial communications.
*	- default communication at 8-N-1
*	- baud rate 38.4k
*	- (USART 0)
*	- (look at spec sheet for Rx/Tx pins, e.g. D0/1 on Mega16/32/644)
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Util.h"
#include "Comm.h"

static void TxCh(char ch);

/* initializes serial transmit */
void InitComm()
{
	UCSR0C = 0x86;	// 1000 0110 - asynch, no parity, 1 stop bit, 8 data bits
	UBRR0H = 0;
	UBRR0L = 12;	// 28.4k baud for 8MHz uC clock

	UCSR0B |= (1 << TXEN0);	// enable transmit
}

/* transmits text via serial connection */
void Print(const char *psz)
{
	const char *pch;

	for (pch = psz; *pch != 0; ++pch)
		TxCh(*pch);
}

/* transmits text plus end of line (e.g. CRLF) via serial connection */
void PrintLn(const char *psz)
{
	if (psz != 0)
		Print(psz);
	TxCh('\r');
	TxCh('\n');
}

/* transmits a single character (after waiting
   for any prev transmission to complete) */
static void TxCh(char ch)
{
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;
	UDR0 = ch;
}

