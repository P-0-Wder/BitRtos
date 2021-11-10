#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "linked_list.h"

#define LABEL_COMBINE(x) x##" : "
#define MAX_DROP_ITEM 20
#define MAX_COMBOBOX_ITEM 20

typedef bool (*UI_DrawPoint)(uint8_t x, uint8_t y, bool state);
typedef bool (*UI_DrawLine)(uint8_t x, uint8_t y, uint8_t len);
typedef bool (*UI_DrawRectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_width);
typedef bool (*UI_DrawCircle)(uint8_t cneter_x, uint8_t cneter_y, uint8_t radius);

typedef int (*gen_callback)(uint32_t arg, uint32_t len);

typedef enum
{
    Auto_Reset = 0,
    No_Reset,
} UI_Button_Type;

typedef enum
{
    ProcBar_MoveDir_Default = 0,
    ProcBar_MoveDir_Left = 0,
    ProcBar_MoveDir_Right = 1,
} ProcessBar_MoveDir_TypeDef;

typedef struct
{
    char *label;
    uint8_t x;
    uint8_t y;

    uint8_t line_width;
    bool label_roll;
    bool label_dsp;

    bool selected;

    uint32_t widget_addr;
    bool init;
} UI_GeneralData_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    float percent;

    uint32_t range;
    uint32_t cur_val;

    uint8_t width;
    uint8_t height;

    ProcessBar_MoveDir_TypeDef Mv_Dir;
    UI_DrawPoint DrawPoint;
} UI_ProcessBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    float percent;
    uint32_t range;
    uint32_t cur_val;

    uint8_t radius;

    UI_DrawPoint DrawPoint;
} UI_ProcessCircleObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    UI_DrawLine DrawLine;
} UI_VerticalBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    UI_DrawLine DrawLine;
} UI_HorizonBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t frame_size;
    bool checked;

    UI_DrawRectangle DrawRectangle;
} UI_CheckBoxObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t item_id;
    void *group_ptr;
    bool checked;
    uint8_t radius;

    UI_DrawCircle DrawCircle;
} UI_ComboBoxObj_TypeDef;

typedef struct
{
    char *label;
    uint8_t item_num;
    UI_ComboBoxObj_TypeDef *Obj[MAX_COMBOBOX_ITEM];
} UI_ComboBox_Group_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    bool check_state;
    UI_Button_Type type;
    gen_callback callback;
} UI_ButtonObj_TypeDef;

typedef struct
{
    char *name;
    bool selected;
    bool active;
    gen_callback callback;
} UI_DropItemObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t item_num;
    uint8_t item_height;
    uint8_t item_width;
    UI_DropItemObj_TypeDef item[MAX_DROP_ITEM];
} UI_DropListObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    int32_t range;
    char *input_dig;
    int32_t dig;
    bool inputing;
} UI_DigInputObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t item_num;

    /* need a linked list */
    list_obj item_list;
} UI_DropObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t max_input_len;
    char *input_str;
    bool inputing;
} UI_StrInputObj_TypeDef;

typedef struct
{
    bool (*Is_Init)(UI_GeneralData_TypeDef GenData);
} UI_Controller_TypeDef;

extern UI_Controller_TypeDef UI_Controller;

#endif
