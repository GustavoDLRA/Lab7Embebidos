/*
 * spi.h
 *
 * Created: 11/05/2022 05:36:12 p. m.
 *  Author: ejord
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "sam.h"

#define BAUDRATE 9600

#define LENGTH_R1 0x03 
#define LENGTH_R7 0x07
#define SIZE_SD_CMD 0x06  

#define kCMD00 0x40
#define kCMD08 0x48
#define kCMD55 0x77
#define kCMD41 0x69

void spiInit( void );
uint8_t spiSend( uint8_t data );

#endif /* SPI_H_ */