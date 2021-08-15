#include "widget_mng.h"
#include "GenDsp.h"
#include "oled.h"

/* internal variable */
Widget_MonitorData_TypeDef MonitorDataObj = {
    .created_widget = 0,
    .widget_used_size = 0,
    .remain_size = 0,
    .max_display_cache = 0,
};

/* internal function */

/* external function */
static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name);
static bool Widget_Deleted(Widget_Handle hdl);
static void Widget_Show(Widget_Handle hdl);
static void Widget_Hide(Widget_Handle hdl);
static void Widget_MovdeDis(Widget_Handle hdl, int8_t x, int8_t y);
static void Widget_MoveTo(Widget_Handle hdl, uint8_t x, uint8_t y);
static void Widget_FreshAll(void);

Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name)
{
    WidgetObj_TypeDef *widget_tmp;

    if (MonitorDataObj.max_display_cache == 0)
    {
        MonitorDataObj.max_display_cache = (DrvOled.get_max_height() * DrvOled.get_max_width()) * MAX_WIDGET_CACHE_PAGE;
        MonitorDataObj.remain_size = MonitorDataObj.max_display_cache;
    }

    widget_tmp = (WidgetObj_TypeDef *)malloc(sizeof(WidgetObj_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->cord_x = cord_x;
    widget_tmp->cord_y = cord_y;

    widget_tmp->width = width;
    widget_tmp->height = height;

    widget_tmp->name = name;
    widget_tmp->on_layer = DEFAULT_LAYER;

    if (MonitorDataObj.remain_size < (height * width))
        return WIDGET_CREATE_ERROR;

    widget_tmp->pixel_map = (uint8_t **)malloc(sizeof(uint8_t *) * height);

    if (widget_tmp->pixel_map == NULL)
        return WIDGET_CREATE_ERROR;

    for (uint8_t h = 0; h < height; h++)
    {
        widget_tmp->pixel_map[h] = (uint8_t *)malloc(width);

        if (widget_tmp->pixel_map == NULL)
            return WIDGET_CREATE_ERROR;
    }

    MonitorDataObj.remain_size -= width * height;
    MonitorDataObj.widget_used_size += width * height;

    widget_tmp->state = Widget_Created;
    MonitorDataObj.created_widget++;

    widget_tmp->Dsp = &GenDsp_Interface;

    return (Widget_Handle)widget_tmp;
}

bool Widget_Deleted(Widget_Handle hdl)
{

    return false;
}

void Widget_FreshAll(void)
{
}
