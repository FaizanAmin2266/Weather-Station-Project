#include "xc.h"
#include "stdint.h"
#include <stdio.h>
#include <string.h>

#include "lib_MAX.h"

#define MAX7219_NOOP        0x00
#define MAX7219_DIGIT0      0x01
#define MAX7219_DIGIT1      0x02
#define MAX7219_DIGIT2      0x03
#define MAX7219_DIGIT3      0x04
#define MAX7219_DIGIT4      0x05
#define MAX7219_DIGIT5      0x06
#define MAX7219_DIGIT6      0x07
#define MAX7219_DIGIT7      0x08
#define MAX7219_DECODEMODE  0x09
#define MAX7219_INTENSITY   0x0A
#define MAX7219_SCANLIMIT   0x0B
#define MAX7219_SHUTDOWN    0x0C
#define MAX7219_DISPLAYTEST 0x0F   // Display settings

#define DIN LATBbits.LATB14   // Display pin 1
#define CS  LATBbits.LATB13   // display pin 2
#define CLK LATBbits.LATB12   // display pin 3

#define tempUnits 1     // 0 for celcius, any other for fahrenheit
int tmp = 11;


void MAX7219_send (unsigned char a, unsigned char d) {  // send data to display with spi
//    CS = 0;
    for (int i=7; i>=0; i--) {                          // a is address, referring to the MAX register to write to (1-8 are for digits)
        DIN = (a >> i) & 1;                             // d is the data to write to the register
        CLK = 1;
        CLK = 0;
    }

    for (int i=7; i>=0; i--) {
        DIN = (d >> i) & 1;
        CLK = 1;
        CLK = 0;
    }

    DIN = 0;
//    CS = 1;

}
void MAX7219_update (void) {  // update display	
    CS = 1;
    CS = 0;
	
}


void MAX7219_Init() {        // Initialize all 4 MAX chips
    for (uint8_t i = 0; i < 4; i++) {
        MAX7219_send(MAX7219_SHUTDOWN, 0x01);    // Exit shutdown mode
        MAX7219_send(MAX7219_SCANLIMIT, 0x07);   // Display digits 0-7
        MAX7219_send(MAX7219_DECODEMODE, 0x00);  // No decode for digits
        MAX7219_send(MAX7219_INTENSITY, 0x0F);   // Brightness intensity
        MAX7219_send(MAX7219_DISPLAYTEST, 0x00); // Disable display test
        MAX7219_update();
    }
    if (tempUnits == 0) {
        tmp = 11;
    } else {
        tmp = 13;
    }
}

void MAX7219_Clear() {         // Clear Display
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 1; j <= 8; j++) {
            MAX7219_send(j, 0x00);
        }
        MAX7219_update();
    }
}

// Array containing numbers and symbols used for display
const uint8_t fontArray[14][8] = {
    {0b0000, // 0
    0b1110,
    0b1010,
    0b1010,
    0b1010,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,  // 1
    0b0100,
    0b0100,
    0b0100,
    0b0100,
    0b0100,
    0b0000,
    0b0000},
    
    {0b0000, // 2
    0b1110,
    0b0010,
    0b1110,
    0b1000,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,
    0b1110,  // 3
    0b1000,
    0b1100,
    0b1000,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,
    0b1000,  // 4
    0b1000,
    0b1110,
    0b1010,
    0b1010,
    0b0000,
    0b0000},
    
    {0b0000, // 5
    0b1110,
    0b1000,
    0b1110,
    0b0010,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,
    0b1110, // 6
    0b1010,
    0b1110,
    0b0010,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000, // 7
    0b1000,
    0b1000,
    0b1000,
    0b1000,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,
    0b1110, // 8
    0b1010,
    0b1110,
    0b1010,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,  // 9 
    0b1110,
    0b1000,
    0b1110,
    0b1010,
    0b1110,
    0b0000,
    0b0000},
    
    {0b0000,
    0b0000, // 10 (space)
    0b0000,
    0b0000,
    0b0000,
    0b0000,
    0b0000,
    0b0000},
    
    {0b0000,  // 11 (celcius)
    0b0000,
    0b1100,
    0b0100,
    0b0100,
    0b1100,
    0b0001,
    0b0000},
    
    {0b0000,  // 12 (percent)
    0b0000,
    0b0000,
    0b1010,
    0b0100,
    0b1010,
    0b0000,
    0b0000},
    
    {0b0000,  // 13 (fahrenheit)
    0b0100,
    0b0100,
    0b1100,
    0b0100,
    0b1100,
    0b0001,
    0b0000},
};

void print_numbers_on_display(uint8_t numbers[8]) {

    for (int row = 8; row > 0; row--) {
        for (int col = 7; col >= 0; col-=2) {  // Iterates through all columns of each row
            uint8_t number = numbers[col];
            uint8_t numberN = numbers[col-1];  // allows for 2 digits per panel
            uint8_t what = (fontArray[numberN][row]) | (fontArray[number][row] << 4);  // shift both digits together for transfer
            MAX7219_send(row, what );  // send both digits to display
        }
        MAX7219_update();  // update screen
    }
    
}
