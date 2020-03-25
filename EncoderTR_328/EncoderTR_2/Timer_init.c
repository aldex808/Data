/*
 * Timer_init.c
 *
 * Created: 21.01.2018 1:01:29
 *  Author: Администратор2
 */ 
#include "Timer_init.h"
#include "ADC.h"

void Timer_init()
{
	
TCCR1A=0x00;
TCCR1B=0x01;//       CS10=1
//TCNT1=TMR1_CALCULATE;
TCNT1=0xfce0;
//TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization

TIMSK1 |= (1<<TOIE1); // Enable Overflow Interrupt Enable
}