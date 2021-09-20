#include "periph_adc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

static void (*ADC_IO_Init[])(void) = {
    GPIO_USART1_IO_Init,
    GPIO_USART2_IO_Init,
    GPIO_USART3_IO_Init,
    GPIO_USART6_IO_Init,
};

static void Periph_ADC_IO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(, ENABLE);

    GPIO_InitStruct.GPIO_Mode = ;
    GPIO_InitStruct.GPIO_OType = ;
    GPIO_InitStruct.GPIO_Pin = ;
    GPIO_InitStruct.GPIO_PuPd = ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
}

static void Periph_ADC_Init()
{
}

static void Periph_ADC_GetValue()
{
}
