#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include <stdint.h>
#include <stdbool.h>
#include "periph_adc.h"

typedef enum
{
    DrvADC_Open = 0,
    DrvADC_Close,
} DrvADC_CMD_List;

typedef struct
{
    void (*Ctl)(DrvADC_CMD_List cmd, uint32_t data, uint16_t len);
    uint16_t (*read)(uint32_t data, uint16_t len);
} DrvADC_TypeDef;

#endif
