#ifndef __TASK_INPUT_H
#define __TASK_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Dev_Toggle.h"
#include "Dev_5DirButton.h"
#include "Dev_Gimbal.h"
#include "Dev_Button.h"
#include "Dev_Encoder.h"

#define GIMBAL_RANGE 500

typedef enum
{
    SrvInput_NoError = 0,
    SrvInput_Initial,
    SrvInput_Encoder_Error,
    SrvInput_Gimbal_Error,
    SrvInput_Button_Error,
    SrvInput_Toggle_Error,
    SrvInput_5DirButton_Error,
} SrvInput_Error_List;

typedef struct
{
    DevGimbal_Val_TypeDef Gimbal_L;
    DevGimbal_Val_TypeDef Gimbal_R;

    bool zl_btn;
    bool zr_btn;

    Encoder_Data_TypeDef Enc_Val;

    DevToggle_Pos_List zl_tog;
    DevToggle_Pos_List zr_tog;

    DirButton_Val_List dir_btn;
    SrvInput_Error_List error;
} SrvInput_Data_TypeDef;

typedef struct
{
    SrvInput_Error_List (*init)(void);
    SrvInput_Error_List (*sample)(void);
    SrvInput_Data_TypeDef (*get_data)(void);
} SrvInput_TypeDef;

extern SrvInput_TypeDef InputObj;

#endif
