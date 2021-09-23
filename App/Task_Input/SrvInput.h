#ifndef __SRV_INPUT_H
#define __SRV_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GIMBAL_AXIS_NUM 2
#define GIMBAL_RANGE 500
#define POT_RANGE GIMBAL_RANGE

typedef enum
{
    Gimbal_Axis_X = 0,
    Gimbal_Axis_Y,
} Gimbal_Axis_TypeDef;

typedef enum
{
    tog_up = 1,
    tog_mid,
    tog_dwn,
} Toggle_Pos_List;

typedef enum
{
    dir_node = 0,
    dir_up,
    dir_lft,
    dir_mid,
    dir_rgt,
    dir_dwn,
} DirButton_Val_List;

typedef struct
{
    uint16_t gimbal_l[GIMBAL_AXIS_NUM];
    uint16_t gimbal_r[GIMBAL_AXIS_NUM];

    uint16_t pot_l;
    uint16_t pot_r;

    bool zl_btn;
    bool zr_btn;

    Toggle_Pos_List zl_tog;
    Toggle_Pos_List zr_tog;

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
