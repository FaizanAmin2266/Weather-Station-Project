/* 
 * File:   lib_dht.h
 * Author: fuzzw
 *
 * Created on December 11, 2024, 7:04 PM
 */

#ifndef LIB_DHT_H
#define	LIB_DHT_H

#ifdef	__cplusplus
extern "C" {
#endif


    uint8_t DHT11_ReadByte();
    void DHT11_ReadData(uint8_t *humidity, uint8_t *temperature);


#ifdef	__cplusplus
}
#endif

#endif	/* LIB_DHT_H */

