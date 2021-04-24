// Copyright (C) 2014 Diego Herranz

#define NO_BIT_DEFINES
#include <pic16f88.h>
#include <stdint.h> 

// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN),
// disable watchdog,
// and disable low voltage programming.
// The rest of fuses are left as default.
__code uint16_t __at (_CONFIG1) __configword = _INTRC_IO & _WDTE_OFF & _LVP_OFF & _MCLRE_OFF;

#define LED_TRIS TRISAbits.TRISA2
#define LED_PORT PORTAbits.RA2
#define LED2_TRIS TRISAbits.TRISA1
#define LED2_PORT PORTAbits.RA1


static void Timer1Delay(void){     //65ms delay
    T1CON=0x00;             //Timer-1 internal clock, 1:1 prescale
    TMR1H=0x00;             //Count Hight Byte
    TMR1L=0x00;             //Count Low Byte
	PIR1bits.TMR1IF = 0;	//Clear flag
    T1CONbits.TMR1ON=1;              //Run timer
    while(PIR1bits.TMR1IF==0);       //Wait for flag to over flow
    T1CONbits.TMR1ON=0;              //Switch off timer
}

static void delay(uint16_t ms)
{
	for (uint16_t i = ms/65; i>0; --i) {
		// for (char j = 250; j > 0; --j) {
		// 	__asm nop __endasm;  // Prevent this loop from being optimized away.
		// }
		Timer1Delay();
	}
}

void main(void)
{
	CMCON=7;         // Comparators off, all pins digital I/O 
	LED_TRIS = 0; // Pin as output
	LED_PORT = 1; // LED
	LED2_TRIS = 0; // Pin as output
	LED2_PORT = 1; // LED 

	delay(500);
	LED2_PORT = 0; // LED 

	delay(200);
	LED2_PORT = 1; // LED 

	delay(200);
	LED2_PORT = 0; // LED 

	while (1) {
		delay(100);
		// LED_PORT = 0;
		LED2_PORT = 1;
		delay(100);
		// LED_PORT = 1;
		LED2_PORT = 0;
		// __asm CLRWDT __endasm;  //clear watchdog timer, or disable in config above
	}
}
