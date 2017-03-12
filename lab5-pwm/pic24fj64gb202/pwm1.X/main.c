/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 10, 2017, 9:13 AM
 */


#include "xc.h"

_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMD_NONE & OSCIOFCN_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL)
_CONFIG3 (SOSCSEL_OFF)

unsigned int i = 0;
unsigned int j = 0;
int change_duty = 0;

void setup(void) {
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
    RPOR3bits.RP7R = 0x0D;  // set RP7 as output compare 1
    __builtin_write_OSCCONL(OSCCON|(1<<6));
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    if (i < 500) {
      i++;
    } else {
      change_duty = 1;
      i = 0;
    }
    IFS0bits.T2IF = 0;  // Reset Timer2 interrupt flag and return from ISR
}

int main(void) {
    setup();
    TRISB = 0xFF7F;
    
    // Fcy = 4 MHz
    // Tcy = 250ns
    // F = 50kHz, T = 20us
    // PR2 = 20us / 250ns
    
    
    
    OC1CON1 = 0;
    OC1R = 0x0000;
    OC1RS = 0x0014;
    OC1CON2 = 0x020C;
    OC1CON1 = 0x2008;
    OC1CON1bits.OCM = 0x7;
    PR2 = 0x004F;
    T2CON = 0;          // reset Timer 2 control register
    
    IFS0bits.T2IF = 0;  // clear the Timer 2 interrupt status flag
    IEC0bits.T2IE = 1;  // Enable Timer 2 interrupt
    T2CONbits.TON = 1;  // Start Timer 2 with prescaler settings at 1:1 and
                        // clock source set to the internal instruction cycle
    
    int incpw = 1;

    while(1) {
      if (change_duty) {
        if (incpw) {
          if (OC1RS < PR2)
            OC1RS = OC1RS + 1;
          if (OC1RS == PR2) incpw = 0;
        } else {
          if (OC1RS > 1)
            OC1RS = OC1RS - 1;
          if (OC1RS == 1) incpw = 1;
        }
          
        change_duty = 0;
      }
    }
    
    return 0;
}
