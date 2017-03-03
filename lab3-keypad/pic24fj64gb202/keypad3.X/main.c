/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Seatwork - Detecting button presses in a row
 * Created on February 27, 2017, 4:03 AM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF & ICS_PGx2)
_CONFIG2 (POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_OFF)

#define DEB_MAX 200

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_toggle(void);

int row1_press;
int row2_press;
int row3_press;
int row4_press;

int main(void) {
    /* Configure ports
     * RA0 - row 1 (input)
     * 
     * RB0 - col 1 (output)
     * RB1 - col 2 (output)
     * RB2 - col 3 (output)
     * 
     * RB5 - led 1 (output)
     */
    
    ANSA = 0x0000; // AD1PCFG = 0xffff;
    TRISA = 0x000f;
    TRISB = 0xfe18;
    LATB = 0xffff;
    
    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     * RA1 - CN3 (CNPU1)
     * RA2 - CN30 (CNPU2) bit 14
     * RA3 - CN29 (CNPU2) bit 13
     */
    CNPU1 = CNPU1 | 0x000c;
    CNPU2 = CNPU2 | 0x6000;
    
    
    /* Enable interrupts and clear IRQ flag
     * RA0 - CN2 (CNEN1)
     * RA1 - CN3 (CNEN1)
     * RA2 - CN30 (CNEN2) bit 14
     * RA3 - CN29 (CNEN2) bit 13
     */
    CNEN1 = CNEN1 | 0x000c;
    CNEN2 = CNEN2 | 0x6000;
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
            led1_toggle();
            row1_press = 0; // clear flag
        } 
        if (row2_press) {
            led2_toggle();
            row2_press = 0; // clear flag
        }
        if (row3_press) {
            led3_toggle();
            row3_press = 0; // clear flag
        }
        if (row4_press) {
            led4_toggle();
            row4_press = 0; // clear flag
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
        if (deb_ctr == DEB_MAX) {
            row1_press = 1; // set flag
        } else {
            row1_press = 0;
        }
    } 
    
    if (!PORTAbits.RA1) {
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row2_press = 1; // set flag
        else
            row2_press = 0;
    }

    if (!PORTAbits.RA2) {
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row3_press = 1; // set flag
        else
            row3_press = 0;
    }

    if (!PORTAbits.RA3) {
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row4_press = 1; // set flag
        else
            row4_press = 0;
    }
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}
    
void led1_toggle(void) {
    LATBbits.LATB5 = ~LATBbits.LATB5;
}

void led2_toggle(void) {
    LATBbits.LATB6 = ~LATBbits.LATB6;
}

void led3_toggle(void) {
    LATBbits.LATB7 = ~LATBbits.LATB7;
}

void led4_toggle(void) {
    LATBbits.LATB8 = ~LATBbits.LATB8;
}