#include "drv_spi.h"
#include "periph_spi.h"

/* external function define */
static void DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj);
static void DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj);
static bool DrvSpi_Transmit(DrvSpi_Obj_TypeDef *Obj, uint8_t data);
static bool DrvSpi_Receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *data);
static bool DrvSpi_transmit_receive(DrvSpi_Obj_TypeDef *Obj, uint8_t tdata, uint8_t *rdata);
static void DrvSpi_SetSpeed(DrvSpi_Obj_TypeDef *Obj, SPIClockSpeed_e speed);
static uint16_t DrvSpi_Transfer(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata, uint16_t len);

/* internal function define */

DrvSpi_GenProcFunc_TypeDef GenSPI_Drv = {
    .open = DrvSpi_Open,
    .close = DrvSpi_Close,
    .transmit = DrvSpi_Transmit,
    .receive = DrvSpi_Receive,
    .transmit_receive = DrvSpi_transmit_receive,
    .set_speed = DrvSpi_SetSpeed,
    .transfer = DrvSpi_Transfer,
};

uint8_t periph_SPI_CheckBusy(SPI_List SPIx);

static void DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj)
{
    periph_SPI_Init(Obj->SPIx, Obj->speed, Obj->CPOL, Obj->CPHA);
}

static void DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj)
{
    periph_SPI_DeInit(Obj->SPIx);
}

static bool DrvSpi_Transmit(DrvSpi_Obj_TypeDef *Obj, uint8_t data)
{
    return periph_SPI_WriteByte(Obj->SPIx, data);
}

static bool DrvSpi_Receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *data)
{
    return periph_SPI_ReadByte(Obj->SPIx, data);
}

static bool DrvSpi_transmit_receive(DrvSpi_Obj_TypeDef *Obj, uint8_t tdata, uint8_t *rdata)
{
    return periph_SPI_ReadWriteByte(Obj->SPIx, tdata, rdata);
}

static uint16_t DrvSpi_Transfer(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata, uint16_t len)
{
    return periph_SPI_Transfer(Obj->SPIx, rdata, tdata, len);
}

static void DrvSpi_SetSpeed(DrvSpi_Obj_TypeDef *Obj, SPIClockSpeed_e speed)
{
    periph_SPI_SetSpeed(Obj->SPIx, Obj->speed);
}
