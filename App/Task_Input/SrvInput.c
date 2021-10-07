#include "SrvInput.h"
#include "Input_IO_Def.h"

/* internal variable */
static SrvInput_Data_TypeDef InputData;

/* input hardware abstract object */
static DevEncoder_Obj_TypeDef Encoder_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_L_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_R_Obj;
//static DevButton_TypeDef Btn_LShoulder_Obj;
//static DevButton_TypeDef Btn_RShoulder_Obj;
//static DirButton_Obj_TypeDef Btn_5Dir_Obj;

/* internal funciton */
static SrvInput_Error_List SrvInput_Init(void);
static SrvInput_Error_List SrvInput_Update(void);
static SrvInput_Data_TypeDef SrvInput_GetData(void);

/* internal function */

/* external variable */
SrvInput_TypeDef InputObj = {
    .init = SrvInput_Init,
    .sample = SrvInput_Update,
    .get_data = SrvInput_GetData,
};

static SrvInput_Error_List SrvInput_Init(void)
{
    /* encoder pin */
    DrvGPIO_Obj_TypeDef EncPin[Encoder_IO_Sum];

    EncPin[Encoder_IO_A] = Encoder_A_Pin;
    EncPin[Encoder_IO_B] = Encoder_B_Pin;
    EncPin[Encoder_IO_Btn] = Encoder_Btn;

    if (!DevEncoder.open(&Encoder_Obj, EncPin, true))
        return SrvInput_Encoder_Error;

    return SrvInput_NoError;
}

static SrvInput_Error_List SrvInput_Update(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
