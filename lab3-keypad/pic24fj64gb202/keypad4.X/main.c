/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Quiz - Detecting button presses in a row
 * Created on February 27, 2017, 12:40 PM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF & ICS_PGx2)
_CONFIG2 (POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_OFF)

#define DEB_MAX 1000

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);

int col1_press;
int col2_press;
int col3_press;

int main(void) {
    /* Configure ports
     * RA0 - row 1 (input)
     *
     * RB0 - col 1 (output)
     * RB1 - col 2 (output)
     * RB2 - col 3 (output)
     *
     * RB4 - led 1 (output)
     * RB5 - led 2 (output)
     * RB7 - led 3 (output)
     * RB8 - led 4 (output)
     */

    ANSA = 0x0000; // AD1PCFG = 0xffff;
    TRISA = 0x000f;
    TRISB = 0xfe18;
    LATB = 0xffff;

    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     */
    CNPU1 = CNPU1 | 0x000c;

    /* Enable interrupts and clear IRQ flag
     * RA0 - CN2 (CNEN1)
     */
    CNEN1 = CNEN1 | 0x000c;
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
        if (col1_press) {
            led1_toggle();
            col1_press = 0;
        }
        if (col2_press) {
            led2_toggle();
            col2_press = 0;
        }
        if (col3_press) {
            led3_toggle();
            col3_press = 0;
        }
    }

    return 0;
}

void __attribute__((interrupt)) _CNInterrupt(void) {
    int deb_ctr = 0;    // debounce counter
    int i = 0;

    if (!PORTAbits.RA1) {
        for (i = 0; i < 3; i++) {
            if (i == 0) {
                LATBbits.LATB0 - 0;
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    col1_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    col1_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 1) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    col2_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    col2_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 2) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 0;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    col3_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    col3_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
        }
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
