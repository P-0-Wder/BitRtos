#ifndef __PERIPH_SPI_H
#define __PERIPH_SPI_H

#include "stm32f4xx.h"

#define SPI_PORT_SUM 3

typedef enum
{
    SPI_1,
    SPI_2,
    SPI_3,
} SPI_List;

typedef enum
{
    SPI_CLOCK_INITIALIZATON = 0, // Lowest possible
    SPI_CLOCK_SLOW = 1,          // ~1 MHz
    SPI_CLOCK_STANDARD = 2,      // ~10MHz
    SPI_CLOCK_FAST = 3,          // ~20MHz
    SPI_CLOCK_ULTRAFAST = 4      // Highest possible
} SPIClockSpeed_e;

void periph_SPI_Init(SPI_List SPIx, SPIClockSpeed_e speed, uint16_t CPOL, uint16_t CPHA);
void periph_SPI_SetSpeed(SPI_List SPIx, SPIClockSpeed_e speed);

uint8_t periph_SPI_ReadWriteByte(SPI_List SPIx, u8 TxData);
uint8_t periph_SPI_Transfer(SPI_List SPIx, u8 *rx, u8 *tx, uint16_t len);
uint8_t periph_SPI_WriteByte(SPI_List SPIx, uint8_t TxData);
uint8_t periph_SPI_ReadByte(SPI_List SPIx);
uint8_t periph_SPI_CheckBusy(SPI_List SPIx);

#endif
