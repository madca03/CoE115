/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Quiz - Full keypad
 * Created on February 27, 2017, 8:58 PM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF & ICS_PGx2)
_CONFIG2 (POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_OFF)

#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led_row1(void);
void led_row2(void);
void led_row3(void);
void led_row4(void);

void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);
void led4_toggle(void);

int one_press;
int two_press;
int three_press;
int four_press;
int five_press;
int six_press;
int seven_press;
int eight_press;
int nine_press;
int star_press;
int zero_press;
int hash_press;


int main(void) {
    /* Configure ports
     * RA0 - row 1 (input)
     * RA1 - row 2 (input)
     * RA2 - row 3 (input)
     * RA3 - row 4 (input)
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

    ANSA = 0x0000;// AD1PCFG = 0xffff;
    TRISA = 0x000f;
    TRISB = 0xfe18;
    LATB = 0xffff;

    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     * RA1 - CN3 (CNPU1)
     * RA2 - bit 14 (CNPU2)
     * RA3 - bit 13 (CNPU2)
     */
    CNPU1 = CNPU1 | 0x000c;
    CNPU2 = CNPU2 | 0x6000;

    /* Enable interrupts and clear IRQ flag
     * RA0 - CN2 (CNEN1)
     * RA1 - CN3 (CNEN1)
     * RA2 - bit 14 (CNEN2)
     * RA3 - bit 13 (CNEN2)
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
        if (one_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            one_press = 0;
        } else if (two_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            two_press = 0;
        } else if (three_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            three_press = 0;
        } else if (four_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 1;
            four_press = 0;
        } else if (five_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 1;
            five_press = 0;
        } else if (six_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 1;
            six_press = 0;
        } else if (seven_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 1;
            seven_press = 0;
        } else if (eight_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 0;
            eight_press = 0;
        } else if (nine_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 0;
            nine_press = 0;
        } else if (star_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 0;
            star_press = 0;
        } else if (zero_press) {
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            zero_press = 0;
        } else if (hash_press) {
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 0;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 0;
            hash_press = 0;
        }
    }

    return 0;
}

void __attribute__((interrupt)) _CNInterrupt(void) {
    int deb_ctr = 0;    // debounce counter
    int i = 0;

    if (!PORTAbits.RA0) {
        for (i = 0; i < 3; i++) {
            if (i == 0) {
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    one_press = 1;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    one_press = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 1) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    two_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    two_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 2) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 1;

                while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    three_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    break;
                } else {
                    three_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                }
            }
        }
    } else if (!PORTAbits.RA1) {
        for (i = 0; i < 3; i++) {
            if (i == 0) {
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    four_press = 1;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    four_press = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 1) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    five_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    five_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 2) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 1;

                while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    six_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    break;
                } else {
                    six_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                }
            }
        }
    } else if (!PORTAbits.RA2) {
        for (i = 0; i < 3; i++) {
            if (i == 0) {
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    seven_press = 1;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    seven_press = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 1) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    eight_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    eight_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 2) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 1;

                while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    nine_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    break;
                } else {
                    nine_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                }
            }
        }
    } else if (!PORTAbits.RA3) {
        for (i = 0; i < 3; i++) {
            if (i == 0) {
              LATBbits.LATB1 = 1;
              LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    star_press = 1;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    star_press = 0;
                    LATBbits.LATB1 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 1) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB2 = 1;

                while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    zero_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                    break;
                } else {
                    zero_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB2 = 0;
                }
            }
            else if (i == 2) {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 1;

                while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)) {
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) {
                    hash_press = 1;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                    break;
                } else {
                    hash_press = 0;
                    LATBbits.LATB0 = 0;
                    LATBbits.LATB1 = 0;
                }
            }
        }
    }

    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}
