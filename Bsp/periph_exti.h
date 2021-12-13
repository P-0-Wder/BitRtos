#ifndef __PERIPH_EXTI_H
#define __PERIPH_EXTI_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"

typedef void (*exti_callback)(void);

typedef struct
{
    uint8_t exti_port;
    uint8_t exti_pinsource;

    uint32_t exti_line;

    EXTITrigger_TypeDef trigger_type;

    uint8_t nvic_channel;
    uint8_t pre_priority;
    uint8_t sub_priority;
} Periph_Exti_Config_TypeDef;

void periph_exit_init(Periph_Exti_Config_TypeDef exti_cfg);
bool periph_exti_SetCallback(uint32_t EXTI_LineX, exti_callback callbck);
exti_callback periph_exti_GetCallback(uint32_t EXIT_LineX);

#endif
