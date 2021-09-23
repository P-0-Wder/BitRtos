#include "periph_exti.h"
#include "periph_nvic.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"

void periph_exti_init()
{
    EXTI_InitTypeDef EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}