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
    WidgetUI_Type_Button = 0,

    WidgetUI_Type_Sum,
} WidgetUI_Type_List;

/*
* use linked list to distinguish which item in created ui item list.
* need to be frsehed on the dispaly blackboard 
*/
typedef struct
{
    WidgetUI_Type_List type;
    UI_GenCTL_Handle Handler;
} WidgetUI_FreshItem_TypeDef;

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

typedef struct
{
    void (*draw_point)(uint8_t x, uint8_t y, bool set);
    void (*draw_circle)(uint8_t x, uint8_t y, uint8_t radius, uint8_t line_size);
    void (*draw_char)(GenFont_List font, char c, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_str)(GenFont_List font, char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint32_t num, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_rectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
    void (*draw_radius_rectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size);
    void (*draw_line)(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_width);
    void (*draw_vline)(uint8_t x, uint8_t y, uint8_t len, uint8_t line_size);
    void (*draw_hline)(uint8_t x, uint8_t y, uint8_t len, uint8_t line_size);

    void (*fill_circle)(uint8_t x, uint8_t y, uint8_t radius);
    void (*fill_rectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*fill_radius_rectangle)(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius);
} Widget_DrawFunc_TypeDef;

typedef struct
{
    /* ui control section */
    void (*UI_Button)();
    void (*UI_CheckBox)();
    void (*UI_ComboBox)();
    void (*UI_DigInput)();
    void (*UI_StrInput)();
    void (*UI_ProcBar)();
    void (*UI_VerBar)();
    void (*UI_HorBar)();
} WidgetUI_Utils_TypeDef;

typedef struct
{
    bool (*Show)(void);
    bool (*Hide)(void);
    bool (*Move)(uint8_t x, uint8_t y);
    bool (*Clear)(void);
    WidgetDsp_Status_List (*Dsp_status)(void);
    Widget_DrawFunc_TypeDef *(*Draw)(void);
} Widget_Control_TypeDef;

typedef struct
{
    bool use_frame;
    uint8_t frame_line_size;
    bool is_selected;
    bool show_state;

    uint8_t cord_x;
    uint8_t cord_y;
    uint8_t width;
    uint8_t height;
    uint8_t **pixel_map;

    uint8_t Sub_Widget_Num;

    Widget_DrawFunc_TypeDef *Dsp; //widget draw function block
    Widget_Control_TypeDef *Ctl;  //widget control function block

    char *name;

    uint8_t level;
    item_obj *dsp_item;
    list_obj *uictl_item;
} WidgetObj_TypeDef;

typedef struct
{
    bool (*mirror)(Oled_Mirror_Direction_Def dir);
    bool (*routate)(Oled_Routate_Direction_Def dir);
} Widget_Config_TypeDef;

typedef struct
{
    Widget_Config_TypeDef *config_all;
    Widget_Handle (*Create)(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame);
    Widget_Control_TypeDef *(*Control)(Widget_Handle hdl);
    bool (*Delete)(Widget_Handle *hdl);
    bool (*trigger_fresh)(void);
    bool (*fresh_all)(void);
} Widget_GenProcFunc_TypeDef;

extern Widget_GenProcFunc_TypeDef Widget_Mng;

#endif
