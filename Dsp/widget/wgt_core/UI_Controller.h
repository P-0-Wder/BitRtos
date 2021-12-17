#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "oledfont.h"
#include "runtime.h"
#include "linked_list.h"

typedef uint32_t UI_GenCTL_Handle;
typedef UI_GenCTL_Handle UI_Button_Handle;
typedef UI_GenCTL_Handle UI_CheckBox_Handle;
typedef UI_GenCTL_Handle UI_SlideBar_Handle;
typedef UI_GenCTL_Handle UI_ProcessBar_Handle;

#define HandleToButtonObj(x) ((UI_ButtonObj_TypeDef *)x)
#define HandleToCheckBoxObj(x) ((UI_CheckBoxObj_TypeDef *)x)
#define HandleToSlideBarObj(x) ((UI_SlideBarObj_TypeDef *)x)
#define HandleToProcessBarObj(x) ((UI_ProcessBarObj_TypeDef *)x)

#define MAX_DROP_ITEM 20
#define MAX_COMBOBOX_ITEM 20
#define Default_Font Font_8

#define DEFAULT_BUTTON_PUSH_LABEL "yes"
#define DEFAULT_BUTTON_RELEASE_LABEL "no"
#define DEFAULT_BUTTON_WIDTH 30
#define DEFAULT_BUTTON_HEIGHT 16
#define DEFAULT_BUTTON_RADIUS 3
#define DEFAULT_BUTTON_STATE UI_Btn_RlsUp

#define DEFAULT_CHECKBOX_FRAME_SIZE 7
#define DEFAULT_CHECKBOX_FILLFRAME (DEFAULT_CHECKBOX_FRAME_SIZE - 4)
#define DEFAULT_CHECKBOX_OFFSET 5
#define DEFAULT_CHECKBOX_STATE false

#define BUTTON_SELECTOR_LINE_SIZE 1
#define BUTTON_SELECTOR_RADIUS 2

#define DEFAULT_SLIDERBAR_LEN 40
#define DEFAULT_SLIDERBAR_LINESIZE 2
#define DEFAULT_SLIDERBAR_BLOCK_WIDTH 3
#define DEFAULT_SLIDERBAR_BLOCK_HIGH 6
#define DEFAULT_SLIDERBAR_OFFSET 5

#define DEFAULT_PROCESSBAR_LINE_WIDTH 1

typedef enum
{
    UICircle_Left_Up = 0x02,
    UICircle_Left_Down = 0x04,
    UICircle_Right_Up = 0x01,
    UICircle_Right_Down = 0x08,
} UICircle_Section_List;

typedef uint8_t (*UI_GetWidget_Width)(void);
typedef uint8_t (*UI_GetWidget_Height)(void);

typedef void (*UI_DrawPoint)(int16_t x, int16_t y, bool state);
typedef void (*UI_DrawLine)(int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawRadiusRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawCircle)(int16_t cneter_x, int16_t cneter_y, uint8_t radius, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawCircleSection)(int16_t x, int16_t y, uint8_t radius, uint8_t option, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawStr)(uint8_t font, char *str, int16_t x, int16_t y, bool inv);

typedef void (*UI_FillRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv);
typedef void (*UI_FillCircle)(int16_t x, int16_t y, uint8_t radius, bool col_inv);
typedef void (*UI_FillCircle_Section)(int16_t x, int16_t y, uint8_t radius, uint8_t section, bool col_inv);
typedef void (*UI_FillRadiusRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv);

typedef int (*UI_ButtonTrigger_Callback)(void);
typedef int (*UI_CheckBoxTrigger_Callback)(bool state);
typedef int (*UI_SliderBarTrigger_Callback)(int16_t data);

typedef enum
{
    UI_Type_Button = 0,
    UI_Type_CheckBox,
    UI_Type_SlideBar,
    UI_Type_ProcBar,

    UI_Type_Sum,
} WidgetUI_Type_List;

typedef enum
{
    SliderBar_Horizon_Mode = 0,
    SilderBar_Default_Mode = SliderBar_Horizon_Mode,
    SliderBar_Vertical_Mode,
} UI_SliderBar_Mode_List;

typedef enum
{
    UI_ProcBar_DspType_1 = 0,
    UI_ProcBar_DspType_2,
    UI_ProcBar_DspType_3,
    UI_ProcBar_DspType_Sum,
} UI_ProcessBar_DspType_List;

typedef enum
{
    UI_ProcBar_GrothDir_Default = 0,
    UI_ProcBar_GrothFrom_Right = 0,
    UI_ProcBar_GrothFrom_Left = 1,
    UI_ProcBar_GrothFrom_Mid = 2,
} UI_ProcessBar_MoveDir_TypeDef;

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
    UI_DrawStr draw_str;
    UI_DrawPoint draw_point;
    UI_DrawLine draw_line;
    UI_DrawRadiusRectangle draw_radius_rectangle;
    UI_DrawRectangle draw_rectangle;
    UI_DrawCircle draw_circle;
    UI_DrawCircleSection draw_circle_section;

    UI_FillRectangle fill_rectangle;
    UI_FillCircle fill_circle;
    UI_FillCircle_Section fill_circle_section;
    UI_FillRadiusRectangle fill_radius_rectangle;
} UI_DrawInterface_TypeDef;

