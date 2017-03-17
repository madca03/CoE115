/*
 * File:   main.c
 * Author: Mark
 *
 * Created on March 16, 2017, 6:47 AM
 */

// CONFIG1
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)
#pragma config ICS = PGx2   // Comm Channel Select (Emulator EMUC2/EMUD2 pins are shared with PGC2/PGD2)

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#include "xc.h"
#define FCY 8000000UL
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// #include "../../../libraries/lcd/lcd.X/lcd_p24fjxxga002.h"

#define CHAR_HEX_SIZE (4)
#define LCD_WRITE_EXEC_TIME (100) 

#define COMMAND (0x01)
#define DATA    (0x02)

#define ROW0    0
#define ROW1    1

#define LCD_command(data)   LCD_send(data, COMMAND)
#define LCD_write(data)                     \
  do {                                      \
    LCD_send(data, DATA);                   \
    __delay_us(LCD_WRITE_EXEC_TIME);        \
  } while(0)

#define LCD_FUNCTION_SET_CMD        (0x0028)
#define LCD_DISPLAY_OFF_CMD         (0x0008)
#define LCD_CLEAR_DISPLAY_CMD       (0x0001)
#define LCD_ENTRY_MODE_SET_CMD      (0x0006)
#define LCD_DISPLAY_ON_CMD          (0x000C)
#define LCD_RETURN_HOME_CURSOR_CMD  (0x0080)

#define LCD_function_set()    LCD_command(LCD_FUNCTION_SET_CMD)
#define LCD_display_off()     LCD_command(LCD_DISPLAY_OFF_CMD)

#define LCD_clear_display()                 \
  do {                                      \
    LCD_command(LCD_CLEAR_DISPLAY_CMD);     \
    __delay_ms(15);                         \
  } while (0);

#define LCD_entry_mode_set()  LCD_command(LCD_ENTRY_MODE_SET_CMD)
#define LCD_display_on()      LCD_command(LCD_DISPLAY_ON_CMD)

#define LCD_return_home()                       \
  do {                                          \
    LCD_command(LCD_RETURN_HOME_CURSOR_CMD);    \
    __delay_us(100);                            \
  } while (0)

#define LCD_set_cursor(row, col)              \
  do {                                        \
    int address;                              \
    address = (!row) ? 0x00 : 0x40;           \
    address += col;                           \
    LCD_command(address | 0x80);              \
  } while(0)

#define LCD_power_on_init()   \
  do {                        \
    LCD_send4bit(0x0003);     \
    __delay_ms(5);            \
    LCD_send4bit(0x0003);     \
    __delay_us(100);          \
    LCD_send4bit(0x0003);     \
    __delay_us(100);          \
    LCD_send4bit(0x0002);     \
  } while (0)

void LCD_send(int data, int mode);
void LCD_send4bit(int data);
void LCD_init();
void LCD_print_char(char c);
void LCD_print(char *data);

void ADC_init();
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

int adcvalue;
char adcvalue_str[17];
int i = 0;

int main(void) {
  // RB14 - LCD enable pin
  // RB13 - LCD RS pin
  // RB[11:8] - LCD data pins

  // RB15 - temp sensor analog pin

  TRISB = 0x90FF;

  ADC_init();

  LCD_init();
  LCD_return_home();
  LCD_set_cursor(ROW1, 0);
  LCD_print("Agaton");

  while(1) {
    LCD_return_home();
    LCD_print(adcvalue_str);
  }

  return 0; 
}


void ADC_init() {
    // setup ADC configuration bits and TRISB
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0x20E7;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0201;
    AD1CHS = 0x0009;
    AD1CSSL = 0x0000;

    IEC0bits.AD1IE = 1; // enable interrupt
    IFS0bits.AD1IF = 0; // clear interrupt
    AD1CON1bits.ADON = 1;   // turn on ADC
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
  if (i < 0xFFFF) {
    i++;
    IEC0bits.AD1IE = 1; // Enable interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt
  } else {
    i = 0;
    IEC0bits.AD1IE = 0; // Disable interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt
    
    adcvalue = ADC1BUF0;
    sprintf(adcvalue_str, "%lf", ((double) adcvalue / pow(2,10)) * 3.43 * 100);

    IEC0bits.AD1IE = 1; // Enable interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt
  }
}


/*========================================*/

void LCD_print_char(char c) {
  char temp[2];
  int val;

  sprintf(temp, "%x", c);
  val = strtol(temp, NULL, 16);
  LCD_write(val);
}

void LCD_print(char *data) {
  int i;

  for (i = 0; i < strlen(data); i++) {
    LCD_print_char(data[i]);
  }
}

void LCD_init() {
  __delay_ms(15);

  LCD_power_on_init();

  LCD_function_set();
  __delay_us(100);

  LCD_display_off();
  __delay_us(100);

  LCD_clear_display();

  LCD_entry_mode_set();
  __delay_us(100);

  LCD_display_on();
  __delay_us(100);
}

void LCD_send(data, mode) {
  int dataH_bits = (data & 0x00F0) << 4;
  int dataL_bits = (data & 0x000F) << 8;

  LATBbits.LATB13 = (mode == DATA) ? 1 : 0;
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

void LCD_send4bit(data) {
  int data_bits = (data & 0x000F) << 8;

  LATBbits.LATB14 = 1;
  LATB = LATB | data_bits;
  __delay_us(1);
  LATB = 0;
}