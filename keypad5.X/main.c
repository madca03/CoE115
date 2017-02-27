/*
 * File:   main.c
 * Author: Mark
 * Description: Lab3 Quiz - Full keypad
 * Created on February 27, 2017, 8:58 PM
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)

#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led_row1(void);
void led_row2(void);
void led_row3(void);
void led_row4(void);

int row1_press;
int row2_press;
int row3_press;
int row4_press;

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
    
    AD1PCFG = 0xffff;
    TRISA = 0x000f;
    TRISB = 0xfe48;
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
    
    LATB = 0xfff8;  // all columns pulled down
    while (1) {
        if (row1_press) {
            led_row1();
            row1_press = 0; // clear flag
        } else if (row2_press) {
            led_row2();
            row2_press = 0;
        } else if (row3_press) {
            led_row3();
            row3_press = 0;
        } else if (row4_press) {
            led_row4();
            row4_press;
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
    } else if (!PORTAbits.RA1) {
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row2_press = 1;
        else
            row2_press = 0;
    } else if (!PORTAbits.RA2) {
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row3_press = 1;
        else
            row3_press = 0;
    } else if (!PORTAbits.RA3) {
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)) {
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row4_press = 1;
        else
            row4_press = 0;
    }
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}
    
void led_row1(void) {
    int i = 0;
    while (i < 3) {
        switch (i) {
            case 0:
                LATB = 0xfffe;  // col1 pulled down, others pulled up
                if (!PORTAbits.RA0) {   // check row1
                    LATB = LATB & 0xfe5f;   // display "1" in binary
                }
            case 1:
                LATB = 0xfffd;  // col2 pulled down, others pulled up
                if (!PORTAbits.RA0) {
                    LATB = LATB & 0xfe6f;   // display "2" in binary
                }
            case 2:
                LATB = 0xfffb;  // col3 pulled down, others pulled up
                if (!PORTAbits.RA0) {
                    LATB = LATB & 0Xfe7f;   // display "3" in binary
                }
        }
    }
}

void led_row2(void) {
    int i = 0;
    while (i < 3) {
        switch (i) {
            case 0:
                LATB = 0xfffe;  // col1 pulled down, others pulled up
                if (!PORTAbits.RA1) {   // check row2
                    LATB = LATB & 0xfecf;   // display "4" in binary
                }
            case 1:
                LATB = 0xfffd;  // col2 pulled down, others pulled up
                if (!PORTAbits.RA1) {
                    LATB = LATB & 0xfedf;   // display "5" in binary
                }
            case 2:
                LATB = 0xfffb;  // col3 pulled down, others pulled up
                if (!PORTAbits.RA1) {
                    LATB = LATB & 0Xfeef;   // display "6" in binary
                }
        }
    }
}

void led_row3(void) {
    int i = 0;
    while (i < 3) {
        switch (i) {
            case 0:
                LATB = 0xfffe;  // col1 pulled down, others pulled up
                if (!PORTAbits.RA2) {   // check row3
                    LATB = LATB & 0xfeff;   // display "7" in binary
                }
            case 1:
                LATB = 0xfffd;  // col2 pulled down, others pulled up
                if (!PORTAbits.RA2) {
                    LATB = LATB & 0xff4f;   // display "8" in binary
                }
            case 2:
                LATB = 0xfffb;  // col3 pulled down, others pulled up
                if (!PORTAbits.RA2) {
                    LATB = LATB & 0Xff5f;   // display "9" in binary
                }
        }
    }
}

void led_row4(void) {
    int i = 0;
    while (i < 3) {
        switch (i) {
            case 0:
                LATB = 0xfffe;  // col1 pulled down, others pulled up
                if (!PORTAbits.RA3) {   // check row4
                    LATB = LATB & 0xff6f;   // display for "*"
                }
            case 1:
                LATB = 0xfffd;  // col2 pulled down, others pulled up
                if (!PORTAbits.RA3) {
                    LATB = LATB & 0xfe4f;   // display "0" in binary
                }
            case 2:
                LATB = 0xfffb;  // col3 pulled down, others pulled up
                if (!PORTAbits.RA3) {
                    LATB = LATB & 0Xff7f;   // display for "#"
                }
        }
    }
}