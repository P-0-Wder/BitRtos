#ifndef __UI_CONTROLLER_H
#define __UI_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "oledfont.h"
#include "runtime.h"
#include "linked_list.h"

#define MAX_INPUTSTR_LEN 16

typedef uint32_t UI_GenCTL_Handle;
typedef UI_GenCTL_Handle UI_Drop_Handle;
typedef UI_GenCTL_Handle UI_Button_Handle;
typedef UI_GenCTL_Handle UI_CheckBox_Handle;
typedef UI_GenCTL_Handle UI_SlideBar_Handle;
typedef UI_GenCTL_Handle UI_DigInput_Handle;
typedef UI_GenCTL_Handle UI_ProcessBar_Handle;

#define HandleToDropObj(x) ((UI_DropObj_TypeDef *)x)
#define HandleToDropObj(x) ((UI_DropObj_TypeDef *)x)
#define HandleToButtonObj(x) ((UI_ButtonObj_TypeDef *)x)
#define HandleToCheckBoxObj(x) ((UI_CheckBoxObj_TypeDef *)x)
#define HandleToSlideBarObj(x) ((UI_SlideBarObj_TypeDef *)x)
#define HandleToDigInputObj(x) ((UI_DigInputObj_TypeDef *)x)
#define HandleToStrInputObj(x) ((UI_StrInputObj_TypeDef *)x)
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
#define DEFAULT_PROCESSBAR_DOWNLOADTYPE_HEIGHT 5
#define DEFAULT_PROCESSBAR_DOWNLOADTYPE_RADIUS 2

#define UICTL_DEFAULT_HEIGHT 10
#define UICTL_SLIDERBAR_HEIGHT UICTL_DEFAULT_HEIGHT
#define UICTL_PROCESSBAR_DOWNLOAD_HEIGHT 20
#define UICTL_PROCESSBAR_DOT_HEIGHT 20
#define UICTL_PROCESSBAR_FRAME_HEIGHT 20
#define UICTL_CHECKBOX_HEIGHT UICTL_DEFAULT_HEIGHT
#define UICTL_DROP_HEIGHT UICTL_DEFAULT_HEIGHT
#define UICTL_DIGINPUT_HEIGHT UICTL_DEFAULT_HEIGHT
#define UICTL_STRINPUT_HEIGHT UICTL_DEFAULT_HEIGHT

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
typedef void (*UI_DrawHLine)(int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawVLine)(int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawRadiusRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawCircle)(int16_t cneter_x, int16_t cneter_y, uint8_t radius, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawCircleSection)(int16_t x, int16_t y, uint8_t radius, uint8_t option, uint8_t line_width, bool col_inv);
typedef void (*UI_DrawStr)(uint8_t font, char *str, int16_t x, int16_t y, bool inv);
typedef void (*UI_DrawDig)(uint8_t font, int16_t dig, int16_t x, int16_t y, bool inv);

typedef void (*UI_FillRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv);
typedef void (*UI_FillCircle)(int16_t x, int16_t y, uint8_t radius, bool col_inv);
typedef void (*UI_FillCircle_Section)(int16_t x, int16_t y, uint8_t radius, uint8_t section, bool col_inv);
typedef void (*UI_FillRadiusRectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv);

typedef int (*UI_ButtonTrigger_Callback)(void);
typedef int (*UI_CheckBoxTrigger_Callback)(bool state);
typedef int (*UI_SliderBarTrigger_Callback)(int16_t data);
typedef int (*UI_Drop_Callback)(void *data);
typedef int (*UI_DigInput_Callback)(void *data, uint8_t len);
typedef int (*UI_StrInput_Callback)(char *data, uint8_t len);

typedef enum
{
    UI_Type_Button = 0,
    UI_Type_CheckBox,
    UI_Type_SlideBar,
    UI_Type_ProcBar,
    UI_Type_Drop,
    UI_Type_DigInput,
    UI_Type_StrInput,

    UI_Type_Sum,
} WidgetUI_Type_List;

typedef enum
{
    UI_Fresh_Error = 0,
    UI_Fresh_Done,
    UI_Fresh_Skip,
} WidgetUI_FreshState_List;

