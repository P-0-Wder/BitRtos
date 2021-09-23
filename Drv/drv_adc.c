#include "drv_adc.h"
#include "periph_adc.h"
#include "periph_gpio.h"

typedef struct
{
    bool InUse;
    uint8_t id;
} ADC_Channel_State;

/* internal variable */
static ADC_Channel_State Channel_State[ADC1_Channel_Sum] = {false};
static uint8_t InUse_ADC_Channel_cnt = 0;

/* external variable */
static bool DrvADC_SigalCh_Init(ADC_Channel_List channel);
static uint16_t DrvADC_Get(ADC_Channel_List channel);
static bool DrvADC_Ctl(DrvADC_CMD_List cmd, uint8_t data);

/* driver object */
DrvADC_TypeDef DrvADC = {
    .ctl = DrvADC_Ctl,
    .read = DrvADC_Get,
};

static bool DrvADC_SigalCh_Init(ADC_Channel_List channel)
{
    if (channel >= ADC1_Channel_Sum)
        return false;

    if (!Channel_State[channel].InUse)
    {
        Channel_State[channel].InUse = true;
        Channel_State[channel].id = InUse_ADC_Channel_cnt;

        InUse_ADC_Channel_cnt++;

        Periph_ADC_IO_Init(channel);

        return true;
    }

    return false;
}

static bool DrvADC_Open(void)
{
    Periph_ADC_Init(InUse_ADC_Channel_cnt);

    return true;
}

static uint16_t DrvADC_Get(ADC_Channel_List channel)
{
    if (Channel_State[channel].InUse)
    {
        return Periph_ADC_GetValue(channel, Channel_State[channel].id);
    }

    return 0;
}

static bool DrvADC_Ctl(DrvADC_CMD_List cmd, uint8_t data)
{
    switch ((uint8_t)cmd)
    {
    case ADC_Config_IO:
        return DrvADC_SigalCh_Init(data);

    case ADC_Open:
        return DrvADC_Open();

    default:
        return false;
    }

    return false;
}
