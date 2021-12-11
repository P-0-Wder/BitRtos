#include "periph_exti.h"
#include "periph_nvic.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include <string.h>

#if defined STM32F40_41xxx
#define GEN_EXTI_LINE_SUM 16
#endif

static exti_callback CallBack_List[GEN_EXTI_LINE_SUM] = {NULL};

void periph_exit_init(Periph_Exti_Config_TypeDef exti_cfg)
{
    EXTI_InitTypeDef EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(exti_cfg.exti_port, exti_cfg.exti_pinsource);

    EXTI_InitStruct.EXTI_Line = exti_cfg.exti_line;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;

    EXTI_Init(&EXTI_InitStruct);

    periph_nvic_Structure_Setting(exti_cfg.nvic_channel, exti_cfg.pre_priority, exti_cfg.sub_priority, exti_cfg.nvic_state);
}

bool periph_exti_SetCallback(uint32_t EXTI_LineX, exti_callback callbck)
{
}

exti_callback periph_exti_GetCallback(uint32_t EXIT_LineX)
{
    return CallBack_List[EXIT_LineX];
}