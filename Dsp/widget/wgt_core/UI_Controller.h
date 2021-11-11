#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "linked_list.h"

#define LABEL_COMBINE(x) x##" : "
#define MAX_DROP_ITEM 20
#define MAX_COMBOBOX_ITEM 20

typedef uint32_t UI_Handler;
typedef void (*UI_DrawPoint)(uint8_t x, uint8_t y, bool state);
typedef void (*UI_DrawLine)(uint8_t x, uint8_t y, uint8_t len, uint8_t line_width);
typedef void (*UI_DrawRectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width);
typedef void (*UI_DrawCircle)(uint8_t cneter_x, uint8_t cneter_y, uint8_t radius, uint8_t line_width);
typedef void (*UI_DrawStr)(uint8_t font, char *str, uint8_t x, uint8_t y);

typedef int (*UI_Trigger_Callback)(uint32_t arg, uint32_t len);

typedef struct
{
    UI_DrawPoint draw_point;
    UI_DrawLine draw_line;
    UI_DrawRectangle draw_rectangle;
    UI_DrawCircle draw_circle;
    UI_DrawStr draw_str;
} UI_DrawInterface_TypeDef;

typedef enum
{
    Reset_Btn = 0,
    Lock_Btn,
} UI_Button_Type;

typedef enum
{
    Push_Trigger = 0,
    Release_Trigger,
    StateChange_Trigger,
    Keep_Trigger,
} UI_Button_Trigger_Type;

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

    bool selected;

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
    bool default_state;
    bool state;
    UI_Button_Type type;
    UI_Trigger_Callback callback;
    UI_Button_Trigger_Type trigger;

    uint8_t width;
    uint8_t height;
} UI_ButtonObj_TypeDef;

typedef struct
{
    char *name;
    bool selected;
    bool active;
    UI_Trigger_Callback callback;
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

bool UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawLine line,
                         UI_DrawRectangle rectangle,
                         UI_DrawCircle circle,
                         UI_DrawStr str);

bool UI_Button_Init(UI_ButtonObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_Trigger_Type trigger, bool state);

#endif