typedef enum
{
    SliderBar_Horizon_Mode = 0,
    SilderBar_Default_Mode = SliderBar_Horizon_Mode,
    SliderBar_Vertical_Mode,
} UI_SliderBar_Mode_List;

typedef enum
{
    UI_ProcBar_DspType_LoadBar = 0,
    UI_ProcBar_DspType_DotBar,
    UI_ProcBar_DspType_FrameBar,
    UI_ProcBar_DspType_Sum,
} UI_ProcessBar_DspType_List;

typedef enum
{
    UI_ProcBar_LabelDefaultSide = 0,
    UI_ProcBar_LabelUpSide = 0,
    UI_ProcBar_LabelRightSide,
} UI_ProcessBar_LabelPos_List;

typedef enum
{
    UI_ProcBar_GrothDir_Default = 0,
    UI_ProcBar_GrothFrom_Left = 0,
    UI_ProcBar_GrothFrom_Mid = 1,
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
    UI_DrawDig draw_dig;
    UI_DrawPoint draw_point;
    UI_DrawHLine draw_line_h;
    UI_DrawVLine draw_line_v;
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
    uint8_t height;

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
    int16_t x;
    int16_t y;
    uint8_t width;
    uint8_t height;

    bool OnDsp;
    SYSTEM_RunTime ShowRt;
} CharCuser_TypeDef;

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

    int32_t min;
    int32_t max;
    int32_t range;
    int32_t cur_val;

    uint8_t width;
    uint8_t height;

    UI_ProcessBar_LabelPos_List label_pos;
    UI_ProcessBar_DspType_List Dsp_Type;
    UI_ProcessBar_MoveDir_TypeDef Mv_Dir;
} UI_ProcessBarObj_TypeDef;

typedef struct
{
    uint8_t id;
    char *describe;
    void *data;
    uint16_t data_size;
    UI_Drop_Callback callback;
} UI_DropItemDataObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    uint8_t item_cnt;

    list_obj *DropList;
    item_obj *CurDrop_Item;

    bool is_selected;

} UI_DropObj_TypeDef;

typedef enum
{
    UI_DefaultDig_Input = 0,
    UI_IntDig_Input = 0,
    UI_DoubleDig_Input,
} UI_DigInput_Type;

typedef enum
{
    DigInput_DefaultPart = 0,
    DigInput_IntPart = 0,
    DigInput_PointPart,
} UI_DigInput_SelectedPart;

typedef struct
{
    int32_t Max;
    int32_t Min;
    uint8_t effective_len;

    int32_t CurVal;
} UI_IntDigData_TypeDef;

