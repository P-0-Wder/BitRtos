#include "periph_adc.h"
#include "periph_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

static void (*ADC_IO_Init[])(void) = {
    GPIO_ADC1_Channel0_IO_Init,
    GPIO_ADC1_Channel1_IO_Init,
    GPIO_ADC1_Channel2_IO_Init,
    GPIO_ADC1_Channel3_IO_Init,
    GPIO_ADC1_Channel4_IO_Init,
    GPIO_ADC1_Channel5_IO_Init,
    GPIO_ADC1_Channel6_IO_Init,
    GPIO_ADC1_Channel7_IO_Init,
    GPIO_ADC1_Channel8_IO_Init,
    GPIO_ADC1_Channel9_IO_Init,
    GPIO_ADC1_Channel10_IO_Init,
    GPIO_ADC1_Channel11_IO_Init,
    GPIO_ADC1_Channel12_IO_Init,
    GPIO_ADC1_Channel13_IO_Init,
    GPIO_ADC1_Channel14_IO_Init,
};

void Periph_ADC_Init(ADC_Channel_List ADCx_Channelx)
{
    ADC_IO_Init[ADCx_Channelx]();
}

void Periph_ADC_GetValue()
{
}
