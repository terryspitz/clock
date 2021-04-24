//(c) Terry Spitz 2016
// Drive motor for disc-clock

//compile with 
//set SDCC_HOME=C:\Program Files\SDCC
//sdcc -mpic14 -p16f628a --use-non-free motor.c -oobj\  (need to manually create the subdir)

#include "pic16f628a.h"

unsigned int __at 0x2007 __CONFIG = _INTRC_OSC_NOCLKOUT & _LVP_OFF & _WDT_OFF & _PWRTE_ON & _BODEN_ON;

#define SHORTDELAY __asm nop; nop; __endasm;

// DELAY 250 MSEC 
static void delay()
{
	int loop1, loop2;
	for(loop1=255; loop1!=0; --loop1)
	for(loop2=255; loop2!=0; --loop2)
	{
		SHORTDELAY;
	}
}

void main()
{
	
	// INITIALIZE 
	PORTA=0;
	PORTB=0;

	TRISA=0;           // All pins port A output 
	TRISB=~0;           // All pins port B input 

	// FUNCTION OF PORT A PINS 
	CMCON=7;         // Comparators off, all pins digital I/O 
	
	// MAIN LOOP 
	while(1)
	{
		//PORTA |= 1U<<4;        // Turn on RA4
        PORTA = 255;
		delay();
		delay();
        PORTA = 0;
		//PORTA &= ~(1U<<4);        // Turn off RA4
		delay();
		delay();
	}
}
