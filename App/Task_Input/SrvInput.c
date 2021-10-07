#include "SrvInput.h"
#include "Dev_5DirButton.h"
#include "Dev_Button.h"
#include "Dev_Encoder.h"
#include "Dev_Toggle.h"
#include "Dev_Gimbal.h"
#include "drv_gpio.h"
#include "Input_IO_Def.h"

/* internal variable */
static SrvInput_Data_TypeDef InputData;

/* internal funciton */
static void SrvInput_Init(void);
static void SrvInput_Sample(void);
static SrvInput_Data_TypeDef SrvInput_GetData(void);

/* internal function */

/* external variable */
SrvInput_TypeDef InputObj = {
    .init = SrvInput_Init,
    .sample = SrvInput_Sample,
    .get_data = SrvInput_GetData,
};

static void SrvInput_Init(void)
{
    memset(&InputData, 0, sizeof(InputData));
}

static void SrvInput_Update(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
