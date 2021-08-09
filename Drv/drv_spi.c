#include "drv_spi.h"
#include "periph_spi.h"
#include "periph_gpio.h"

/* external function define */
static bool DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj);
static bool DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj);
static bool DrvSpi_Transmit(DrvSpi_Obj_TypeDef *Obj, uint8_t data);
static bool DrvSpi_Receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *data);
static bool DrvSpi_transmit_receive(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata);

/* internal function define */

DrvSpi_GenProcFunc_TypeDef GenSPI_Drv = {
    .open = DrvSpi_Open,
    .close = DrvSpi_Close,
    .transmit = DrvSpi_Transmit,
    .receive = DrvSpi_Receive,
    .transmit_receive = DrvSpi_transmit_receive,
};

static bool DrvSpi_Open(DrvSpi_Obj_TypeDef *Obj)
{
}

static bool DrvSpi_Close(DrvSpi_Obj_TypeDef *Obj)
{
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
