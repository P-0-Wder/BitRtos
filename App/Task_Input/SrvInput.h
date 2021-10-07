#ifndef __SRV_INPUT_H
#define __SRV_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Dev_Toggle.h"
#include "Dev_5DirButton.h"
#include "Dev_Gimbal.h"
#include "Dev_Button.h"

#define GIMBAL_AXIS_NUM 2
#define GIMBAL_RANGE 500
#define POT_RANGE GIMBAL_RANGE

typedef struct
{
    uint16_t gimbal_l[GIMBAL_AXIS_NUM];
    uint16_t gimbal_r[GIMBAL_AXIS_NUM];

    uint16_t pot_l;
    uint16_t pot_r;

    bool zl_btn;
    bool zr_btn;

    DevToggle_Pos_List zl_tog;
    DevToggle_Pos_List zr_tog;

    DirButton_Val_List dir_btn;

    uint16_t encdoer_val;
    bool encoder_btn;

    bool pwr_btn;
} SrvInput_Data_TypeDef;

typedef struct
{
    void (*init)(void);
    void (*sample)(void);
    SrvInput_Data_TypeDef (*get_data)(void);
} SrvInput_TypeDef;

extern SrvInput_TypeDef InputObj;

#endif
