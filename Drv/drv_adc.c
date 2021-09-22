#include "drv_adc.h"
#include "periph_adc.h"
#include "periph_gpio.h"

/* internal variable */
static bool InUse_ADC1_Channel_List[ADC1_Channel_Sum] = {false};
static uint8_t InUse_ADC_Channel_cnt = 0;

static bool DrvADC_ConvertNumber_Init(void)
{
    if (InUse_ADC_Channel_cnt > 0)
    {
        Periph_ADC_Init(InUse_ADC_Channel_cnt);
        return true;
    }

    return false;
}

static bool DrvADC_SigalCh_Init(ADC_Channel_List channel)
{
    if (!InUse_ADC1_Channel_List[channel])
    {
        InUse_ADC1_Channel_List[channel] = true;
        InUse_ADC_Channel_cnt++;

        return true;
    }

    return false;
}

static bool DrvADC_ChToCh_Init(ADC_Channel_List ch_s, ADC_Channel_List ch_e)
{
}

static uint16_t DrvADC_Get()
{
}
