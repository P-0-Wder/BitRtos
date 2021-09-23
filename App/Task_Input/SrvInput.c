#include "SrvInput.h"
#include "drv_adc.h"

/* internal variable */
static SrvInput_Data_TypeDef InputData;

/* internal funciton */
static void SrvInput_Init(void);
static void SrvInput_Sample(void);
static SrvInput_Data_TypeDef SrvInput_GetData(void);

/* external variable */
SrvInput_TypeDef InputObj = {
    .init = SrvInput_Init,
    .sample = SrvInput_Sample,
    .get_data = SrvInput_GetData,
};

static void SrvInput_Init(void)
{
    memset(&InputData, 0, sizeof(InputData));

    /* left gimbal io init */
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel0);
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel1);

    /* right gimbal io init */
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel2);
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel3);

    /* pot on both side io init */
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel4);
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel5);

    /* toggle io init */

    /* button io init */

    /* encoder io init */

    /* adc init */
    DrvADC.ctl(ADC_Open, 0);
}

static void SrvInput_Sample(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
