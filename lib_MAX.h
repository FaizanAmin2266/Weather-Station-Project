/* 
 * File:   lib_MAX.h
 * Author: fuzzw
 *
 * Created on December 11, 2024, 7:09 PM
 */

#ifndef LIB_MAX_H
#define	LIB_MAX_H

#ifdef	__cplusplus
extern "C" {
#endif

    void MAX7219_send (unsigned char a, unsigned char d);
    void MAX7219_update (void);
    void MAX7219_Init();
    void MAX7219_Clear();
    const uint8_t fontArray[14][8];
    void print_numbers_on_display(uint8_t numbers[8]);



#ifdef	__cplusplus
}
#endif

#endif	/* LIB_MAX_H */

