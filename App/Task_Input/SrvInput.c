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

/* internal function */
static void SrvInput_PowerButton_Check(void);
static void SrvInput_DirectionButton_Check(void);
static void SrvInput_Encoder_Check(void);
static void SrvInput_LeftGimbal_Check(void);
static void SrvInput_RightGimbal_Check(void);
static void SrcInput_SyncInput_Callback(void);

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
    DrvGPIO.open(&ShoulderBtn_Lft, GPIO_Input, NULL);
    DrvGPIO.open(&ShoulderBtn_Rgt, GPIO_Input, NULL);

    /* 5 direction button io init */
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();
    DrvGPIO.open();

    /* power button io init */
    DrvGPIO.open(&Power_Btn, GPIO_Input, NULL);

    /* encoder button io init */
    DrvGPIO.open(&Encoder_Btn, GPIO_Input, NULL);

    /* sync io output init */
    DrvGPIO.open(&Sync_Out, GPIO_Output, NULL);

    /* sync io input init */
    DrvGPIO.open(&Sync_In, GPIO_Input, (uint32_t)SrcInput_SyncInput_Callback);

    /* encoder io init */

    /* adc init */
    DrvADC.ctl(ADC_Open, 0);
}

static void SrcInput_SyncInput_Callback(void)
{
}

static void SrvInput_PowerButton_Check(void)
{
}

static void SrvInput_DirectionButton_Check(void)
{
}

static void SrvInput_Encoder_Check(void)
{
}

static void SrvInput_LeftGimbal_Check(void)
{
}

static void SrvInput_RightGimbal_Check(void)
{
}

static void SrvInput_Update(void)
{
    SrvInput_PowerButton_Check();
    SrvInput_DirectionButton_Check();
    SrvInput_Encoder_Check();
    SrvInput_LeftGimbal_Check();
    SrvInput_RightGimbal_Check();
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
