#include "SrvInput.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "Input_IO_Def.h"

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

    /* voltage detect */
    DrvADC.ctl(ADC_Config_IO, ADC1_Channel6);

    /* toggle io init */
    DrvGPIO.open();
    DrvGPIO.open();

    /* shoulder button io init */
    DrvGPIO.open();
    DrvGPIO.open();

    /* 5 direction button io init */
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();

    /* power button io init */
    DrvGPIO.open();

    /* encoder button io init */
    DrvGPIO.open();

    /* sync io input init */
    DrvGPIO.open();

    /* sync io output init */
    DrvGPIO.open();

    /* encoder io init */

    /* adc init */
    DrvADC.ctl(ADC_Open, 0);
}

static void SrvInput_Update(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
