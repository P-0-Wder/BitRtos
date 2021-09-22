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

void Periph_ADC_Init(ADC_Channel_List ADCx_Channelx, uint8_t channel_sum)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    ADC_IO_Init[ADCx_Channelx]();

    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = channel_sum;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
}

void Periph_ADC_Close(ADC_Channel_List ADCx_Channelx)
{
}

uint16_t Periph_ADC_GetValue(ADC_Channel_List ADCx_Channelx, uint8_t sequence)
{
    ADC_RegularChannelConfig(ADC1, ADCx_Channelx, sequence, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;
    return ADC_GetConversionValue(ADC1);
}
