/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 3, 2017, 1:07 AM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)

int adcvalue;

void ADC_init() {
    // setup ADC configuration bits and TRISB
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0xA0E6;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0201;
    AD1CHS = 0x0009;
    AD1CSSL = 0x0000;
    
    TRISB = 0xFFE0;
    LATB = 0xffff;
}

void main(void) {
    ADC_init(); // initialize ADC
    
    IEC0bits.AD1IE = 1; // enable interrupt
    IFS0bits.AD1IF = 0; // clear IRQ flag
    AD1CON1bits.ADON = 1; // turn on ADC
    
    while (1) {
        LATB = adcvalue;
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IEC0bits.AD1IE = 0; //Disable interrupt
    IFS0bits.AD1IF = 0; //Clear flag
    adcvalue = ADC1BUF0;
    IEC0bits.AD1IE = 1;//Enable interrupt
    IFS0bits.AD1IF = 0;//Clear flag
}