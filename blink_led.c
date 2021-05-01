// Copyright (C) 2021 terryspitz@gmail

#define NO_BIT_DEFINES
#include <pic16f88.h>
#include <stdint.h> 

__code uint16_t __at (_CONFIG1) __configword = 
    _INTRC_IO         // internal oscillator, allow IO port
    & _WDTE_OFF     // disable watchdog timer
    & _LVP_OFF         // disable low voltage programming.
    & _MCLRE_OFF;    // disable reset

static void delayTimer1(uint32_t ticks){
    T1CON = 0x00;                   //Timer-1 internal clock, 1:1 prescale
    T1CONbits.T1CKPS1 = 1;          //Timer-1 1:8 prescale
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.TMR1ON = 1;           //Run timer
    for(; ticks>=0x10000; ticks-=0x10000) {
        TMR1H = 0;                  //timer start Hight Byte
        TMR1L = 0;                  //timer start Low Byte
        PIR1bits.TMR1IF = 0;        //Clear overflow flag
        while(PIR1bits.TMR1IF == 0);//Wait for flag to overflow
    }
    T1CONbits.TMR1ON = 0;           //Switch off timer
}

static void delayTimer0(uint8_t ticks){
    OPTION_REGbits.PSA = 1;         //assign prescaler to WDT; Timer0 is 1:1
    OPTION_REGbits.T0CS = 0;        //Timer0 internal timer
    INTCONbits.TMR0IE = 0;          //Timer0 interupt disable
    TMR0 = 0xFF - ticks;            //timer initial value
    INTCONbits.TMR0IF = 0;          //Clear overflow flag
    while(INTCONbits.TMR0IF == 0);  //Wait for flag to overflow
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
    __asm nop __endasm;      // NOP, pause while comparators settle
    OPTION_REGbits.NOT_RBPU = 0; //Enable RB Pull-ups
    LED_TRIS = 0;            // output
    STEP_TRIS = 0;           // output
    SLEEP_TRIS = 0;          // output
    BUTTON_TRIS = 1;         // input
    const uint8_t stepper_ticks = 80;
    const uint16_t steps_per_min_rotation = 20 * 150 /30*31; // 60*2 * 
    uint32_t ticks_per_s = 250000;
    _Bool led = 0;
    _Bool step = 0;
    while (1) {
        SLEEP_PORT = 1;     //enable stepper
        for(uint16_t t = steps_per_min_rotation; t; --t) {
            step = !step;
            STEP_PORT = step;
            delayTimer0(stepper_ticks);
        }
        SLEEP_PORT = 0;     //sleep stepper
        if(!BUTTON_PORT) {
            while(!BUTTON_PORT) {}
            ticks_per_s >>= 1;
        }
        else
        {
            //pause
            for(uint8_t t = 5; t; --t)
                delayTimer1(ticks_per_s);
        }
        led = !led;
        LED_PORT = led;
    }
}
