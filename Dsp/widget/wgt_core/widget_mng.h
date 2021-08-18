#ifndef __WIDGET_MNG_H
#define __WIDGET_MNG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "GenDsp.h"
#include "pixel.h"

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
    Fresh_FRQ_1Hz = 0,
    Fresh_FRQ_5Hz,
    Fresh_FRQ_10Hz,
    Fresh_FRQ_20Hz,
    Fresh_FRQ_25Hz,
    Fresh_FRQ_50Hz,
    Fresh_FRQ_100Hz,
} Widget_FreshFrq_List;

typedef enum
{
    Fresh_State_DrvInit,
    Fresh_State_DrvError,
    Fresh_State_Reguler,
    Fresh_State_Sleep,
} WidgetFresh_State_List;

typedef struct
{
    uint8_t fresh_duration;
    uint8_t created_widget;
    uint16_t widget_used_size;
    uint16_t remain_size;
    uint16_t max_display_cache;
} Widget_MonitorData_TypeDef;

typedef enum
{
    Widget_Monitor_Init,
    WIdget_Operate_Exectue,
} WidgetState_List;

typedef struct
{
    void (*draw_point)(uint8_t **map, uint8_t x, uint8_t y, bool set);
    void (*draw_circle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_size);
    void (*draw_char)(GenFont_List font, uint8_t **map, char c, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_str)(GenFont_List font, uint8_t **map, char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint8_t **map, uint32_t num, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
    void (*draw_line)(uint8_t **map, uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_width);
} Widget_DrawFunc_TypeDef;

typedef struct
{
    bool (*Show)(void);
    bool (*Hide)(void);
    bool (*Move)(uint8_t x, uint8_t y);
    Widget_DrawFunc_TypeDef *(*Draw)(Widget_Handle hdl);
} Widget_Control_TypeDef;

typedef struct
{
    bool use_frame;
    uint8_t frame_line_size;
    bool is_selected;
    bool on_show;

    uint8_t on_layer;
    uint8_t cord_x;
    uint8_t cord_y;
    uint8_t width;
    uint8_t height;
    uint8_t **pixel_map;

    uint8_t Sub_Widget_Num;

    Widget_DrawFunc_TypeDef *Dsp; //widget draw function block
    Widget_DrawFunc_TypeDef *Ctl; //widget control function block

    char *name;

    uint8_t level;
} WidgetObj_TypeDef;

typedef struct
{
    Widget_Handle (*Create)(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name);
    Widget_Control_TypeDef *(*Control)(Widget_Handle hdl);
    bool (*Delete)(Widget_Handle *hdl);
    bool (*set_freshFrq)(uint8_t frq);
    bool (*fresh_all)(void);
} Widget_GenProcFunc_TypeDef;

extern Widget_GenProcFunc_TypeDef Widget_Mng;

#endif
