#include "widget_mng.h"
#include "oled.h"

/* internal variable */
Widget_MonitorData_TypeDef MonitorDataObj = {
    .created_widget = 0,
    .widget_used_size = 0,
    .remain_size = 0,
    .max_display_cache = 0,
};

static Widget_Handle CurActive_Widget = 0;

/* internal function */
static WidgetObj_TypeDef *GetCur_Active_Widget(void);

/* external function */
static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name);
static bool Widget_Deleted(Widget_Handle hdl);
static void Widget_Show(Widget_Handle hdl);
static void Widget_Hide(Widget_Handle hdl);
static void Widget_MovdeDis(Widget_Handle hdl, int8_t x, int8_t y);
static void Widget_MoveTo(Widget_Handle hdl, uint8_t x, uint8_t y);
static void Widget_FreshAll(void);

/* widget draw function interface */
static void Widget_DrawPoint(uint8_t x, uint8_t y, bool set);
static void Widget_DrawChr(Widget_Font font, char char_dsp, uint8_t x, uint8_t y, bool col_cnv);
static void Widget_DrawStr(Widget_Font font, char *str_dsp, uint8_t x, uint8_t y, bool col_cnv);
static void Widget_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_size);
static void Widget_DrawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t line_size);
static void Widget_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
static void Widget_DrawNum(Widget_Font font, int32_t num, uint8_t x, uint8_t y, bool col_cnv);

static Widget_DrawFunc_TypeDef WidgetDraw_Interface = {
    .draw_str = Widget_DrawStr,
    .draw_rectangle = Widget_DrawRectangle,
    .draw_point = Widget_DrawPoint,
    .draw_num = Widget_DrawNum,
    .draw_line = Widget_DrawLine,
    .draw_circle = Widget_DrawCircle,
    .draw_char = Widget_DrawChr,
};

static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name)
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

    MonitorDataObj.created_widget++;

    widget_tmp->Dsp = &WidgetDraw_Interface;

    return (Widget_Handle)widget_tmp;
}

static bool Widget_Deleted(Widget_Handle hdl)
{

    return false;
}

static void Widget_Show(Widget_Handle hdl)
{
    WidgetObj_TypeDef *tmp = (WidgetObj_TypeDef *)hdl;
}

static WidgetObj_TypeDef *GetCur_Active_Widget(void)
{
    if (CurActive_Widget == 0)
        return 0;

    return (WidgetObj_TypeDef *)CurActive_Widget;
}

static Widget_DrawFunc_TypeDef *Widget_Draw(Widget_Handle hdl)
{
    WidgetObj_TypeDef *widget_tmp = (WidgetObj_TypeDef *)hdl;
    CurActive_Widget = hdl;

    return widget_tmp->Dsp;
}

//fresh all widget
static void Widget_FreshAll(void)
{
    WidgetObj_TypeDef *tmp = NULL;

    if (MonitorDataObj.created_widget > 0)
    {
        for (uint8_t widget_index = 0; widget_index < MonitorDataObj.created_widget; widget_index++)
        {
        }
    }
    //DrvOled.fresh();
}

static void Widget_CtlShow(void)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    tmp->on_show = true;
}

static void Widget_CtlHide(void)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    tmp->on_show = false;
}

static void Widget_DrawPoint(uint8_t x, uint8_t y, bool set)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_point(tmp->pixel_map,
                                x + tmp->cord_x,
                                y + tmp->cord_y,
                                set);
}

static void Widget_DrawNum(Widget_Font font, int32_t num, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_num(font,
                              tmp->pixel_map,
                              num,
                              x + tmp->cord_x,
                              y + tmp->cord_y,
                              col_cnv);
}

static void Widget_DrawChr(Widget_Font font, char char_dsp, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_char(font,
                               tmp->pixel_map,
                               char_dsp,
                               x + tmp->cord_x,
                               y + tmp->cord_y,
                               col_cnv);
}

static void Widget_DrawStr(Widget_Font font, char *str_dsp, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_str(font,
                              tmp->pixel_map,
                              str_dsp,
                              x + tmp->cord_x,
                              y + tmp->cord_y,
                              col_cnv);
}

static void Widget_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_line(tmp->pixel_map,
                               start_x + tmp->cord_x,
                               start_y + tmp->cord_y,
                               end_x + tmp->cord_x,
                               end_y + tmp->cord_y,
                               line_size);
}

static void Widget_DrawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_circle(tmp->pixel_map,
                                 center_x + tmp->cord_x,
                                 center_y + tmp->cord_y,
                                 radius,
                                 line_size);
}

static void Widget_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.draw_rectangle(tmp->pixel_map,
                                    x + tmp->cord_x,
                                    y + tmp->cord_y,
                                    width,
                                    height,
                                    line_size);
}
