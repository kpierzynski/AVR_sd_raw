#ifndef _SD_H
#define _SD_H

#include <avr/io.h>
#include <util/delay.h>

#include "SPI.h"

void SD_init();

uint8_t SD_read_block(uint32_t sector, uint8_t *buf);
uint8_t SD_write_block(uint32_t sector, uint8_t *buf);

#endif
