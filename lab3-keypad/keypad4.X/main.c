/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Quiz - Detecting button presses in a row
 * Created on February 27, 2017, 12:40 PM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)

#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led_toggle(void);
void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);

int row1_press;

int main(void) {
    /* Configure ports
     * RA0 - row 1 (input)
     * RB0 - col 1 (output)
     * RB1 - col 2 (output)
     * RB2 - col 3 (output)
     * RB4 - led 1 (output)
     * RB5 - led 2 (output)
     * RB7 - led 3 (output)
     * RB8 - led 4 (output)
     */
    
    AD1PCFG = 0xffff;
    TRISA = 0x0001;
    TRISB = 0xfe48;
    LATB = 0xffff;
    
    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     */
    CNPU1 = CNPU1 | 0x0002;
    
    /* Enable interrupts and clear IRQ flag
     * RA0 - CN2 (CNEN1)
     */
    CNEN1 = CNEN1 | 0x0002;
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    
    /* Pull down col1 - LATB0 
     */
    
    /* When all cols are pulled down -> all keys in a row toggle the led
     * When col2 is the one only pulled down -> only key in col2 and rowN toggles LED
     * When col2 and col3 are pulled down -> all keys in a row toggle the led
     */
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    while (1) {
        if (row1_press) {
            led_toggle();
            row1_press = 0; // clear flag
        }
    }
    
    return 0;
}

void __attribute__((interrupt)) _CNInterrupt(void) {
    int deb_ctr = 0;    // debounce counter
    
    if (!PORTAbits.RA0) {
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row1_press = 1; // set flag
        else
            row1_press = 0;
    }
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}
    
void led_toggle(void) {
    int i = 0;
    while (i < 3) {
        switch(i) {
            case 0:
                LATB = 0xfffe;  // col1 is pulled down, others pulled up
                if (!PORTAbits.RA0) {
                    led1_toggle();
                }
            case 1:
                LATB = 0xfffd;  // col2 is pulled down, others pulled up
                if (!PORTAbits.RA0) {
                    led2_toggle();
                }
            case 2:
                LATB = 0xfffb;  // col3 is pulled down, others pulled up
                if (!PORTAbits.RA0) {
                    led3_toggle();
                }
        }
    }
}

void led1_toggle(void) {
    LATBbits.LATB4 = ~LATBbits.LATB4;
}

void led2_toggle(void) {
    LATBbits.LATB5 = ~LATBbits.LATB5;
}

void led3_toggle(void) {
    LATBbits.LATB7 = ~LATBbits.LATB7;
}