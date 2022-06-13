#include "SD.h"

static void SD_send_cmd(uint8_t cmd, uint32_t args, uint8_t crc)
{
        uint8_t buf[6];

        buf[0] = cmd | 0x40;
        buf[1] = args >> 24;
        buf[2] = args >> 16;
        buf[3] = args >> 8;
        buf[4] = args;
        buf[5] = crc;
        for (uint8_t i = 0; i < 6; i++)
                SPI_byte(buf[i]);
}

static uint8_t SD_execute_command_r7(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t *r7_response)
{
        uint8_t ret;

        SD_send_cmd(8, 0x1AA, 0x87);
        while ((ret = SPI_byte(0xFF)) == 0xFF)
                ;

        r7_response[0] = ret;
        for (uint8_t i = 1; i < 5; i++)
                r7_response[i] = SPI_byte(0xFF);

        return ret;
}

static uint8_t SD_execute_command_r1(uint8_t cmd, uint32_t arg, uint8_t crc)
{
        uint8_t ret;

        SD_send_cmd(cmd, arg, crc);
        while ((ret = SPI_byte(0xFF)) == 0xFF)
                ;

        return ret;
}

uint8_t SD_write_block(uint32_t sector, uint8_t *buf)
{
        SPI_byte(0xFF);
        CS_LOW;
        SPI_byte(0xFF);

        uint8_t ret;
        ret = SD_execute_command_r1(24, sector * 512, /*Test crc*/ 0xAA);

        if (ret == 0x00 || ret == 0x01)
        {
                SPI_byte(0xFE);
                for (uint16_t i = 0; i < 512; i++)
                {
                        SPI_byte(*buf++);
                }

                while ((ret = SPI_byte(0xFF)) == 0xFF)
                        ;

                if ((ret & 0x1F) == 0b00101)
                {

                        while ((ret = SPI_byte(0xFF)) == 0x00)
                                ;
                }
        }
        SPI_byte(0xFF);
        CS_HIGH;
        SPI_byte(0xFF);

        return ret;
}

uint8_t SD_read_block(uint32_t sector, uint8_t *buf)
{
        SPI_byte(0xFF);
        CS_LOW;
        SPI_byte(0xFF);

        uint8_t ret;

        SD_execute_command_r1(17, sector * 512, 0);
        while ((ret = SPI_byte(0xFF)) == 0xFF)
                ;

        if (ret == 0xFE)
        {
                for (uint16_t i = 0; i < 512; i++)
                        *buf++ = SPI_byte(0xFF);
                uint8_t crc1, crc2;
                crc1 = SPI_byte(0xFF);
                crc2 = SPI_byte(0xFF);
        }

        SPI_byte(0xFF);
        CS_HIGH;
        SPI_byte(0xFF);

        return ret;
}

void SD_init()
{
        CS_HIGH;
        for (uint8_t i = 0; i < 10; i++)
                SPI_byte(0xFF);
        CS_LOW;

        uint8_t ret = SD_execute_command_r1(0, 0, 0x95);

        if (ret == 0x01)
        {
                uint8_t r7[5];
                SD_execute_command_r7(8, 0x1AA, 0x87, r7);

                _delay_ms(1100);

                do
                {
                        ret = SD_execute_command_r1(1, 0, 0xFF);
                        _delay_ms(10);
                } while (ret != 0 && ret != 1);

                _delay_ms(1100);
                SD_execute_command_r1(16, 512, 21);

                SPI_byte(0xFF);
                CS_HIGH;
                SPI_byte(0xFF);
        }
}
