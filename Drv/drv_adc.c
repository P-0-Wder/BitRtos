#include "drv_adc.h"
#include "periph_adc.h"
#include "periph_gpio.h"

/* internal variable */
static bool InUse_ADC1_Channel_List[ADC1_Channel_Sum] = {false};
static uint8_t InUse_ADC_Channel_cnt = 0;

static bool DrvADC_SigalCh_Init(ADC_Channel_List channel)
{
    if (!InUse_ADC1_Channel_List[channel])
    {
        InUse_ADC1_Channel_List[channel] = true;
        InUse_ADC_Channel_cnt++;

        Periph_ADC_Init(channel, InUse_ADC_Channel_cnt);

        return true;
    }

    return false;
}

static uint16_t DrvADC_Get()
{
}
