#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include <stdint.h>
#include <stdbool.h>
#include "periph_adc.h"

typedef enum
{
    ADC_Open = 0,
    ADC_Close,
} DrvADC_CMD_List;

typedef struct
{
    void (*ctl)(DrvADC_CMD_List CMD, uint8_t data);
    uint16_t (*read)(uint32_t data, uint16_t len);
} DrvADC_TypeDef;

extern DrvADC_TypeDef DrvADC;

#endif
