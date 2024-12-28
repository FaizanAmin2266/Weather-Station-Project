#include "xc.h"
#include "stdint.h"
#include "newAsmTemplate.h"
#include <stdio.h>
#include <string.h>
//#include "DHT_lib.h"
#include "lib_MAX.h"
#include "lib_dht.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)



// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))





void delay_ms(int ms) {
    while (ms--) {
        for (int i = 0; i < 2345; i++) { // approximately 1 ms delay
            asm("nop");
        }
    }
}

void lcd_cmd(char command) {
    LATBbits.LATB5 = 1;
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN);
    _MI2C1IF = 0;
    
    I2C1TRN = 0b01111000;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
    
    I2C1TRN = 0x00;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
    
    I2C1TRN = command;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
     
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);
    LATBbits.LATB5 = 0;
}

void delay(long n) {
    for (n=n; n>0; n--) {
        asm("nop");
    }
}

void lcd_init() {
    LATBbits.LATB6=0;
    delay(50000);
    LATBbits.LATB6=1;
    delay(50000);
    lcd_cmd(0x3A);  // function set
    lcd_cmd(0x09);  // extended function set
    lcd_cmd(0x06);  // entry mode set
    lcd_cmd(0x1E);  // bias setting
    
    lcd_cmd(0x39);  // function set
    lcd_cmd(0x1B);  // internal osc
    lcd_cmd(0x6E);  // follower control
    lcd_cmd(0x56);  // power control
    lcd_cmd(0x7A);  // contrast set
    lcd_cmd(0x38);  // function set
    lcd_cmd(0x0F);  // display on
    
    /* Function set (RE=1 version) */
lcd_cmd(0x3a); /* DL, N, ~BE, enter extended mode RE=1, ~REV */ 
/* Extended function set (assumes RE=1) */
lcd_cmd(0x09); /* NW, ~FW, ~B/W */
/* Double-height/bias/dot-shift (assumes RE=1) */
lcd_cmd(0x1a); /* UD2, ~UD1, BS1, ~DH? */
/* Function set (RE=0 version, IS=0) */
lcd_cmd(0x3c); /* DL, N, DH, return to RE=0, ~IS */
   
}

void lcd_printChar(char myChar) {
    LATBbits.LATB5 = 1;
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN);
    _MI2C1IF = 0;
    
    I2C1TRN = 0b01111000;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
    
    I2C1TRN = 0b01000000;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
    
    I2C1TRN = myChar;
    while(!_MI2C1IF || I2C1STATbits.TRSTAT);
    _MI2C1IF = 0;
    
    
    
    
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);
    LATBbits.LATB5 = 0;
}

void lcd_setCursor(int x, int y) {
    lcd_cmd(128 | ((0x20*y)+x));
}

void lcd_printStr(const char * str) {
    while (*str) {
        lcd_printChar(*str++);
    }
}

void clear() {
    lcd_cmd(1);
}

void setup() {
    CLKDIVbits.RCDIV = 0;  
    AD1PCFG = 0xffff;   // set pins as digital
    TRISBbits.TRISB4 = 1;
    LATBbits.LATB4 = 1;
    
    
    LATBbits.LATB6 = 1;
    LATBbits.LATB5 = 0;
    LATAbits.LATA0 = 1;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISAbits.TRISA0 = 1;
    
    
    TRISBbits.TRISB14 = 0; // Set RB2 as output
    TRISBbits.TRISB13 = 0; // Set RB3 as output
    TRISBbits.TRISB12 = 0; // Set RB5 as output
    
    I2C1CONbits.I2CEN = 0;
    I2C1BRG = 157;
    I2C1CONbits.I2CEN = 1;
}


void displayTempAndHumidity(int temperature, int humidity) {
    char buffer[16];

    // Clear the LCD
    lcd_cmd(0x01);  // Clear display command
    delay_ms(2);

    // Print temperature
    if (tempUnits == 0) {
        sprintf(buffer, "Temp: %dC", temperature);
    } else {
        sprintf(buffer, "Temp: %dF", temperature);
    }
    
    lcd_printStr(buffer);

    lcd_setCursor(0,1);
    delay_ms(2);

    // Print humidity
    sprintf(buffer, "H: %d%%", humidity);
    lcd_printStr(buffer);
}

int main(void) {
    setup();
    MAX7219_Init();
    MAX7219_Clear();
    lcd_init();
    clear();
    uint8_t humidity, temperature;
    uint8_t numbers_to_display[8] = {10, 10, tmp, 10, 10, 10, 12, 10};  // format to display numbers
 
//        MAX7219_send(8, 0b00000000); MAX7219_send(8, 0b00000000);MAX7219_send(8, 0b00000000); MAX7219_send(8, 0b00000000); MAX7219_send(8, 0b00000000); MAX7219_update();
//        MAX7219_send(7, 0b01001010); MAX7219_send(7, 0b00000000); MAX7219_send(7, 0b00000000); MAX7219_send(7, 0b00000000); MAX7219_update();
//		MAX7219_send(6, 0b01001010); MAX7219_send(6, 0b00000000); MAX7219_send(6, 0b00000000); MAX7219_send(6, 0b00000000); MAX7219_update();
//		MAX7219_send(5, 0b01001110); MAX7219_send(5, 0b00000000); MAX7219_send(5, 0b00000000); MAX7219_send(5, 0b00000000); MAX7219_update();
//		MAX7219_send(4, 0b01001000); MAX7219_send(4, 0b00000000); MAX7219_send(4, 0b00000000); MAX7219_send(4, 0b00000000); MAX7219_update();
//		MAX7219_send(3, 0b01001000); MAX7219_send(3, 0b00000000); MAX7219_send(3, 0b00000000); MAX7219_send(3, 0b00000000); MAX7219_update();
//		MAX7219_send(2, 0b01001000); MAX7219_send(2, 0b00000000); MAX7219_send(2, 0b00000000); MAX7219_send(2, 0b00000000); MAX7219_update();
//        MAX7219_send(1, 0b00000000); MAX7219_send(1, 0b00000000); MAX7219_send(1, 0b00000000); MAX7219_send(1, 0b00000000); MAX7219_update();
    

    
    while(1) {
        delay_ms(3000);  // wait between measurements 
        DHT11_ReadData(&humidity, &temperature);  // obtain sensor data
        numbers_to_display[0] = temperature / 10;  // split temp into 2 digits
        numbers_to_display[1] = temperature % 10;
        numbers_to_display[4] = humidity / 10;     // split humidity into 2 digits
        numbers_to_display[5] = humidity % 10;
        MAX7219_Clear();   // refresh screen
        print_numbers_on_display(numbers_to_display);   // display info
        displayTempAndHumidity(temperature, humidity);
        


        
    }
}