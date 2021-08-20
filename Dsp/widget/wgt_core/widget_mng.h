#ifndef __WIDGET_MNG_H
#define __WIDGET_MNG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "GenDsp.h"
#include "pixel.h"
#include "linked_list.h"

#define WIDGET_OPR_ERROR 0
#define WIDGET_CREATE_ERROR WIDGET_OPR_ERROR
#define WIDGET_DELETE_ERROR WIDGET_OPR_ERROR

#define MAX_WIDGET_CACHE_PAGE 8 //64k for widget display
#define DEFAULT_LAYER 0

#define MAX_SUB_WIDGET_NUM 8

#define MAX_FRESH_FRQ 500

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
    Fresh_FRQ_100Hz = 100,
} Widget_FreshFrq_List;

typedef enum
{
    Fresh_State_DrvInit,
    Fresh_State_DrvError,
    Fresh_State_Prepare,
    Fresh_State_Reguler,
    Fresh_State_Sleep,
} WidgetFresh_State_List;

typedef struct
{
    uint32_t LstFreshRT;
    uint8_t fresh_duration;
    uint8_t created_widget;
    uint16_t widget_used_size;
    uint16_t remain_size;
    uint16_t max_display_cache;
    list_obj *widget_dsp_list;
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
    void (*draw_line)(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_width);
} Widget_DrawFunc_TypeDef;

typedef struct
{
    bool (*Show)(void);
    bool (*Hide)(void);
    bool (*Move)(uint8_t x, uint8_t y);
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
    item_obj *item;
} WidgetObj_TypeDef;

typedef struct
{
    Widget_Handle (*Create)(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame);
    Widget_Control_TypeDef *(*Control)(Widget_Handle hdl);
    bool (*Delete)(Widget_Handle *hdl);
    bool (*set_freshFrq)(uint8_t frq);
    bool (*trigger_fresh)(void);
    bool (*fresh_all)(void);
} Widget_GenProcFunc_TypeDef;

extern Widget_GenProcFunc_TypeDef Widget_Mng;

#endif
