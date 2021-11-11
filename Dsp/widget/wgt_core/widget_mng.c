#include "widget_mng.h"
#include "SrvOled.h"
#include "runtime.h"
#include "linked_list.h"
#include "UI_Controller.h"
#include "GenDsp.h"
#include "mmu.h"

#define Set_FreshStateBIT(x) 1 << x
#define Clr_FreshStateBIT(x) ~(1 << x)

#define Set_ErrorStateBIT(x) Set_FreshStateBIT(x)
#define Clr_ErrorStateBIT(x) Clr_FreshStateBIT(x)

/* internal variable */
Widget_MonitorData_TypeDef MonitorDataObj = {
    .on_show = 0,
    .LstFreshRT = 0,
    .created_widget = 0,
    .widget_used_size = 0,
    .remain_size = 0,
    .max_display_cache = 0,
    .widget_dsp_list = NULL,
};

static Widget_Handle CurActive_Widget = 0;
static uint8_t WidgetFresh_Reg = Set_FreshStateBIT(Fresh_State_DrvInit);
static uint8_t **widget_blackboard;
static uint8_t Error_Code;

/* internal function */
static WidgetObj_TypeDef *GetCur_Active_Widget(void);
static void Widget_Fusion(item_obj *item, WidgetObj_TypeDef *hdl, void *arg);
static bool Widget_ClearBlackBoard(void);
static void WIdget_ClearFreshState(WidgetFresh_State_List state);
static void Widget_SetFreshState(WidgetFresh_State_List state);
static void Widget_ClearAllFreshState(void);
static uint8_t Widget_GetFreshState(void);
static bool Widgte_CheckErrorState(Widget_Error_TypeDef error);
static void Widget_ClearErrorState(Widget_Error_TypeDef error);
static void Widget_clearAllErrorState(Widget_Error_TypeDef error);
static void Widget_SetErrorState(Widget_Error_TypeDef error);
static uint8_t Widget_GetError(void);
static bool Widget_RoutateBlackboard(void);
static bool Widget_MirrorBlackboard(void);
static bool Widget_ClearDspCache(void);

/* external widget manager config function definition */
static bool Widget_ConfigDisplay_RoutateDir(Oled_Routate_Direction_Def dir);
static bool Widget_ConfigDisplay_MirrorDir(Oled_Mirror_Direction_Def dir);

/* external widget manager function definition */
static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame);
static Widget_Control_TypeDef *Widget_CtlInterface(Widget_Handle hdl);
static bool Widget_Deleted(Widget_Handle *hdl);
static bool Widget_FreshAll(void);

/* external widget control function */
static bool Widget_SetFreshFrq(uint8_t frq);
static bool Widget_Show(void);
static bool Widget_Hide(void);
static bool Widget_Clear(void);
static bool Widget_MoveTo(uint8_t x, uint8_t y);
static bool Widget_CheckFlashTrigger(void);
static Widget_DrawFunc_TypeDef *Widget_DrawInterface(void);
static WidgetDsp_Status_List Widget_DspStatus(void);

/* widget draw function interface */
static void Widget_DrawPoint(uint8_t x, uint8_t y, bool set);
static void Widget_DrawChr(Widget_Font font, char char_dsp, uint8_t x, uint8_t y, bool col_cnv);
static void Widget_DrawStr(Widget_Font font, char *str_dsp, uint8_t x, uint8_t y, bool col_cnv);
static void Widget_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_size);
static void Widget_DrawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t line_size);
static void Widget_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
static void Widget_DrawNum(Widget_Font font, int32_t num, uint8_t x, uint8_t y, bool col_cnv);

/* for temp we init each var as null */
static WidgetUI_Utils_TypeDef WidgetUI_Interface = {
    .UI_CheckBox = NULL,
    .UI_ComboBox = NULL,
    .UI_DigInput = NULL,
    .UI_StrInput = NULL,
    .UI_ProcBar = NULL,
    .UI_VerBar = NULL,
    .UI_HorBar = NULL,
};

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
    .Clear = Widget_Clear,
    .Dsp_status = Widget_DspStatus,
};

