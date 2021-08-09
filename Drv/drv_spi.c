#include "drv_spi.h"
#include "periph_spi.h"

/* external function define */
static bool DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj);
static bool DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj);
static bool DrvSpi_Transmit(DrvSpi_Obj_TypeDef *Obj, uint8_t data);
static bool DrvSpi_Receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *data);
static bool DrvSpi_transmit_receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata);
static void DrvSpi_SetSpeed(DrvSpi_Obj_TypeDef *Obj, SPIClockSpeed_e speed);

/* internal function define */

DrvSpi_GenProcFunc_TypeDef GenSPI_Drv = {
    .open = DrvSpi_Open,
    .close = DrvSpi_Close,
    .transmit = DrvSpi_Transmit,
    .receive = DrvSpi_Receive,
    .transmit_receive = DrvSpi_transmit_receive,
    .set_speed = DrvSpi_SetSpeed,
};

uint8_t periph_SPI_ReadWriteByte(SPI_List SPIx, u8 TxData);
uint8_t periph_SPI_Transfer(SPI_List SPIx, u8 *rx, u8 *tx, uint16_t len);
uint8_t periph_SPI_WriteByte(SPI_List SPIx, uint8_t TxData);
uint8_t periph_SPI_ReadByte(SPI_List SPIx);
uint8_t periph_SPI_CheckBusy(SPI_List SPIx);

static bool DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj)
{
    periph_SPI_Init(Obj->SPIx, Obj->speed, Obj->CPOL, Obj->CPHA);
}

static bool DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj)
{
    periph_SPI_DeInit(Obj->SPIx);
}

static bool DrvSpi_Transmit(DrvSpi_Obj_TypeDef *Obj, uint8_t data)
{
}

static bool DrvSpi_Receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *data)
{
}

static bool DrvSpi_transmit_receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata)
{
}

static void DrvSpi_SetSpeed(DrvSpi_Obj_TypeDef *Obj, SPIClockSpeed_e speed)
{
    periph_SPI_SetSpeed(Obj->SPIx, Obj->speed);
}
