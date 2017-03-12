/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 12, 2017, 11:59 AM
 */

// CONFIG1
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)
#pragma config ICS = PGx2   // Comm Channel Select (Emulator EMUC2/EMUD2 pins are shared with PGC2/PGD2)

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


#include "xc.h"

int adcvalue;
int b6;

void ADC_init() {
    // setup ADC configuration bits and TRISB
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0x20E7;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0201;
    AD1CHS = 0x0009;
    AD1CSSL = 0x0000;
    TRISB = 0x8000;
}

void main(void) {
    ADC_init();
    IEC0bits.AD1IE = 1; // enable interrupt
    IFS0bits.AD1IF = 0; // clear interrupt
    AD1CON1bits.ADON = 1;   // turn on ADC
    
    while (1) {
        LATB = adcvalue;
    }
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IEC0bits.AD1IE = 0; // Disable interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt
    adcvalue = ADC1BUF0;
    b6 = adcvalue & 0x0040;
    b6 = b6 << 7;
    adcvalue = adcvalue | b6;
    IEC0bits.AD1IE = 1;// Enable interrupt
    IFS0bits.AD1IF = 0;// Clear interrupt
}