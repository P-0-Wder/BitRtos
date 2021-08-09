#ifndef __DEV_SCREEN_H
#define __DEV_SCREEN_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum
{
    Oled_Bus_I2C,
    Oled_Bus_Spi,
} OledBus_TypeDef;

typedef struct
{
    OledBus_TypeDef Bus;
    uint8_t Bus_PortID;

    bool (*bus_init)();
    bool (*transmit)(uint8_t data);
    uint8_t (*receive)(uint8_t addr);
    bool (*transmit_receive)(uint8_t *tdata, uint8_t *rdata, uint16_t len);
} OledObj_TypeDef;

#endif
