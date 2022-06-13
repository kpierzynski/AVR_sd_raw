#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

#define DDR_SPI  DDRB
#define PORT_SPI PORTB

#define CS   PB2
#define MOSI PB3
#define MISO PB4
#define SCK  PB5

#define CS_HIGH PORT_SPI |= ( 1 << CS );
#define CS_LOW  PORT_SPI &= ~( 1 << CS );

void SPI_init( );
uint8_t SPI_byte( uint8_t byte );

#endif
