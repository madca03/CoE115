/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 12, 2017, 9:02 PM
 */


#include "xc.h"
#include <math.h>

// CONFIG1
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)
#pragma config ICS = PGx2   // Comm Channel Select (Emulator EMUC2/EMUD2 pins are shared with PGC2/PGD2)

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

unsigned int i = 0;
int change_duty = 0;
unsigned int new_duty;
int pwm_period = 0x009F;
int min_duty = 0x0007;

void ADC_init(void);
void PWM_init(void);
void PWM_pin_setup(void);
void port_init(void);



int main(void) {
    port_init();
    PWM_pin_setup();
    ADC_init();
    PWM_init();
    
    // Fcy = 8 MHz
    // Tcy = 125ns
    // F = 50kHz, T = 20us
    // PR2 = 20us / 125ns
   

    while(1) {
        if (change_duty) {
            OC1RS = new_duty;
            change_duty = 0;
        }
    }
    
    return 0;
}

void ADC_init() {
    // setup ADC configuration bits and TRISB
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0x20E7;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0201;
    AD1CHS = 0x0009;
    AD1CSSL = 0x0000;
    
    IEC0bits.AD1IE = 1; // enable interrupt
    IFS0bits.AD1IF = 0; // clear interrupt
    AD1CON1bits.ADON = 1;   // turn on ADC
}

void PWM_pin_setup(void) {
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
    RPOR3bits.RP7R = 0x12;  // set RP7 as output compare 1
    __builtin_write_OSCCONL(OSCCON|(1<<6));
}

void port_init(void) {
    TRISB = 0xFF7F;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    change_duty = 1;
    IFS0bits.T2IF = 0;  // Reset Timer2 interrupt flag and return from ISR
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    int adcvalue;
    
    // total levels = 1023
    // pwm period = 160 
    // adc_levels = 2^10 - 1 = 1023
    // min_duty = 7 (4%) -> relative to pwm period
    // max_duty = 149 (93&) -> relative to pwm period
    // number of available duty in between = max_duty - min_duty = 149 - 7 = 142
    // number of steps per duty = adc_levels / (max_duty - min_duty)
    //      = 1023 - (149 - 7)
    
    
    IEC0bits.AD1IE = 0; // Disable interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt
    adcvalue = ADC1BUF0;
    if (adcvalue == 0) {
        new_duty = min_duty;
    } else if (adcvalue > 1020) {
        new_duty = 149;
    } else {
        for (i = 0; i < 142; i++) {
            if ((adcvalue > 7*i) && (adcvalue <= 7*(i+1))){
                break;
            }
        }
        new_duty = min_duty + i + 1;
    }
    IEC0bits.AD1IE = 1;// Enable interrupt
    IFS0bits.AD1IF = 0;// Clear interrupt
}

void PWM_init(void) {
    OC1CON = 0;
    OC1R = 0x004F;
    OC1RS = 0x004F;
    OC1CON = 0x2000;
    OC1CONbits.OCM = 0x6;
    PR2 = 0x009F;
    T2CON = 0;          // reset Timer 2 control register
    
    IFS0bits.T2IF = 0;  // clear the Timer 2 interrupt status flag
    IEC0bits.T2IE = 1;  // Enable Timer 2 interrupt
    T2CONbits.TON = 1;  // Start Timer 2 with prescaler settings at 1:1 and
                        // clock source set to the internal instruction cycle
}
