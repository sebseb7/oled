/*	Comm.h
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

#pragma once

void InitComm();
void Print(const char *psz);
void PrintLn(const char *psz);	// output text followed by CRLF
