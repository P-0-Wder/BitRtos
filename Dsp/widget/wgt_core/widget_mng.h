#ifndef __WIDGET_MNG_H
#define __WIDGET_MNG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pixel.h"
#include "GenDsp.h"

typedef uint32_t Widget_Handle;

#define WIDGET_OPR_ERROR 0
#define WIDGET_CREATE_ERROR WIDGET_OPR_ERROR
#define WIDGET_DELETE_ERROR WIDGET_OPR_ERROR

#define MAX_WIDGET_CACHE_PAGE 10
#define DEFAULT_LAYER 0

#define MAX_SUB_WIDGET_NUM 8

typedef enum
{
    Widget_Created,
    Widget_Showing,
    Widget_Hiding,
} Widget_State_TypeDef;

typedef struct
{
    bool use_frame;
    uint8_t frame_line_size;
    bool is_active;

    Widget_State_TypeDef state;
    uint8_t on_layer;
    uint8_t cord_x;
    uint8_t cord_y;
    uint8_t width;
    uint8_t height;
    uint8_t **pixel_map;

    void *Sub_Widget[MAX_SUB_WIDGET_NUM];
    uint8_t Sub_Widget_Num;

    GeneralDispalyProc_TypeDef *Dsp;

    char *name;
} WidgetObj_TypeDef;

typedef struct
{
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
    Widget_Handle (*Create)();
    bool (*Delete)(Widget_Handle *hdl);
    bool (*MoveDis)();
    bool (*MoveTo)();
    bool (*show)();
    bool (*Hide)();
    bool (*Draw)();
} Widget_GenProcFunc_TypeDef;

#endif
