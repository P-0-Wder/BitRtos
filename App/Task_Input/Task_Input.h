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
#include "task_manager.h"

#define GIMBAL_RANGE 500
#define FILTER_BUFF_SIZE 11

typedef enum
{
    TaskInput_Initialize = 0,
    TaskInput_Init_Error,
    TaskInput_Updating,
} TaskInput_Stage_List;

typedef enum
{
    Input_NoError = 0,
    Input_Initial,
    Input_Encoder_Error,
    Input_Gimbal_Error,
    Input_Button_Error,
    Input_Toggle_Error,
    Input_5DirButton_Error,
} Input_Error_List;

typedef struct
{
    int16_t sample_buff[FILTER_BUFF_SIZE];
    int16_t sort_buff[FILTER_BUFF_SIZE];
    int16_t slide[FILTER_BUFF_SIZE];

    float weight_gain;
} InputAnalog_FilterObj_TypeDef;

typedef struct
{
    int16_t range_max;
    int16_t range_min;
    int16_t offset;

    double gain;
} InputAnalog_MapObj_TypeDef;

typedef struct
{
    bool use_filter;
    InputAnalog_FilterObj_TypeDef flt_obj;

    bool use_map;
    InputAnalog_MapObj_TypeDef map_obj;
} AnalogProc_Obj_TypeDef;

typedef struct
{
    AnalogProc_Obj_TypeDef X_proc_obj;
    AnalogProc_Obj_TypeDef Y_proc_obj;
} Gimbal_Proc_TypeDef;

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
    Input_Error_List error;
} Input_Data_TypeDef;

void TaskInput_Core(Task_Handler self);

extern Task_Handler SigInput_Tsk_Hdl;

#endif
