#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum
{
    ProcBar_Move_Left = 0,
    ProcBar_Move_Right,
} ProcessBar_MoveDir_TypeDef;

typedef struct
{
    char *label;
    uint8_t x;
    uint8_t y;

    uint8_t line_width;
    bool label_roll;
    bool label_dsp;
} UI_GeneralData_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    float percent;
    uint32_t range;
    uint32_t cur_var;

    uint8_t width;
    uint8_t height;
} UI_ProcessBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    float percent;
    uint32_t range;
    uint32_t cur_val;

    uint8_t radius;
} UI_ProcessCircleObj_TypeDef;

typedef struct
{
    /* data */
} UI_VerticalBarObj_TypeDef;

typedef struct
{
    /* data */
} UI_HorizonBarObj_TypeDef;

typedef struct
{
    /* data */
} UI_CheckBoxObj_TypeDef;

typedef struct
{
    char *label;
    uint8_t item_num;
    /* data */
} UI_ComboBox_Group_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t item_num;
    uint32_t group_prt;
    /* data */
} UI_ComboBoxObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t frame_size;
    /* data */
} UI_ButtonObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t max_item_num;
    uint8_t item_height;
    uint8_t item_width;
} UI_DropListObj_TypeDef;

typedef struct
{
} UI_DropItemObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    int32_t range;
} UI_DigInputObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t max_input_len;
} UI_StrInputObj_TypeDef;

/* ui process function block */
typedef struct
{

} UI_Utils_TypeDef;

#endif
