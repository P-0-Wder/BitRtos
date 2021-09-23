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

void periph_exti_init()
{
    EXTI_InitTypeDef EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig();

    EXTI_InitStruct.EXTI_Line = ;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;

    EXTI_Init(&EXTI_InitStruct);

    periph_nvic_Structure_Setting();
}

exti_callback periph_exti_GetCallback(uint32_t EXTI_LineX)
{
    return CallBack_List[EXTI_LineX];
}