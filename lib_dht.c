#include "xc.h"
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "lib_dht.h"

#define FCY 16000000UL // 16 MHz Instruction Cycle Frequency
#include <libpic30.h>

#define tempUnits 1     // 0 for celcius, any other for fahrenheit
int tmp = 11;

uint8_t DHT11_ReadByte() {
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        while (!PORTBbits.RB4); // Wait for the pin to go high
        __delay_us(30);
        if (PORTBbits.RB4) { // get next incoming bits
            data = (data << 1) | 1;
        } else {
            data = (data << 1);
        }
        while (PORTBbits.RB4); // Wait for the pin to go low
    }
    return data;
}

void DHT11_ReadData(uint8_t *humidity, uint8_t *temperature) {
    uint8_t hum_int, hum_dec, temp_int, temp_dec, checksum;
    TRISBbits.TRISB4 = 0;
    LATBbits.LATB4 = 0;  // pull voltage low
    __delay_ms(20);        // wait 20 ms
    LATBbits.LATB4 = 1;  // bring voltage high
    __delay_us(30);      // wait 30 us for sensor response
    TRISBbits.TRISB4 = 1;  // set to input
    __delay_us(160);      // delay for sensor acknowledgement
    
    
        hum_int = DHT11_ReadByte();  // humidity data
        hum_dec = DHT11_ReadByte();
        temp_int = DHT11_ReadByte();  // temp data
        temp_dec = DHT11_ReadByte();
        checksum = DHT11_ReadByte();
        if (checksum == (hum_int + hum_dec + temp_int + temp_dec)) { // verify data
            *humidity = hum_int;
            if (tempUnits == 0) {
                *temperature = temp_int;
            } else {                
                *temperature = (temp_int * 1.8 + 32);
            }
            
        }
    
}
