#include "SrvInput.h"

/* internal variable */
static SrvInput_Data_TypeDef InputData;

/* IO object list */
/* encoder pin */

/* input hardware abstract object */
static DevEncoder_Obj_TypeDef Encoder_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_L_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_R_Obj;
//static DevButton_TypeDef Btn_LShoulder_Obj;
//static DevButton_TypeDef Btn_RShoulder_Obj;
//static DirButton_Obj_TypeDef Btn_5Dir_Obj;

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
    //DevEncoder.open(&Encoder_Obj, , );
}

static void SrvInput_Update(void)
{
}

static SrvInput_Data_TypeDef SrvInput_GetData(void)
{
    return InputData;
}
