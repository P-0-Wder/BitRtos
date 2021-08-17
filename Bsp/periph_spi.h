#ifndef __PERIPH_SPI_H
#define __PERIPH_SPI_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_spi.h"

#define SPI_PORT_SUM 3
#define SPI_TIMEOUT 1000

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
void periph_SPI_DeInit(SPI_List SPIx);
void periph_SPI_SetSpeed(SPI_List SPIx, SPIClockSpeed_e speed);

bool periph_SPI_ReadWriteByte(SPI_List SPIx, uint8_t TxData, uint8_t *RxData);
uint16_t periph_SPI_Transfer(SPI_List SPIx, uint8_t *rx, uint8_t *tx, uint16_t len);
bool periph_SPI_WriteByte(SPI_List SPIx, uint8_t TxData);
uint8_t periph_SPI_CheckBusy(SPI_List SPIx);
bool periph_SPI_ReadByte(SPI_List SPIx, uint8_t *data);
#endif
