/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 15, 2017, 1:47 PM
 */


#include "xc.h"

// CONFIG1
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)
#pragma config ICS = PGx2   // Comm Channel Select (Emulator EMUC2/EMUD2 pins are shared with PGC2/PGD2)

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = ON // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define DEB_MAX 500

void __attribute__((interrupt)) _CNInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void keypad_init(void);
void PWM_init(void);
void PWM_pin_setup(void);

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
   * RA0 - row 1 (input) - keypad row
   * RA1 - row 2 (input) - keypad row
   * RA2 - row 3 (input) - keypad row 
   * RA3 - row 4 (input) - keypad row
   *
   * RB0 - col 1 (output) - keypad column
   * RB1 - col 2 (output) - keypad column
   * RB2 - col 3 (output) - keypad column
   *
   * RB7 - PWM output
  */

  AD1PCFG = 0xffff;
  TRISA = 0x000f;
  TRISB = 0xFF78;
  LATB = 0xffff;

  PWM_pin_setup();
  PWM_init();
  keypad_init();

  while(1) {
    if (two_press) {
      if (OC1RS <= PR2) OC1RS = OC1RS + 1;
      two_press = 0;
    } else if (eight_press) {
      if (OC1RS) OC1RS = OC1RS - 1;
      eight_press = 0;
    }
  }
  
  return 0;
}


void keypad_init(void) {
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

    // Pull down all columns
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
}

void PWM_init(void) {
    OC1CON = 0;
    OC1R = 0xFFFF;
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

void PWM_pin_setup(void) {
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
    RPOR3bits.RP7R = 0x12;  // set RP7 as output compare 1
    __builtin_write_OSCCONL(OSCCON|(1<<6));
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;  // Reset Timer2 interrupt flag and return from ISR
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