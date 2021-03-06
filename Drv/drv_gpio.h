#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include <stdbool.h>
#include <stdint.h>
#include "periph_gpio.h"
#include "periph_exti.h"

typedef void (*GPIO_EXTI_Callback)(void);

typedef enum
{
    GPIO_ERR = 0,
    GPIO_LOW,
    GPIO_HIGH,
} DrvGPIO_IO_Level_TypeDef;

typedef enum
{
    GPIO_Output = 0,
    GPIO_Input,
    GPIO_Encoder,
    GPIO_EXTI_Input,
} DrvGPIO_CTL_TypeDef;

typedef struct
{
    uint32_t CLK;
    uint16_t Pin;
    uint8_t Pin_Src;
    void *Port;
    DrvGPIO_CTL_TypeDef IO_Type;
    uint8_t AF_TIMx;

    Periph_Exti_Config_TypeDef Exti_Cfg;
} DrvGPIO_Obj_TypeDef;

/* still need exti init & interrupt func regist */
typedef struct
{
    bool (*open)(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_CTL_TypeDef type, uint32_t data);
    bool (*close)(DrvGPIO_Obj_TypeDef *Obj);
    bool (*set)(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_IO_Level_TypeDef level);
    DrvGPIO_IO_Level_TypeDef (*get)(DrvGPIO_Obj_TypeDef *Obj);
} DrvGPIO_GenProcFunc_TypeDef;

extern DrvGPIO_GenProcFunc_TypeDef DrvGPIO;

#endif
