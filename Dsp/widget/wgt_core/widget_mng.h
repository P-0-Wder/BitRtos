#ifndef __WIDGET_MNG_H
#define __WIDGET_MNG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "GenDsp.h"
#include "linked_list.h"
#include "Oled_DspProp_Def.h"
#include "UI_Controller.h"

#define WIDGET_OPR_ERROR 0
#define WIDGET_CREATE_ERROR WIDGET_OPR_ERROR
#define WIDGET_DELETE_ERROR WIDGET_OPR_ERROR

#define MAX_WIDGET_CACHE_PAGE 5 //40k for widget display
#define DEFAULT_LAYER 0

#define WIDGET_CONFIG_ALL 0xFFFFFFFF

#define MAX_SUB_WIDGET_NUM 8

#define MAX_FRESH_FRQ 100

#define DEFAULT_CACHE_CLEAR_TYPE Auto_Cache_Clear

typedef GenFont_List Widget_Font;
typedef uint32_t Widget_Handle;

#define HandleToWidgetObj(x) ((WidgetObj_TypeDef *)x)

typedef enum
{
    WidgetUI_get_x = 0,
    WidgetUI_get_y,
} WidgetUI_GetGeneralInfo_List;

typedef enum
{
    Widget_NoExist = 0,
    Widget_Showing,
    Widget_Hiding,
} WidgetDsp_Status_List;

typedef enum
{
    Fresh_FRQ_1Hz = 1,
    Fresh_FRQ_5Hz = 5,
    Fresh_FRQ_10Hz = 10,
    Fresh_FRQ_20Hz = 20,
    Fresh_FRQ_25Hz = 25,
    Fresh_FRQ_50Hz = 50,
} Widget_FreshFrq_List;

typedef enum
{
    Fresh_State_DrvInit = 0,
    Fresh_State_DrvError,
    Fresh_State_Prepare,
    Fresh_State_Reguler,
    Fresh_State_Sleep,
    Fresh_State_Sum,
} WidgetFresh_State_List;

typedef enum
{
    Widget_ScreenInit_Error = 0,
    Widget_DisplapList_Error,
    Widget_DisplayItem_None,
} Widget_Error_TypeDef;

typedef struct
{
    uint8_t on_show;
    uint32_t LstFreshRT;
    uint8_t fresh_duration;
    uint8_t created_widget;
    uint32_t widget_used_size;
    uint32_t remain_size;
    uint32_t max_display_cache;
    list_obj *widget_dsp_list;

    Oled_Routate_Direction_Def routate_dir;
    Oled_Mirror_Direction_Def mirror_dir;
} Widget_MonitorData_TypeDef;

typedef enum
{
    Widget_Monitor_Init,
    WIdget_Operate_Exectue,
} WidgetState_List;

typedef enum
{
    Circle_Left_Up = 0x02,
    Circle_Left_Down = 0x04,
    Circle_Right_Up = 0x01,
    Circle_Right_Down = 0x08,
} WidgetCircle_Section_List;

#pragma pack(1)
typedef struct
{
    void (*draw_point)(int16_t x, int16_t y, bool col_inv);
    void (*draw_circle)(int16_t x, int16_t y, uint8_t radius, uint8_t line_size, bool col_inv);
    void (*draw_circle_section)(int16_t x, int16_t y, uint8_t radius, uint8_t section, uint8_t line_size, bool col_inv);
    void (*draw_char)(GenFont_List font, char c, int16_t x, int16_t y, bool col_inv);
    void (*draw_str)(GenFont_List font, char *str, int16_t x, int16_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint32_t num, int16_t x, int16_t y, bool col_inv);
    void (*draw_rectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_inv);
    void (*draw_radius_rectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size, bool col_inv);
    void (*draw_line)(int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_width, bool col_inv);
    void (*draw_vline)(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_inv);
    void (*draw_hline)(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_inv);

    void (*fill_circle)(int16_t x, int16_t y, uint8_t radius, bool col_inv);
    void (*fill_circle_section)(int16_t x, int16_t y, uint8_t radius, uint8_t section, uint8_t line_size, bool col_inv);
    void (*fill_rectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv);
    void (*fill_radius_rectangle)(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv);
} Widget_DrawFunc_TypeDef;

