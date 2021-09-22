#include "SrvInput.h"
#include "periph_adc.h"

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

    /* analog pin init */
    Periph_ADC_Init(GIMBAL_LEFT_X);
    Periph_ADC_Init(GIMBAL_LEFT_Y);
    Periph_ADC_Init(GIMBAL_RIGHT_X);
    Periph_ADC_Init(GIMBAL_RIGHT_Y);
    Periph_ADC_Init(POT_LEFT);
    Periph_ADC_Init(POT_RIGHT);

    /* toggle init */

    /* button init */

    /* encoder init */
}

static void SrvInput_Sample(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
