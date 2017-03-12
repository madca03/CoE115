 /*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 11, 2017, 6:10 PM
 */


#include "xc.h"

_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF & ICS_PGx2)
_CONFIG2 (POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)
_CONFIG3 (SOSCSEL_OFF)

int adcvalue;
unsigned int i = 0;

void ADC_init() {
    // setup ADC configuration bits and TRISB
    TRISB = 0x8000;
    ANSB = 0x8000;  // RB15 is configured as analog input
    
    // Discontinue module operation when device enters IDLE mode
    // Sample clock source = internal counter
    // Sampling begins immediately after last conversion; SAMP bit is auto set
    AD1CON1 = 0x2074;
    AD1CON2 = 0x0800;
    AD1CON3 = 0x0201;
    AD1CON5 = 0x0000;
    
    AD1CHS = 0x0009;
    ANCFG = 0x0003;
    AD1CHITL = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;
    AD1CTMENL = 0x0000;
}

void main(void) {
    // OSCCONbits.SOSCEN = 0;
    
    ADC_init(); // initialize ADC
    IEC0bits.AD1IE = 1; // enable interrupt
    IFS0bits.AD1IF = 0;// clear interrupt flag
    AD1CON1bits.ADON = 1;   // turn on ADC
    
    while (1) {
        LATB = adcvalue;
    }
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IEC0bits.AD1IE = 0; // Disable interrupt
    IFS0bits.AD1IF = 0;// Clear Flag
    adcvalue = ADC1BUF0;
    IEC0bits.AD1IE = 1;// enable interrupt
    IFS0bits.AD1IF = 0;// clear flag
}