static Widget_Config_TypeDef Widget_Config = {
    .mirror = Widget_ConfigDisplay_MirrorDir,
    .routate = Widget_ConfigDisplay_RoutateDir,
};

Widget_GenProcFunc_TypeDef Widget_Mng = {
    .config_all = &Widget_Config,
    .Create = Widget_Create,
    .Delete = Widget_Deleted,
    .Control = Widget_CtlInterface,
    .fresh_all = Widget_FreshAll,
    .trigger_fresh = Widget_CheckFlashTrigger,
};

static Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame)
{
    WidgetObj_TypeDef *widget_tmp;

    if (MonitorDataObj.max_display_cache == 0)
    {
        MonitorDataObj.max_display_cache = (SrvOled.get_range().height * SrvOled.get_range().width);
        MonitorDataObj.max_display_cache *= MAX_WIDGET_CACHE_PAGE;

        MonitorDataObj.remain_size = MonitorDataObj.max_display_cache;

        //widget_blackboard = (uint8_t **)malloc(sizeof(uint8_t *) * SrvOled.get_range().height);
        widget_blackboard = (uint8_t **)MMU_Malloc(sizeof(uint8_t *) * SrvOled.get_range().height);

        if (widget_blackboard == NULL)
            return WIDGET_CREATE_ERROR;

        for (uint8_t column_index = 0; column_index < SrvOled.get_range().height; column_index++)
        {
            //widget_blackboard[column_index] = (uint8_t *)(SrvOled.get_range().width);
            widget_blackboard[column_index] = (uint8_t *)MMU_Malloc(SrvOled.get_range().width);

            if (widget_blackboard[column_index] == NULL)
                return WIDGET_CREATE_ERROR;

            memset(widget_blackboard[column_index], NULL, SrvOled.get_range().width);
        }
    }

    //widget_tmp = (WidgetObj_TypeDef *)malloc(sizeof(WidgetObj_TypeDef));
    widget_tmp = (WidgetObj_TypeDef *)MMU_Malloc(sizeof(WidgetObj_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->cord_x = cord_x;
    widget_tmp->cord_y = cord_y;

    widget_tmp->width = width;
    widget_tmp->height = height;

    widget_tmp->frame_line_size = 1;
    widget_tmp->is_selected = false;

    widget_tmp->name = name;

    if (MonitorDataObj.remain_size < (height * width))
        return WIDGET_CREATE_ERROR;

    widget_tmp->pixel_map = (uint8_t **)MMU_Malloc(sizeof(uint8_t *) * height);

    if (widget_tmp->pixel_map == NULL)
        return WIDGET_CREATE_ERROR;

    for (uint8_t h = 0; h < height; h++)
    {
        widget_tmp->pixel_map[h] = (uint8_t *)MMU_Malloc(width);

        if (widget_tmp->pixel_map[h] == NULL)
            return WIDGET_CREATE_ERROR;

        memset(widget_tmp->pixel_map[h], NULL, width);
    }

    MonitorDataObj.remain_size -= width * height;
    MonitorDataObj.widget_used_size += width * height;

    MonitorDataObj.created_widget++;

    widget_tmp->Dsp = &WidgetDraw_Interface;
    widget_tmp->Ctl = &WidgetCtl_Interface;

    widget_tmp->item = (item_obj *)MMU_Malloc(sizeof(item_obj));

    if (widget_tmp->item == NULL)
        return WIDGET_CREATE_ERROR;

    List_ItemInit(widget_tmp->item, widget_tmp);

    widget_tmp->use_frame = show_frame;
    widget_tmp->show_state = false;

    return (Widget_Handle)widget_tmp;
}

static bool Widget_Clear(void)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    for (uint8_t row = 0; row < GetCur_Active_Widget()->height; row++)
    {
        memset(GetCur_Active_Widget()->pixel_map[row], NULL, GetCur_Active_Widget()->width);
    }

    return true;
}

static bool Widget_Deleted(Widget_Handle *hdl)
{
    uint8_t height = 0;
    uint8_t width = 0;

    if ((hdl == NULL) || ((*hdl) == 0))
        return false;

    height = ((WidgetObj_TypeDef *)(*hdl))->height;
    width = ((WidgetObj_TypeDef *)(*hdl))->width;

    MMU_Free(((WidgetObj_TypeDef *)(*hdl))->item);

    MMU_Free(((WidgetObj_TypeDef *)(*hdl))->pixel_map);

    for (uint8_t h = 0; h < height; h++)
    {
        MMU_Free(((WidgetObj_TypeDef *)(*hdl))->pixel_map[h]);
    }

    MonitorDataObj.remain_size += width * height;
    MonitorDataObj.widget_used_size -= width * height;

    if (MonitorDataObj.remain_size > MonitorDataObj.max_display_cache)
        return false;

    MMU_Free(*hdl);

    *hdl = 0;
    return true;
}

static bool Widget_CheckFlashTrigger(void)
{
    uint32_t RT = Get_CurrentRunningMs();

    if ((RT - MonitorDataObj.LstFreshRT) >= Fresh_FRQ_50Hz)
    {
        MonitorDataObj.LstFreshRT = RT;
        return true;
    }
    else
        return false;
}

static bool Widget_ConfigDisplay_RoutateDir(Oled_Routate_Direction_Def dir)
{
    MonitorDataObj.routate_dir = dir;

    return true;
}

static bool Widget_ConfigDisplay_MirrorDir(Oled_Mirror_Direction_Def dir)
{
    MonitorDataObj.mirror_dir = dir;

    return true;
}

/* still in developing */
static bool Widget_RoutateBlackboard(void)
{
}

static bool Widget_MirrorBlackboard(void)
{
    uint8_t matrix_column_tmp = 0;

    switch (MonitorDataObj.mirror_dir)
    {
    case Oled_MirrorX:
        for (uint8_t column = 0; column < SrvOled.get_range().width / 2; column++)
        {
            for (uint8_t row = 0; row < SrvOled.get_range().height; row++)
            {
                matrix_column_tmp = widget_blackboard[row][column];
                widget_blackboard[row][column] = widget_blackboard[row][SrvOled.get_range().width - 1 - column];
                widget_blackboard[row][SrvOled.get_range().width - 1 - column] = matrix_column_tmp;
            }
        }
        break;

    case Oled_MirrorY:
        for (uint8_t row = 0; row < SrvOled.get_range().height / 2; row++)
        {
            for (uint8_t column = 0; column < SrvOled.get_range().width; column++)
            {
                matrix_column_tmp = widget_blackboard[row][column];
                widget_blackboard[row][column] = widget_blackboard[SrvOled.get_range().height - 1 - row][column];
                widget_blackboard[SrvOled.get_range().height - 1 - row][column] = matrix_column_tmp;
            }
        }
        break;

    case Oled_Mirror_None:
        break;

    default:
        return false;
    }

    return true;
}

static bool Widget_ClearDspCache(void)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    for (uint8_t i = 0; i < GetCur_Active_Widget()->height; i++)
    {
        memset(GetCur_Active_Widget()->pixel_map[i], NULL, GetCur_Active_Widget()->width);
    }

    return true;
}

