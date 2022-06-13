#include "SPI.h"

void SPI_init( ) {
        SPCR |= ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR1 ) | ( 1 << SPR0 );
        SPSR |= ( 1 << SPI2X );

        DDR_SPI |= ( 1 << CS ) | ( 1 << SCK ) | ( 1 << MOSI );
        PORT_SPI |= ( 1 << MISO );
        CS_HIGH;
}

uint8_t SPI_byte( uint8_t byte ) {
        SPDR = byte;
        while ( !( SPSR & ( 1 << SPIF ) ) )
                ;
        return SPDR;
}
