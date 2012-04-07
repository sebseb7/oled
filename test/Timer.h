/*	Timer.h
*
*	Timer for ATmega644 running at 8 MHz
*	Uses Timer2
*
*	Written by Cathy Saxton
*	robotics@idleloop.com
*/

#pragma once

void InitTimer();
void WaitMs(uint ms);
