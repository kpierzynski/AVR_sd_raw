#include <avr/io.h>

#include "spi.h"
#include "SD.h"

int main()
{
	SPI_init();
	SD_init();

	uint8_t buf[512];
	SD_read_block(0, buf);

	while (1)
	{
	}

	return 0;
}
