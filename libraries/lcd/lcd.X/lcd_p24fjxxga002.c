/*
 * File:   lcd.c
 * Author: Mark
 *
 * Created on March 16, 2017, 10:52 AM
 */


#include "xc.h"
#include "lcd_p24fjxxga002.h"

void send4ToLCD(int data) {
  int data_bits = (data & 0x000F) << 8;
  int rs_bit = (data & 0x0010) ? 1 : 0;

  LATBbits.LATB13 = rs_bit;
  __delay_us(1);
  LATBbits.LATB14 = 1;
  LATB = LATB | data_bits;
  __delay_us(1);
  LATB = 0;
}

void send8ToLCD(int data) {
  int rs_bit = (data & 0x0100) ? 1 : 0;
  int dataH_bits = (data & 0x00F0) << 4;
  int dataL_bits = (data & 0x000F) << 8;

  LATBbits.LATB13 = rs_bit;
  __delay_us(1);
  LATBbits.LATB14 = 1;
  LATB = LATB | dataH_bits;
  __delay_us(1);
  LATBbits.LATB14 = 0;
  __delay_us(1);
  LATB = LATB & 0x2000;
  LATBbits.LATB14 = 1;
  __delay_us(1);
  LATB = LATB | dataL_bits;
  __delay_us(1);
  LATB = 0;
}