static bool Widget_Show(void)
{
    if (GetCur_Active_Widget() == NULL)
        return false;

    if (GetCur_Active_Widget()->use_frame)
    {
        Widget_DrawRectangle(0,
                             0,
                             GetCur_Active_Widget()->width,
                             GetCur_Active_Widget()->height,
                             1);
    }

    if (!GetCur_Active_Widget()->show_state)
    {
        GetCur_Active_Widget()->show_state = true;

        if (MonitorDataObj.widget_dsp_list != NULL)
        {
            List_Insert_Item(MonitorDataObj.widget_dsp_list, GetCur_Active_Widget()->item);
        }
        else
        {
            MonitorDataObj.widget_dsp_list = GetCur_Active_Widget()->item;
            List_Init(MonitorDataObj.widget_dsp_list, GetCur_Active_Widget()->item, by_order, NULL);
        }

        Widget_SetFreshState(Fresh_State_Prepare);

        MonitorDataObj.on_show++;
    }

    return true;
}

static bool Widget_Hide(void)
{
    if ((GetCur_Active_Widget() == NULL) || (MonitorDataObj.on_show <= 1))
        return false;

    if (GetCur_Active_Widget()->show_state)
    {
        List_Delete_Item(GetCur_Active_Widget()->item, NULL);
        Widget_SetFreshState(Fresh_State_Prepare);
        GetCur_Active_Widget()->show_state = false;

        MonitorDataObj.on_show--;
    }

    return true;
}

