#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include <stdbool.h>
#include <stdint.h>
#include "periph_spi.h"
#include "queue.h"

typedef struct
{
    SPI_List SPIx;
    SPIClockSpeed_e speed;
    uint16_t CPOL;
    uint16_t CPHA;
} DrvSpi_Obj_TypeDef;

typedef struct
{
    void (*open)(DrvSpi_Obj_TypeDef *Obj);
    void (*close)(DrvSpi_Obj_TypeDef *Obj);
    bool (*set_speed)(DrvSpi_Obj_TypeDef *Obj, SPIClockSpeed_e speed);
    bool (*transmit)(DrvSpi_Obj_TypeDef *Obj, uint8_t data);
    bool (*receive)(DrvSpi_Obj_TypeDef *Obj, uint8_t *data);
    bool (*transmit_receive)(DrvSpi_Obj_TypeDef *Obj, uint8_t tdata, uint8_t *rdata);
    uint16_t (*transfer)(DrvSpi_Obj_TypeDef *Obj, uint8_t *tdata, uint8_t *rdata, uint16_t len);
} DrvSpi_GenProcFunc_TypeDef;

extern DrvSpi_GenProcFunc_TypeDef GenSPI_Drv;

#endif