typedef struct
{
    UI_Button_Handle (*create)(char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
    bool (*Set_OprLabel)(UI_Button_Handle Btn_Hdl, char *psh_lbl, char *rls_lbl);
    bool (*Set_TriggerCallback)(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback Callback);
    bool (*Move)(UI_Button_Handle Btn_Hdl, int16_t x, int16_t y);
    bool (*Operate)(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type);
} WidgetUI_Button_Interface_TypeDef;

typedef struct
{
    UI_CheckBox_Handle (*create)(char *label, int16_t x, int16_t y, bool state);
    bool (*Move)(UI_CheckBox_Handle checkbox_hdl, int16_t x, int16_t y);
    bool (*Trigger)(UI_CheckBox_Handle checkbox_hdl);
    bool (*Set_CallBack)(UI_CheckBox_Handle checkbox_hdl, UI_CheckBoxTrigger_Callback callback);
} WidgetUI_CheckBox_Interface_TypeDef;

typedef struct
{
    UI_SlideBar_Handle (*create)(char *label, int16_t x, int16_t y, UI_SliderBar_Mode_List mode, int16_t max, int16_t min, int16_t start_val, int16_t step_len);
    bool (*Set_CallBack)(UI_SlideBar_Handle hdl, UI_SliderBarTrigger_Callback callback);
    bool (*Input)(UI_SlideBar_Handle hdl, int16_t *step);
    bool (*Move)(UI_SlideBar_Handle hdl, int16_t x, int16_t y);
    bool (*Trigger)(UI_SlideBar_Handle hdl);
    bool (*Select)(UI_SlideBar_Handle hdl);
} WidgetUI_SlideBar_Interface_TypeDef;

typedef struct
{
    UI_ProcessBar_Handle (*create)(char *label, UI_ProcessBar_DspType_List dsp_type, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max);
    bool (*Set_DspDir)(UI_ProcessBar_Handle hdl, UI_ProcessBar_MoveDir_TypeDef dir);
    bool (*Set_Value)(UI_ProcessBar_Handle hdl, int32_t val);
    bool (*Move)(UI_ProcessBar_Handle hdl, int16_t x, int16_t y);
} WidgetUI_ProcessBar_Interface_TypeDef;

typedef struct
{
    UI_Drop_Handle (*create)(char *label, int16_t x, int16_t y);
    bool (*add_item)(UI_Drop_Handle hdl, char *label, void *data, uint16_t data_size, UI_Drop_Callback callback);
    bool (*select_item)(UI_Drop_Handle hdl, int8_t *offset);
    bool (*Move)(UI_Drop_Handle hdl, int16_t x, int16_t y);
    bool (*Select)(UI_Drop_Handle hdl);
} WidgetUI_Drop_Interface_TypeDef;

typedef struct
{
    UI_DigInput_Handle (*create)(char *label, int16_t x, int16_t y, UI_DigInput_Type type);
} WidgetUI_DigInput_Interface_TypeDef;

typedef struct
{
    /* general UI function mathod */
    bool (*Show_Selector)(int8_t *search_offset);
    void (*Set_CoordY_Offset)(int8_t offset);
    void (*Fresh)(void);

    /* ui control interface section */
    WidgetUI_Button_Interface_TypeDef *(*Button)(void);
    WidgetUI_CheckBox_Interface_TypeDef *(*CheckBox)(void);
    WidgetUI_SlideBar_Interface_TypeDef *(*SlideBar)(void);
    WidgetUI_ProcessBar_Interface_TypeDef *(*ProcessBar)(void);
    WidgetUI_Drop_Interface_TypeDef *(*Drop)(void);
    WidgetUI_DigInput_Interface_TypeDef (*DigInput)(void);
    // void (*UI_StrInput)();
    // void (*UI_ProcBar)();
    // void (*UI_VerBar)();
    // void (*UI_HorBar)();
} WidgetUI_Utils_TypeDef;

typedef struct
{
    bool (*Show)(void);
    bool (*Hide)(void);
    bool (*Move)(uint16_t x, uint16_t y);
    bool (*Clear)(void);
    WidgetDsp_Status_List (*Dsp_status)(void);
    Widget_DrawFunc_TypeDef *(*Draw)(void);
    WidgetUI_Utils_TypeDef *(*UI)(void);
} Widget_Control_TypeDef;

typedef struct
{
    bool use_frame;
    uint8_t frame_line_size;
    bool is_selected;
    bool show_state;

    int16_t cord_x;
    int16_t cord_y;
    uint8_t width;
    uint8_t height;
    uint8_t **pixel_map;

    uint8_t Sub_Widget_Num;

    Widget_DrawFunc_TypeDef *Dsp; //widget draw function block
    Widget_Control_TypeDef *Ctl;  //widget control function block

    char *name;

    uint8_t level;
    uint8_t ui_ctl_num;
    item_obj *dsp_item;
    list_obj *UICtl_List;

    int8_t UI_CoordY_Offset;
    item_obj *CurSelected_CTL;
} WidgetObj_TypeDef;

typedef struct
{
    bool (*mirror)(Oled_Mirror_Direction_Def dir);
    bool (*routate)(Oled_Routate_Direction_Def dir);
} Widget_Config_TypeDef;

typedef struct
{
    Widget_Config_TypeDef *config_all;
    Widget_Handle (*Create)(int8_t cord_x, int8_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame);
    Widget_Control_TypeDef *(*Control)(Widget_Handle hdl);
    bool (*Delete)(Widget_Handle *hdl);
    bool (*trigger_fresh)(void);
    bool (*fresh_all)(void);
    bool (*Operate)(UI_Button_Trigger_Type type);
} Widget_GenProcFunc_TypeDef;

extern Widget_GenProcFunc_TypeDef Widget_Mng;
#pragma pack()

#endif
