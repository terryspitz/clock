// Copyright (C) 2021 terryspitz@gmail

#define NO_BIT_DEFINES
#include <pic16f88.h>
#include <stdint.h> 

__code uint16_t __at (_CONFIG1) __configword = 
    _INTRC_IO         // internal oscillator, allow IO port
    & _WDTE_OFF     // disable watchdog timer
    & _LVP_OFF         // disable low voltage programming.
    & _MCLRE_OFF;    // disable reset

static void delay(uint16_t ticks){  
    T1CON = 0x00;                   //Timer-1 internal clock, 1:1 prescale
    TMR1H = 0xFF - ticks / 256;    	//timer start Hight Byte
    TMR1L = 0xFF - ticks % 256;     //timer start Low Byte
    PIR1bits.TMR1IF = 0;            //Clear overflow flag
    T1CONbits.TMR1ON = 1;           //Run timer
    while(PIR1bits.TMR1IF == 0);    //Wait for flag to overflow
    T1CONbits.TMR1ON = 0;           //Switch off timer
}

#define LED_TRIS TRISAbits.TRISA2
#define LED_PORT PORTAbits.RA2
#define STEP_TRIS TRISAbits.TRISA1
#define STEP_PORT PORTAbits.RA1
#define SLEEP_TRIS TRISAbits.TRISA0
#define SLEEP_PORT PORTAbits.RA0
#define BUTTON_TRIS TRISBbits.TRISB3
#define BUTTON_PORT PORTBbits.RB3

void main(void)
{
    CMCON = 7;               // Comparators off, all pins digital I/O 
    OPTION_REGbits.NOT_RBPU = 0; //Enable RB Pull-ups
    __asm nop __endasm;      // NOP, pause while comparators settle
    LED_TRIS = 0;            // output
    STEP_TRIS = 0;           // output
    SLEEP_TRIS = 0;          // output
    BUTTON_TRIS = 1;         // input
	const uint16_t delay_ticks = 80;
	const uint16_t ticks_per_min_rotation = 20 * 150 /30*31; // 60*2 * 
	uint16_t ticks_per_s = 8000;
	_Bool led = 0;
	_Bool step = 0;
    while (1) {
		// SLEEP_PORT = 1;	 //enable stepper
		// for(uint16_t t = ticks_per_min_rotation; t; --t) {
		// 	step = !step;
		// 	STEP_PORT = step;
		// 	delay(delay_ticks);
		// }
		SLEEP_PORT = 0;	 //sleep stepper
        if(!BUTTON_PORT) {
            while(!BUTTON_PORT) {}
			ticks_per_s /= 2;
        }
		else
		{
			//pause
			delay(ticks_per_s/2);
		}
		led = !led;
        LED_PORT = led;
    }
}
