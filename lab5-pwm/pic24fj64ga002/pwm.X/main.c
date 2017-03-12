/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 12, 2017, 4:41 PM
 */


#include "xc.h"

// CONFIG1
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)
#pragma config ICS = PGx2   // Comm Channel Select (Emulator EMUC2/EMUD2 pins are shared with PGC2/PGD2)

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

unsigned int i = 0;
int change_duty = 0;

void setup(void) {
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
    RPOR3bits.RP7R = 0x12;  // set RP7 as output compare 1
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
    
    OC1CON = 0;
    OC1R = 0x004F;
    OC1RS = 0x004F;
    OC1CON = 0x2000;
    OC1CONbits.OCM = 0x6;
    PR2 = 0x009F;
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

