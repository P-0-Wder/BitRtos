#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "oledfont.h"
#include "linked_list.h"

#define UI_DEFAULT_WIDTH 0
#define UI_DEFAULT_HEIGHT 0

#define LABEL_COMBINE(x) x##" : "
#define MAX_DROP_ITEM 20
#define MAX_COMBOBOX_ITEM 20
#define Default_Font Font_12
#define Default_Button_FrameRadius 2

#define DEFAULT_FONT font_12
#define DEFAULT_BUTTON_PUSH_LABEL "yes"
#define DEFAULT_BUTTON_RELEASE_LABEL "no"
#define DEFAULT_BUTTON_WIDTH 25
#define DEFAULT_BUTTON_HEIGHT 14
#define DEFAULT__BUTTON_RADIUS 3

typedef uint32_t UI_GenCTL_Handle;
typedef UI_GenCTL_Handle UI_Button_Handle;

typedef uint32_t UI_Handler;
typedef void (*UI_DrawPoint)(int8_t x, int8_t y, bool state);
typedef void (*UI_DrawLine)(int8_t x, int8_t y, uint8_t len, uint8_t line_width);
typedef void (*UI_DrawRadiusRectangle)(int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width);
typedef void (*UI_DrawRectangle)(int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t line_width);
typedef void (*UI_DrawCircle)(int8_t cneter_x, int8_t cneter_y, uint8_t radius, uint8_t line_width);
typedef void (*UI_DrawStr)(uint8_t font, char *str, int8_t x, int8_t y, bool inv);

typedef void (*UI_FillRectangle)(int8_t x, int8_t y, uint8_t width, uint8_t height);
typedef void (*UI_FillCircle)(int8_t x, int8_t y, uint8_t radius);
typedef void (*UI_FillRadiusRectangle)(int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius);

typedef int (*UI_Trigger_Callback)(void);

typedef enum
{
    UI_Type_Button = 0,

    UI_Type_Sum,
} WidgetUI_Type_List;

/*
* use linked list to distinguish which item in created ui item list.
* need to be frsehed on the dispaly blackboard 
*/
typedef struct
{
    WidgetUI_Type_List type;
    UI_GenCTL_Handle Handler;
} WidgetUI_Item_TypeDef;

typedef struct
{
    UI_DrawPoint draw_point;
    UI_DrawLine draw_line;
    UI_DrawRadiusRectangle draw_radius_rectangle;
    UI_DrawRectangle draw_rectangle;
    UI_DrawCircle draw_circle;
    UI_DrawStr draw_str;

    UI_FillRectangle fill_rectangle;
    UI_FillCircle fill_circle;
    UI_FillRadiusRectangle fill_radius_rectangle;
} UI_DrawInterface_TypeDef;

typedef struct
{
    char *label;
    int8_t x;
    int8_t y;

    bool init;
} UI_GeneralData_TypeDef;

typedef enum
{
    Reset_Btn = 0,
    Lock_Btn,
} UI_Button_Type;

typedef enum
{
    Push_Trigger = 0,
    Release_Trigger,
} UI_Button_Trigger_Type;

typedef enum
{
    UI_Btn_RlsUp = 0,
    UI_Btn_PushDwn = 1,
} UI_Button_State_List;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    UI_Button_State_List default_state;
    UI_Button_State_List state;
    UI_Button_Type type;
    UI_Trigger_Callback push_callback;
    UI_Trigger_Callback release_callback;

    char *PushDown_Label;
    char *Release_Label;

    uint8_t width;
    uint8_t height;
} UI_ButtonObj_TypeDef;

typedef struct
{
    bool (*init)(UI_ButtonObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
    bool (*set_trogger_callback)(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_Trigger_Callback callback);
    bool (*set_label)(UI_ButtonObj_TypeDef *Obj, UI_Button_State_List state, char *Rls_Lbl);
    bool (*push)(UI_ButtonObj_TypeDef *obj);
    bool (*release)(UI_ButtonObj_TypeDef *obj);
    bool (*ctl)(UI_ButtonObj_TypeDef *obj);
    bool (*move)(UI_ButtonObj_TypeDef *obj, uint8_t x, uint8_t y);
} UI_Button_Interface_TypeDef;

typedef enum
{
    ProcBar_MoveDir_Default = 0,
    ProcBar_MoveDir_Left = 0,
    ProcBar_MoveDir_Right = 1,
} ProcessBar_MoveDir_TypeDef;

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

void UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawLine line,
                         UI_DrawRectangle rectangle,
                         UI_DrawRadiusRectangle radius_rectangle,
                         UI_DrawCircle circle,
                         UI_DrawStr str,
                         UI_FillCircle fill_circle,
                         UI_FillRectangle fill_rectangle,
                         UI_FillRadiusRectangle fill_radius_rectangle);

extern UI_Button_Interface_TypeDef UI_Button;
#endif
