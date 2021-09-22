#ifndef __PERIPH_ADC_H
#define __PERIPH_ADC_H

#include <stdint.h>

typedef enum
{
    ADC1_Channel0 = 0,
    ADC1_Channel1,
    ADC1_Channel2,
    ADC1_Channel3,
    ADC1_Channel4,
    ADC1_Channel5,
    ADC1_Channel6,
    ADC1_Channel7,
    ADC1_Channel8,
    ADC1_Channel9,
    ADC1_Channel10,
    ADC1_Channel11,
    ADC1_Channel12,
    ADC1_Channel13,
    ADC1_Channel14,
    ADC1_Channel_Sum,
} ADC_Channel_List;

void Periph_ADC_Init(uint8_t channel_sum);
void Periph_ADC_Close(ADC_Channel_List ADCx_Channelx);
void Periph_ADC_IO_Init(ADC_Channel_List ADCx_Channelx);
uint16_t Periph_ADC_GetValue(ADC_Channel_List ADCx_Channelx, uint8_t sequence);

#endif