typedef struct
{
    double Max;
    double Min;

    double CurVal;

    uint8_t effective_int_len;
    uint8_t effective_point_len;

    int32_t IntPart;
    uint32_t PointPart;
} UI_DoubleDigData_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    UI_DigInput_Type type;
    UI_IntDigData_TypeDef InputData_Int;
    UI_DoubleDigData_TypeDef InputData_Dou;

    bool selected;
    uint8_t selected_pos;
    UI_DigInput_Callback callback;
} UI_DigInputObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
    char str[MAX_INPUTSTR_LEN];
    bool selected;
    uint8_t selected_pos;
    UI_StrInput_Callback callback;
} UI_StrInputObj_TypeDef;

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
    void (*Set_Select)(UI_SlideBarObj_TypeDef *Obj, bool state);
    bool (*Get_Select)(UI_SlideBarObj_TypeDef *Obj);
} UI_SliderBar_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_DspType_List dsp_type, char *label, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max);
    bool (*Move)(UI_ProcessBarObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*set_DspDir)(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_MoveDir_TypeDef Dir);
    bool (*set_CurVal)(UI_ProcessBarObj_TypeDef *Obj, int32_t val);
    bool (*ctl)(UI_ProcessBarObj_TypeDef *Obj);
    bool (*Set_Select)(UI_ProcessBarObj_TypeDef *Obj, bool state);
    bool (*Get_Select)(UI_ProcessBarObj_TypeDef *Obj);
    bool (*Set_LabelPos)(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_LabelPos_List Pos);
} UI_ProcessBar_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_DropObj_TypeDef *Obj, char *label, int16_t x, int16_t y);
    bool (*Add_drop_item)(UI_DropObj_TypeDef *Obj, char *item_desc, void *data, uint16_t data_size, UI_Drop_Callback callback);
    bool (*Move)(UI_DropObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*Select_DropItem)(UI_DropObj_TypeDef *Obj, int8_t *select_offset);
    bool (*Set_Select)(UI_DropObj_TypeDef *Obj, bool state);
    bool (*Get_Select)(UI_DropObj_TypeDef *Obj);
    bool (*ctl)(UI_DropObj_TypeDef *Obj);
} UI_Drop_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_DigInputObj_TypeDef *Obj, char *label, int16_t x, int16_t y, UI_DigInput_Type type);
    bool (*set_range_IntInput)(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, int32_t max, int32_t min, int32_t cur);
    bool (*set_range_DouInput)(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, uint8_t efft_point_len, double max, double min, double cur);
    bool (*Move)(UI_DigInputObj_TypeDef *Obj, int16_t x, int16_t y);

    bool (*get_CurInput_Int)(UI_DigInputObj_TypeDef *Obj, int32_t *data);
    bool (*get_CurInout_Double)(UI_DigInputObj_TypeDef *Obj, double *data);
    bool (*set_callback)(UI_DigInputObj_TypeDef *Obj, UI_DigInput_Callback callback);
    int8_t (*get_effective_len)(UI_DigInputObj_TypeDef *Obj, UI_DigInput_SelectedPart part);

    bool (*Set_Select)(UI_DigInputObj_TypeDef *Obj, bool state);
    bool (*input_val)(UI_DigInputObj_TypeDef *Obj, uint8_t set_pos, int8_t *val);
    bool (*ctl)(UI_DigInputObj_TypeDef *Obj);
} UI_DigInput_Interface_TypeDef;

typedef struct
{
    bool (*init)(UI_StrInputObj_TypeDef *Obj, char *label, int16_t x, int16_t y);
    bool (*Move)(UI_StrInputObj_TypeDef *Obj, int16_t x, int16_t y);
    bool (*set_callback)(UI_StrInputObj_TypeDef *Obj, UI_StrInput_Callback callback);
    bool (*Set_Select)(UI_StrInputObj_TypeDef *Obj, bool state);
    bool (*ctl)(UI_StrInputObj_TypeDef *Obj);
} UI_StrInput_Interface_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
} UI_VerticalBarObj_TypeDef;

typedef struct
{
    UI_GeneralData_TypeDef Gen_Data;
} UI_HorizonBarObj_TypeDef;

bool UI_Set_FontType(uint8_t font);
uint8_t UI_Get_FontType(void);

void UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawHLine line_h,
                         UI_DrawVLine line_v,
                         UI_DrawRectangle rectangle,
                         UI_DrawRadiusRectangle radius_rectangle,
                         UI_DrawCircle circle,
                         UI_DrawCircleSection circle_section,
                         UI_DrawStr str,
                         UI_DrawDig dig,
                         UI_FillCircle fill_circle,
                         UI_FillCircle_Section fillcircle_section,
                         UI_FillRectangle fill_rectangle,
                         UI_FillRadiusRectangle fill_radius_rectangle);

WidgetUI_FreshState_List UI_ShowSelector(WidgetUI_Item_TypeDef *item);

void UI_Set_GetWidgetWidthMathod(UI_GetWidget_Width mathod);

extern UI_Drop_Interface_TypeDef UI_Drop;
extern UI_Button_Interface_TypeDef UI_Button;
extern UI_DigInput_Interface_TypeDef UI_DigInput;
extern UI_CheckBox_Interface_TypeDef UI_CheckBox;
extern UI_DigInput_Interface_TypeDef UI_DigInput;
extern UI_SliderBar_Interface_TypeDef UI_SlideBar;
extern UI_ProcessBar_Interface_TypeDef UI_ProcessBar;
#endif
