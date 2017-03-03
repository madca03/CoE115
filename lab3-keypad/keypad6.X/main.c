/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Seatwork - Detecting button presses in a row
 * Created on February 28, 2017, 12:31 PM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)

#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);
void led1_toggle(void);
int row1_press;

int main(void) {
    /* Configure ports
     * RA0 - row 1 (input)
     * RB0 - col 1 (output)
     * RB1 - col 2 (output)
     * RB2 - col 3 (output)
     * RB1 - led 1 (output)
     */
    
    AD1PCFG = 0xffff;
    TRISA = 0x0001;
    TRISB = 0xffe8;
    LATB = 0xffff;
    
    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     */
    CNPU1 = CNPU1 | 0x0004;
    
    /* Enable interrupts and clear IRQ flag
     * RA0 - CN2 (CNEN1)
     */
    CNEN1 = CNEN1 | 0x0004;
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    
    /* Pull down col1 - LATB0 
     * col2 - LATB1
     * col3 - LATB2
     */
    
    /* When all cols are pulled down -> all keys in a row toggle the led
     * When col2 is the one only pulled down -> only key in col2 and rowN toggles LED
     * When col2 and col3 are pulled down -> only keys in col2 and col3 and rowN toggles LED
     */
    
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    while (1) {
        if (row1_press) {
            led1_toggle();
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
    
void led1_toggle(void) {
    LATBbits.LATB4 = ~LATBbits.LATB4;
}