typedef struct
{
    char *label;
    int16_t x;
    int16_t y;

    bool operatable;
} UI_GeneralData_TypeDef;

typedef enum
{
    Reset_Btn = 0,
    Lock_Btn,
} UI_Button_Type;

typedef enum
{
    Release_Trigger = 0,
    Push_Trigger,
} UI_Button_Trigger_Type;

typedef enum
{
    UI_Btn_RlsUp = 0,
    UI_Btn_PushDwn = 1,
} UI_Button_State_List;

#pragma pack(1)
typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    UI_Button_State_List default_state;
    UI_Button_State_List state;
    UI_Button_Type type;
    UI_ButtonTrigger_Callback push_callback;
    UI_ButtonTrigger_Callback release_callback;

    char *PushDown_Label;
    char *Release_Label;

    uint8_t width;
    uint8_t height;
} UI_ButtonObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    UI_CheckBoxTrigger_Callback callback;
    bool checked;
} UI_CheckBoxObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    UI_SliderBar_Mode_List mode;
    UI_SliderBarTrigger_Callback callback;

    float limit_min;
    float limit_max;
    float scale;
    int16_t step_len;
    float cur_val;

    bool is_selected;

    int16_t BarCoord_X;
    int16_t BarCoord_Y;

    /* need callback function */
} UI_SlideBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;

    float percent;

    uint32_t range;
    uint32_t cur_val;

    uint8_t width;
    uint8_t height;

    UI_ProcessBar_DspType_List dsp_type;
    UI_ProcessBar_MoveDir_TypeDef Mv_Dir;
} UI_ProcessBarObj_TypeDef;

#pragma pack()

typedef struct
{
    bool (*init)(UI_ButtonObj_TypeDef *Obj, char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
    bool (*set_trogger_callback)(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback callback);
    bool (*set_label)(UI_ButtonObj_TypeDef *Obj, UI_Button_State_List state, char *Rls_Lbl);
    bool (*push)(UI_ButtonObj_TypeDef *obj);
    bool (*release)(UI_ButtonObj_TypeDef *obj);
    bool (*ctl)(UI_ButtonObj_TypeDef *obj);
    bool (*move)(UI_ButtonObj_TypeDef *obj, int16_t x, int16_t y);
} UI_Button_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_CheckBoxObj_TypeDef *Obj, char *label, int16_t x, int16_t y, bool state);
    bool (*Set_Callback)(UI_CheckBoxObj_TypeDef *Obj, UI_CheckBoxTrigger_Callback callback);
    bool (*Move)(UI_CheckBoxObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*Trigger)(UI_CheckBoxObj_TypeDef *Obj);
    bool (*ctl)(UI_CheckBoxObj_TypeDef *Obj);
} UI_CheckBox_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_SlideBarObj_TypeDef *Obj, UI_SliderBar_Mode_List mode, char *label, int16_t x, int16_t y, int16_t limit_max, int16_t limit_min, int16_t start_val, int16_t step_len);
    bool (*Set_Callbak)(UI_SlideBarObj_TypeDef *Obj, UI_SliderBarTrigger_Callback callback);
    bool (*Move)(UI_SlideBarObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*Input)(UI_SlideBarObj_TypeDef *Obj, int16_t *step);
    bool (*Trigger)(UI_SlideBarObj_TypeDef *Obj);
    bool (*ctl)(UI_SlideBarObj_TypeDef *Obj);
    bool (*Get_Select)(UI_SlideBarObj_TypeDef *Obj);
    void (*Set_Select)(UI_SlideBarObj_TypeDef *Obj, bool state);
} UI_SliderBar_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_ProcessBarObj_TypeDef *Obj, char *label, int16_t x, int16_t y, uint8_t width, uint32_t range);
    bool (*Move)(UI_ProcessBarObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*set_DspDir)(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_MoveDir_TypeDef Dir);
    bool (*set_CurVal)(UI_ProcessBarObj_TypeDef *Obj, uint32_t val);
    bool (*ctl)(UI_ProcessBarObj_TypeDef *Obj);
} UI_ProcessBar_Interface_TypeDef;

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
    UI_ButtonTrigger_Callback callback;
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

bool UI_Set_FontType(uint8_t font);
void UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawLine line,
                         UI_DrawRectangle rectangle,
                         UI_DrawRadiusRectangle radius_rectangle,
                         UI_DrawCircle circle,
                         UI_DrawCircleSection circle_section,
                         UI_DrawStr str,
                         UI_FillCircle fill_circle,
                         UI_FillCircle_Section fillcircle_section,
                         UI_FillRectangle fill_rectangle,
                         UI_FillRadiusRectangle fill_radius_rectangle);

bool UI_ShowSelector(WidgetUI_Item_TypeDef *item);

void UI_Set_GetWidgetWidthMathod(UI_GetWidget_Width mathod);

extern UI_Button_Interface_TypeDef UI_Button;
extern UI_CheckBox_Interface_TypeDef UI_CheckBox;
extern UI_SliderBar_Interface_TypeDef UI_SlideBar;
extern UI_ProcessBar_Interface_TypeDef UI_ProcessBar;
#endif
