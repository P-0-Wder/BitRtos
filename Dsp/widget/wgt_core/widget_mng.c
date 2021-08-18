#include "widget_mng.h"
#include "SrvOled.h"
#include "runtime.h"
#include "linked_list.h"

/* internal variable */
Widget_MonitorData_TypeDef MonitorDataObj = {
    .created_widget = 0,
    .widget_used_size = 0,
    .remain_size = 0,
    .max_display_cache = 0,
    .widget_dsp_list = NULL};

static Widget_Handle CurActive_Widget = 0;
static WidgetFresh_State_List WidgetFresh_State = Fresh_State_DrvInit;
static uint8_t **widget_blackboard;

/* internal function */
static WidgetObj_TypeDef *GetCur_Active_Widget(void);
static void Widget_Fusion(item_obj *item, WidgetObj_TypeDef *hdl, void *arg);
static void Widget_ClearBlackBoard(void);

/* external widget manager function definition */
static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name);
static Widget_Control_TypeDef *Widget_CtlInterface(void);
static bool Widget_Deleted(Widget_Handle *hdl);
static bool Widget_FreshAll(void);

/* external widget control function */
static bool Widget_Show(void);
static bool Widget_Hide(void);
static bool Widget_MoveTo(uint8_t x, uint8_t y);
static Widget_DrawFunc_TypeDef *Widget_DrawInterface(void);

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

static Widget_Control_TypeDef WidgetCtl_Interface = {
    .Show = Widget_Show,
    .Hide = Widget_Hide,
    .Move = Widget_MoveTo,
    .Draw = Widget_DrawInterface,
};

Widget_GenProcFunc_TypeDef Widget_Mng = {
    .Create = Widget_Create,
    .Delete = Widget_Deleted,
    .Control = Widget_CtlInterface,
    .fresh_all = Widget_FreshAll,
};

static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name)
{
    WidgetObj_TypeDef *widget_tmp;

    if (MonitorDataObj.max_display_cache == 0)
    {
        MonitorDataObj.max_display_cache = (SrvOled.get_range().height * SrvOled.get_range().width) * MAX_WIDGET_CACHE_PAGE;
        MonitorDataObj.remain_size = MonitorDataObj.max_display_cache;

        widget_blackboard = (uint8_t **)malloc(sizeof(uint8_t *) * SrvOled.get_range().height);

        if (widget_blackboard == NULL)
            return WIDGET_CREATE_ERROR;

        for (uint8_t column_index = 0; column_index < SrvOled.get_range().height; column_index++)
        {
            widget_blackboard[column_index] = (uint8_t **)malloc(sizeof(uint8_t *) * SrvOled.get_range().width);

            if (widget_blackboard[column_index] == NULL)
                return WIDGET_CREATE_ERROR;
        }
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
    widget_tmp->Ctl = &WidgetCtl_Interface;

    widget_tmp->item = (item_obj *)malloc(sizeof(item_obj));

    if (widget_tmp->item == NULL)
        return WIDGET_CREATE_ERROR;

    List_ItemInit(widget_tmp->item, widget_tmp);

    return (Widget_Handle)widget_tmp;
}

static bool Widget_Deleted(Widget_Handle *hdl)
{
    uint8_t height = 0;
    uint8_t width = 0;

    if ((hdl == NULL) || ((*hdl) == 0))
        return false;

    height = ((WidgetObj_TypeDef *)(*hdl))->height;
    width = ((WidgetObj_TypeDef *)(*hdl))->width;

    free(((WidgetObj_TypeDef *)(*hdl))->pixel_map);

    for (uint8_t h = 0; h < height; h++)
    {
        free(((WidgetObj_TypeDef *)(*hdl))->pixel_map[h]);
    }

    MonitorDataObj.remain_size += width * height;
    MonitorDataObj.widget_used_size -= width * height;

    if (MonitorDataObj.remain_size > MonitorDataObj.max_display_cache)
        return false;

    *hdl = 0;
    return true;
}

static bool Widget_SetFreshFrq(uint8_t frq)
{
    if (frq > Fresh_FRQ_100Hz)
        return false;

    MonitorDataObj.fresh_duration = frq;
    return true;
}

static bool Widget_Show(void)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    GetCur_Active_Widget()->on_show = true;

    List_Insert_Item(MonitorDataObj.widget_dsp_list, GetCur_Active_Widget()->item);

    return true;
}

static bool Widget_Hide(void)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    GetCur_Active_Widget()->on_show = false;

    List_Delete_Item(GetCur_Active_Widget()->item, NULL);
    return true;
}

static bool Widget_MoveTo(uint8_t x, uint8_t y)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    GetCur_Active_Widget()->cord_x = x;
    GetCur_Active_Widget()->cord_y = y;

    return true;
}

static Widget_DrawFunc_TypeDef *Widget_DrawInterface(void)
{
    if (GetCur_Active_Widget() == NULL)
        return NULL;

    return GetCur_Active_Widget()->Dsp;
}

static Widget_Control_TypeDef *Widget_CtlInterface(void)
{
    if (GetCur_Active_Widget() == NULL)
        return NULL;

    return GetCur_Active_Widget()->Ctl;
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

static void Widget_ClearBlackBoard(void)
{
    for (uint8_t height = 0; height < SrvOled.get_range().height; height++)
    {
        memset(&widget_blackboard[0][height], NULL, SrvOled.get_range().width);
    }
}

static void Widget_Fusion(item_obj *item, WidgetObj_TypeDef *obj, void *arg)
{
}

//fresh all widget
static bool Widget_FreshAll(void)
{
    WidgetObj_TypeDef *tmp = NULL;

    //use time difference drive widget fresh
    WidgetFresh_State = Fresh_State_Reguler;

    while (true)
    {
        switch ((uint8_t)WidgetFresh_State)
        {
        case Fresh_State_DrvInit:
            if (SrvOled.init())
            {
                WidgetFresh_State = Fresh_State_Prepare;
            }
            else
                WidgetFresh_State = Fresh_State_DrvError;
            break;

        case Fresh_State_Prepare:
            Widget_ClearBlackBoard();
            WidgetFresh_State = Fresh_State_Reguler;
            break;

        case Fresh_State_Reguler:
            if (MonitorDataObj.created_widget > 0)
            {
                List_traverse(MonitorDataObj.widget_dsp_list, Widget_Fusion, NULL);
                SrvOled.fresh(widget_blackboard);
            }
            WidgetFresh_State = Fresh_State_Sleep;
            return true;

        case Fresh_State_Sleep:
            return true;

        case Fresh_State_DrvError:
            return false;

        default:
            return false;
        }
    }
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