static WidgetDsp_Status_List Widget_DspStatus(void)
{
    if (GetCur_Active_Widget() == NULL)
        return Widget_NoExist;

    if (GetCur_Active_Widget()->show_state)
    {
        return Widget_Showing;
    }

    return Widget_Hiding;
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

static Widget_Control_TypeDef *Widget_CtlInterface(Widget_Handle hdl)
{
    if (hdl == 0)
        return NULL;

    CurActive_Widget = hdl;

    return ((WidgetObj_TypeDef *)CurActive_Widget)->Ctl;
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

static bool Widget_ClearBlackBoard(void)
{
    uint8_t width_max = SrvOled.get_range().width;
    uint8_t height_max = SrvOled.get_range().height;

    for (uint8_t height = 0; height < height_max; height++)
    {
        for (uint8_t width = 0; width < width_max; width++)
        {
            if (widget_blackboard != NULL)
            {
                widget_blackboard[height][width] = 0x00;
            }
            else
                return false;
        }
    }

    return true;
}

static void Widget_Fusion(item_obj *item, WidgetObj_TypeDef *obj, void *arg)
{
    if ((obj->cord_x == 0) &&
        (obj->cord_y == 0) &&
        (obj->width >= SrvOled.get_range().width) &&
        (obj->height >= SrvOled.get_range().height))
    {
        for (uint8_t row = 0; row < SrvOled.get_range().height; row++)
        {
            memcpy(&widget_blackboard[row][0], NULL, SrvOled.get_range().width);
            memcpy(&widget_blackboard[row][0], obj->pixel_map[row], SrvOled.get_range().width);
        }
    }
    else
    {
        for (uint8_t row = obj->cord_y; row < (obj->cord_y + obj->height); row++)
        {
            memset(&widget_blackboard[row][obj->cord_x], NULL, obj->width);
            memcpy(&widget_blackboard[row][obj->cord_x], obj->pixel_map[row - obj->cord_y], obj->width);
        }
    }
}

static uint8_t Widget_GetError(void)
{
    return Error_Code;
}

static void Widget_SetErrorState(Widget_Error_TypeDef error)
{
    Error_Code |= Set_ErrorStateBIT(error);
}

static void Widget_clearAllErrorState(Widget_Error_TypeDef error)
{
    Error_Code = 0;
}

static void Widget_ClearErrorState(Widget_Error_TypeDef error)
{
    Error_Code &= Clr_ErrorStateBIT(error);
}

static bool Widgte_CheckErrorState(Widget_Error_TypeDef error)
{
    uint8_t check_error = 0;

    if (Error_Code == 0)
        return false;

    check_error |= Set_ErrorStateBIT(error);

    if (Error_Code & check_error)
    {
        return true;
    }
    else
        return false;
}

static uint8_t Widget_GetFreshState(void)
{
    return WidgetFresh_Reg;
}

static void Widget_ClearAllFreshState(void)
{
    WidgetFresh_Reg = 0;
}

static void Widget_SetFreshState(WidgetFresh_State_List state)
{
    if (WidgetFresh_Reg & (1 << Fresh_State_DrvError))
    {
        return;
    }

    WidgetFresh_Reg |= Set_FreshStateBIT(state);
}

static void Widget_ClearFreshState(WidgetFresh_State_List state)
{
    WidgetFresh_Reg &= Clr_FreshStateBIT(state);
}

//fresh all widget
static bool Widget_FreshAll(void)
{
    static uint8_t reg_checker = 0;
    WidgetObj_TypeDef *tmp = NULL;

    if (Widget_GetFreshState() & Set_FreshStateBIT(Fresh_State_Sleep))
    {
        Widget_SetFreshState(Fresh_State_Prepare);
        reg_checker = 0;
    }

    while (Widget_GetFreshState())
    {
        if ((1 << reg_checker) & Widget_GetFreshState())
        {
            switch (reg_checker)
            {
            case Fresh_State_DrvInit:
                if (SrvOled.init())
                {
                    Widget_ClearFreshState(Fresh_State_DrvInit);
                    Widget_SetFreshState(Fresh_State_Prepare);
                }
                else
                {
                    Widget_SetErrorState(Widget_ScreenInit_Error);
                    Widget_SetFreshState(Fresh_State_DrvError);
                }
                break;

            case Fresh_State_Prepare:
                if (Widget_ClearBlackBoard())
                {
                    Widget_ClearFreshState(Fresh_State_Prepare);
                    Widget_SetFreshState(Fresh_State_Reguler);
                }
                else
                    Widget_SetFreshState(Fresh_State_DrvError);
                break;

            case Fresh_State_Reguler:
                if (MonitorDataObj.created_widget > 0)
                {
                    if (MonitorDataObj.on_show >= 1)
                    {
                        if (List_traverse(MonitorDataObj.widget_dsp_list, Widget_Fusion, NULL, pre_callback) == list_no_error)
                        {
                            Widget_RoutateBlackboard();
                            Widget_MirrorBlackboard();
                            SrvOled.fresh(widget_blackboard);
                        }
                        else
                        {
                            Widget_SetErrorState(Widget_DisplapList_Error);
                            Widget_SetFreshState(Fresh_State_DrvError);
                            break;
                        }
                    }
                    else
                    {
                        Widget_SetErrorState(Widget_DisplayItem_None);
                        Widget_SetFreshState(Fresh_State_DrvError);
                        break;
                    }

                    Widget_ClearFreshState(Fresh_State_Reguler);
                    Widget_SetFreshState(Fresh_State_Sleep);
                }
                break;

            case Fresh_State_Sleep:
                return true;

            case Fresh_State_DrvError:
                return false;

            default:
                return false;
            }
        }

        reg_checker++;

        if (reg_checker == Fresh_State_Sum)
        {
            reg_checker = 0;
        }
    }
}

static void Widget_DrawPoint(uint8_t x, uint8_t y, bool set)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_point(tmp->pixel_map,
                                x,
                                y,
                                set);
}

static void Widget_DrawNum(Widget_Font font, int32_t num, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_num(font,
                              tmp->pixel_map,
                              num,
                              x,
                              y,
                              col_cnv);
}

static void Widget_DrawChr(Widget_Font font, char char_dsp, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_char(font,
                               tmp->pixel_map,
                               char_dsp,
                               x,
                               y,
                               col_cnv);
}

static void Widget_DrawStr(Widget_Font font, char *str_dsp, uint8_t x, uint8_t y, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_str(font,
                              tmp->pixel_map,
                              str_dsp,
                              x,
                              y,
                              col_cnv);
}

static void Widget_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_line(tmp->pixel_map,
                               start_x,
                               start_y,
                               end_x,
                               end_y,
                               line_size);
}

static void Widget_DrawCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_circle(tmp->pixel_map,
                                 center_x,
                                 center_y,
                                 radius,
                                 line_size);
}

static void Widget_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_rectangle(tmp->pixel_map,
                                    x,
                                    y,
                                    width,
                                    height,
                                    line_size);
}

static uint32_t widgetUI_Creat_Button()
{
    UI_ButtonObj_TypeDef *btn = NULL;

    btn = (UI_ButtonObj_TypeDef *)MMU_Malloc(sizeof(UI_ButtonObj_TypeDef));

    /* init button */
    UI_Button_Init(btn, (uint32_t)GetCur_Active_Widget(), );

    return btn;
}

static void widgetUI_Button_Ctl(UI_CheckBoxObj_TypeDef *obj)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}

static void widgetUI_ComboBox_Ctl()
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}

static void widgetUI_CheckBox_Ctl()
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}

static void widgetUI_DigInput_Ctl()
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}

static void widgetUI_StrInput_Ctl()
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}
