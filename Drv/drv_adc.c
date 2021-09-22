#include "drv_adc.h"
#include "periph_adc.h"
#include "periph_gpio.h"

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

static void DrvADC_SigalCh_Init(ADC_Channel_List channel)
{
}

static bool DrvADC_ChToCh_Init(ADC_Channel_List ch_s, ADC_Channel_List ch_e)
{
}

static uint16_t DrvADC_Get()
{
}
