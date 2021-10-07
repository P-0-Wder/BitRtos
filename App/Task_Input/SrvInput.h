#ifndef __SRV_INPUT_H
#define __SRV_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Dev_Toggle.h"
#include "Dev_5DirButton.h"
#include "Dev_Gimbal.h"
#include "Dev_Button.h"
#include "Dev_Encoder.h"

#define GIMBAL_RANGE 500

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

} SrvInput_Data_TypeDef;

typedef struct
{
    void (*init)(void);
    void (*sample)(void);
    SrvInput_Data_TypeDef (*get_data)(void);
} SrvInput_TypeDef;

extern SrvInput_TypeDef InputObj;

#endif
