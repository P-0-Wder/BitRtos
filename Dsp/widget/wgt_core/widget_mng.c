/*
* bref:
* Need to Use OS Event Module to hook the widget process function with 
* the Input Trigger Event
* 
* Coder : 8_B!T0
*/

#include "widget_mng.h"
#include "SrvOled.h"
#include "runtime.h"
#include "linked_list.h"
#include "binary_tree.h"
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
static int Widget_Fusion(item_obj *item, WidgetObj_TypeDef *hdl, void *arg);
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
static bool Widget_Init_UIList(item_obj *first);
static WidgetUI_Item_TypeDef *WidgetUI_InsertSequence_Callback(const WidgetUI_Item_TypeDef *item_prv, const WidgetUI_Item_TypeDef *item_nxt);

/* external widget manager config function definition */
static bool Widget_ConfigDisplay_RoutateDir(Oled_Routate_Direction_Def dir);
static bool Widget_ConfigDisplay_MirrorDir(Oled_Mirror_Direction_Def dir);

/* external widget manager function definition */
static Widget_Handle Widget_Create(int16_t cord_x, int16_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame);
static Widget_Control_TypeDef *Widget_CtlInterface(Widget_Handle hdl);
static bool Widget_Deleted(Widget_Handle *hdl);
static bool Widget_FreshAll(void);

/* external widget control function */
static bool Widget_SetFreshFrq(uint8_t frq);
static bool Widget_Show(void);
static bool Widget_Hide(void);
static bool Widget_Clear(void);
static bool Widget_MoveTo(int16_t x, int16_t y);
static bool Widget_CheckFlashTrigger(void);
static Widget_DrawFunc_TypeDef *Widget_DrawInterface(void);
static WidgetDsp_Status_List Widget_DspStatus(void);

/* widget draw function interface */
static void Widget_DrawPoint(int16_t x, int16_t y, bool set);
static void Widget_DrawChr(Widget_Font font, char char_dsp, int16_t x, int16_t y, bool col_cnv);
static void Widget_DrawStr(Widget_Font font, char *str_dsp, int16_t x, int16_t y, bool col_cnv);
static void Widget_DrawLine(int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_size, bool col_cnv);
static void Widget_DrawHLine(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_cnv);
static void Widget_DrawVLine(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_cnv);
static void Widget_DrawCircle(int16_t center_x, int16_t center_y, uint8_t radius, uint8_t line_size, bool col_cnv);
static void Widget_DrawCircle_Section(int16_t center_x, int16_t center_y, uint8_t radius, uint8_t section, uint8_t line_size, bool col_cnv);
static void Widget_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_cnv);
static void Widget_DrawNum(Widget_Font font, int32_t num, int16_t x, int16_t y, bool col_cnv);
static void Widget_DrawRadiusRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size, bool col_cnv);
static void Widget_FillCircle(int16_t x, int16_t y, uint8_t radius, bool col_cnv);
static void Widget_FillCircle_Section(int16_t x, int16_t y, uint8_t radius, uint8_t section, bool col_cnv);
static void Widget_FillRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_cnv);
static void Widget_FillRadiusRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_cnv);

/* Widget UI Get Button Interface */
static WidgetUI_Button_Interface_TypeDef *WidgetUI_GetButton_Instance(void);
static WidgetUI_CheckBox_Interface_TypeDef *WidgetUI_GetCheckBox_Instance(void);
static WidgetUI_SlideBar_Interface_TypeDef *WidgetUI_GetSlideBar_Instance(void);
static WidgetUI_ProcessBar_Interface_TypeDef *WidgetUI_GetProcessBar_Instance(void);

/* general UI Mathod */
static void WidgetUI_Init(void);
static WidgetUI_Utils_TypeDef *WidgetUI_GetUtil(void);
static bool WidgetUI_SelectCtl(int8_t *search_offset);
static void WidgetUI_Fresh(void);

/* Widget UI Button Mathod */
static UI_Button_Handle WidgetUI_Creat_Button(char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
static bool WidgetUI_SetButton_OprLabel(UI_Button_Handle Btn_Hdl, char *psh_lbl, char *rls_lbl);
static bool WidgetUI_SetButton_TriggerCallback(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback Callback);
static bool WidgetUI_Move_Button(UI_Button_Handle Btn_Hdl, int16_t x, int16_t y);
static bool WidgetUI_Fresh_Button(UI_Button_Handle Btn_Hdl);
static bool WidgetUI_Button_Operate(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type);

/* Widget UI CheckBox Mathod */
static UI_CheckBox_Handle WidgetUI_Create_CheckBox(char *label, int16_t x, int16_t y, bool state);
static bool WidgetUI_CheckBox_Trigger(UI_CheckBox_Handle checkbox_hdl);
static bool WidgetUI_CheckBox_Move(UI_CheckBox_Handle checkbox_hdl, int16_t x, int16_t y);
static bool WidgetUI_Fresh_CheckBox(UI_CheckBox_Handle checkbox_hdl);
static bool WidgetUI_CheckBox_SetCallBack(UI_CheckBox_Handle checkbox_hdl, UI_CheckBoxTrigger_Callback callback);

/* Widget UI SlideBar Mathod */
static UI_SlideBar_Handle WidgetUI_Create_SlideBar(char *label, int16_t x, int16_t y, UI_SliderBar_Mode_List mode, int16_t max, int16_t min, int16_t start_val, int16_t step_len);
static bool WidgetUI_SlideBar_Move(UI_SlideBar_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_SlideBar_SetCallBack(UI_SlideBar_Handle hdl, UI_SliderBarTrigger_Callback callback);
static bool WidgetUI_SlideBar_Input(UI_SlideBar_Handle hdl, int16_t step);
static bool WidgetUI_SlideBar_Trigger(UI_SlideBar_Handle hdl);
static bool WidgetUI_Fresh_SlideBar(UI_SlideBar_Handle hdl);

/* Widget Button object Interface */
WidgetUI_Button_Interface_TypeDef WidgetUI_Button = {
    .create = WidgetUI_Creat_Button,
    .Set_OprLabel = WidgetUI_SetButton_OprLabel,
    .Set_TriggerCallback = WidgetUI_SetButton_TriggerCallback,
    .Move = WidgetUI_Move_Button,
    .Operate = WidgetUI_Button_Operate,
};

WidgetUI_CheckBox_Interface_TypeDef WidgetUI_CheckBox = {
    .create = WidgetUI_Create_CheckBox,
    .Move = WidgetUI_CheckBox_Move,
    .Set_CallBack = WidgetUI_CheckBox_SetCallBack,
    .Trigger = WidgetUI_CheckBox_Trigger,
};

WidgetUI_SlideBar_Interface_TypeDef WidgetUI_SlideBar = {
    .create = WidgetUI_Create_SlideBar,
    .Move = WidgetUI_SlideBar_Move,
    .Trigger = WidgetUI_SlideBar_Trigger,
    .Set_CallBack = WidgetUI_SlideBar_SetCallBack,
    .Input = WidgetUI_SlideBar_Input,
};

WidgetUI_ProcessBar_Interface_TypeDef WidgetUI_ProcessBar = {
    .create = NULL,
    .Move = NULL,
};

/* for temp we init each var as null */
static WidgetUI_Utils_TypeDef WidgetUI_Interface = {
    .Show_Selector = WidgetUI_SelectCtl,
    .Fresh = WidgetUI_Fresh,
    .Button = WidgetUI_GetButton_Instance,
    .CheckBox = WidgetUI_GetCheckBox_Instance,
    .SlideBar = WidgetUI_GetSlideBar_Instance,
    .ProcessBar = WidgetUI_GetProcessBar_Instance,
    // .UI_DigInput = NULL,
    // .UI_StrInput = NULL,
    // .UI_ProcBar = NULL,
    // .UI_VerBar = NULL,
    // .UI_HorBar = NULL,
};

static Widget_DrawFunc_TypeDef WidgetDraw_Interface = {
    .draw_str = Widget_DrawStr,
    .draw_rectangle = Widget_DrawRectangle,
    .draw_radius_rectangle = Widget_DrawRadiusRectangle,
    .draw_point = Widget_DrawPoint,
    .draw_num = Widget_DrawNum,
    .draw_line = Widget_DrawLine,
    .draw_hline = Widget_DrawHLine,
    .draw_vline = Widget_DrawVLine,
    .draw_circle = Widget_DrawCircle,
    .draw_circle_section = Widget_DrawCircle_Section,
    .draw_char = Widget_DrawChr,
    .fill_circle = Widget_FillCircle,
    .fill_circle_section = Widget_FillCircle_Section,
    .fill_radius_rectangle = Widget_FillRadiusRectangle,
    .fill_rectangle = Widget_FillRectangle,
};

static Widget_Control_TypeDef WidgetCtl_Interface = {
    .Show = Widget_Show,
    .Hide = Widget_Hide,
    .Move = Widget_MoveTo,
    .Draw = Widget_DrawInterface,
    .Clear = Widget_Clear,
    .Dsp_status = Widget_DspStatus,
    .UI = WidgetUI_GetUtil,
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

static Widget_Handle Widget_Create(int16_t cord_x, int16_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame)
{
    WidgetObj_TypeDef *widget_tmp;

    /* if is first time create a widget then init the widget_mng first */
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

        /* init ui module */
        WidgetUI_Init();
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

    widget_tmp->dsp_item = (item_obj *)MMU_Malloc(sizeof(item_obj));

    if (widget_tmp->dsp_item == NULL)
        return WIDGET_CREATE_ERROR;

    List_ItemInit(widget_tmp->dsp_item, widget_tmp);

    widget_tmp->use_frame = show_frame;
    widget_tmp->show_state = false;

    //clear ui controller first
    widget_tmp->UICtl_List == NULL;
    widget_tmp->ui_ctl_num = 0;
    widget_tmp->UI_CoordY_Offset = 0;
    widget_tmp->CurSelected_CTL = NULL;

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

    height = (HandleToWidgetObj(*hdl))->height;
    width = (HandleToWidgetObj(*hdl))->width;

    MMU_Free((HandleToWidgetObj(*hdl))->dsp_item);

    MMU_Free((HandleToWidgetObj(*hdl))->pixel_map);

    for (uint8_t h = 0; h < height; h++)
    {
        MMU_Free((HandleToWidgetObj(*hdl))->pixel_map[h]);
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
                             1,
                             true);
    }

    if (!GetCur_Active_Widget()->show_state)
    {
        GetCur_Active_Widget()->show_state = true;

        if (MonitorDataObj.widget_dsp_list != NULL)
        {
            List_Insert_Item(MonitorDataObj.widget_dsp_list, GetCur_Active_Widget()->dsp_item);
        }
        else
        {
            MonitorDataObj.widget_dsp_list = GetCur_Active_Widget()->dsp_item;
            List_Init(MonitorDataObj.widget_dsp_list, GetCur_Active_Widget()->dsp_item, by_order, NULL);
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
        List_Delete_Item(GetCur_Active_Widget()->dsp_item, NULL);
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

static bool Widget_MoveTo(int16_t x, int16_t y)
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

    return HandleToWidgetObj(CurActive_Widget)->Ctl;
}

static WidgetObj_TypeDef *GetCur_Active_Widget(void)
{
    if (CurActive_Widget == 0)
        return 0;

    return HandleToWidgetObj(CurActive_Widget);
}

static Widget_DrawFunc_TypeDef *Widget_Draw(Widget_Handle hdl)
{
    WidgetObj_TypeDef *widget_tmp = HandleToWidgetObj(hdl);
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

static int Widget_Fusion(item_obj *item, WidgetObj_TypeDef *obj, void *arg)
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

    return 0;
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

/************************************** widget draw interface ******************************************/
static void Widget_DrawPoint(int16_t x, int16_t y, bool set)
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

static void Widget_DrawNum(Widget_Font font, int32_t num, int16_t x, int16_t y, bool col_cnv)
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

static void Widget_DrawChr(Widget_Font font, char char_dsp, int16_t x, int16_t y, bool col_cnv)
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

static void Widget_DrawStr(Widget_Font font, char *str_dsp, int16_t x, int16_t y, bool col_cnv)
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

static void Widget_DrawLine(int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_size, bool col_cnv)
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
                               line_size,
                               col_cnv);
}

static void Widget_DrawVLine(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_vertical_line(tmp->pixel_map, x, y, len, line_size, col_cnv);
}

static void Widget_DrawHLine(int16_t x, int16_t y, uint8_t len, uint8_t line_size, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_horizon_line(tmp->pixel_map, x, y, len, line_size, col_cnv);
}

static void Widget_DrawCircle(int16_t center_x, int16_t center_y, uint8_t radius, uint8_t line_size, bool col_cnv)
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
                                 DRAW_ALL,
                                 col_cnv);
}

static void Widget_DrawCircle_Section(int16_t center_x, int16_t center_y, uint8_t radius, uint8_t section, uint8_t line_size, bool col_cnv)
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
                                 section,
                                 col_cnv);
}

static void Widget_DrawRadiusRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.draw_radius_rectangle(tmp->pixel_map,
                                           x,
                                           y,
                                           width,
                                           height,
                                           radius,
                                           line_size,
                                           col_cnv);
}

static void Widget_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_cnv)
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
                                    line_size,
                                    col_cnv);
}

static void Widget_FillRadiusRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.fill_radius_rectangle(tmp->pixel_map,
                                           x,
                                           y,
                                           width,
                                           height,
                                           radius,
                                           col_cnv);
}

static void Widget_FillCircle(int16_t x, int16_t y, uint8_t radius, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.fill_circle(tmp->pixel_map,
                                 x,
                                 y,
                                 radius,
                                 DRAW_ALL,
                                 col_cnv);
}

static void Widget_FillCircle_Section(int16_t x, int16_t y, uint8_t radius, uint8_t section, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.fill_circle(tmp->pixel_map,
                                 x,
                                 y,
                                 radius,
                                 section,
                                 col_cnv);
}

static void Widget_FillRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_cnv)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    GenDsp_Interface.set_range(tmp->cord_x,
                               tmp->cord_y,
                               tmp->width,
                               tmp->height);

    GenDsp_Interface.fill_rectangle(tmp->pixel_map,
                                    x,
                                    y,
                                    width,
                                    height,
                                    col_cnv);
}

/************************************** widget draw interface ******************************************/

/************************************** widget UI interface ******************************************/
static uint8_t WidgetUI_GetWidgetWidth(void)
{
    WidgetObj_TypeDef *widget_tmp = GetCur_Active_Widget();

    if (widget_tmp != NULL)
        return widget_tmp->width;

    return 0;
}

static void WidgetUI_Init(void)
{
    UI_Set_DspInterface(WidgetDraw_Interface.draw_point,
                        WidgetDraw_Interface.draw_line,
                        WidgetDraw_Interface.draw_rectangle,
                        WidgetDraw_Interface.draw_radius_rectangle,
                        WidgetDraw_Interface.draw_circle,
                        WidgetDraw_Interface.draw_circle_section,
                        WidgetDraw_Interface.draw_str,
                        WidgetDraw_Interface.fill_circle,
                        WidgetDraw_Interface.fill_circle_section,
                        WidgetDraw_Interface.fill_rectangle,
                        WidgetDraw_Interface.fill_radius_rectangle);

    UI_Set_GetWidgetWidthMathod(WidgetUI_GetWidgetWidth);
}

static WidgetUI_Utils_TypeDef *WidgetUI_GetUtil(void)
{
    return &WidgetUI_Interface;
}

static void WidgetUI_SetAll_CoordY_Offset(int8_t offset)
{
    GetCur_Active_Widget()->UI_CoordY_Offset = offset;
}

static void WidgetUI_GetCur_SelectedCtl()
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
}

static WidgetUI_Item_TypeDef *WidgetUI_GetCurSelected_UICTL_Item(void)
{
    return (WidgetUI_Item_TypeDef *)(GetCur_Active_Widget()->CurSelected_CTL->data);
}

static bool WidgetUI_SelectCtl(int8_t *search_offset)
{
    volatile WidgetObj_TypeDef *tmp = GetCur_Active_Widget();
    volatile item_obj *UIItem_tmp = tmp->CurSelected_CTL;

    if ((UIItem_tmp == NULL) ||
        (tmp->CurSelected_CTL == NULL))
        return false;

    if (*search_offset > tmp->ui_ctl_num)
        *search_offset = tmp->ui_ctl_num;

    if (*search_offset > 0)
    {
        for (; *search_offset > 0; UIItem_tmp = UIItem_tmp->nxt, (*search_offset)--)
        {
            if (UIItem_tmp->nxt == NULL)
                break;
        }
    }
    else if (*search_offset < 0)
    {
        for (; *search_offset < 0; UIItem_tmp = UIItem_tmp->prv, (*search_offset)++)
        {
            if (UIItem_tmp->prv == NULL)
                break;
        }
    }

    /* move UI selector on it */
    tmp->CurSelected_CTL = UIItem_tmp;

    return UI_ShowSelector((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data));
}

static bool WidgetUI_Fresh_CallBack(item_obj *UI_item)
{
    WidgetUI_Item_TypeDef *UI_Data = NULL;

    if ((UI_item == NULL) || (UI_item->data == NULL))
        return false;

    UI_Data = UI_item->data;

    switch ((uint8_t)(UI_Data->type))
    {
    case UI_Type_Button:
        return WidgetUI_Fresh_Button(UI_Data->Handler);

    case UI_Type_CheckBox:
        return WidgetUI_Fresh_CheckBox(UI_Data->Handler);

    case UI_Type_SlideBar:
        return WidgetUI_Fresh_SlideBar(UI_Data->Handler);

    default:
        return false;
    }

    return false;
}

static void WidgetUI_Fresh(void)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    List_traverse_HaltByCondition(tmp->UICtl_List, WidgetUI_Fresh_CallBack, NULL, pre_callback, false);
}

static int16_t WidgetUI_GetCoord(const WidgetUI_Item_TypeDef *item, WidgetUI_GetGeneralInfo_List option)
{
    switch ((uint8_t)item->type)
    {
    case UI_Type_Button:

        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToButtonObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToButtonObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }

        break;

    case UI_Type_CheckBox:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToCheckBoxObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToCheckBoxObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }
        break;

    case UI_Type_SlideBar:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToSlideBarObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToSlideBarObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }
        break;

    default:
        return 0;
    }
}

static WidgetUI_Item_TypeDef *WidgetUI_InsertSequence_Callback(const WidgetUI_Item_TypeDef *item_prv, const WidgetUI_Item_TypeDef *item_nxt)
{
    /* this 4 var down below can be optmized*/
    int16_t item_prv_y = 0;
    int16_t item_nxt_y = 0;
    int16_t item_prv_x = 0;
    int16_t item_nxt_x = 0;

    if ((item_prv == NULL) && (item_nxt == NULL))
        return NULL;

    if (item_prv == NULL)
        return item_nxt;

    if (item_nxt == NULL)
        return item_prv;

    item_prv_x = WidgetUI_GetCoord(item_prv, WidgetUI_get_x);
    item_prv_y = WidgetUI_GetCoord(item_prv, WidgetUI_get_y);

    item_nxt_x = WidgetUI_GetCoord(item_nxt, WidgetUI_get_x);
    item_nxt_y = WidgetUI_GetCoord(item_nxt, WidgetUI_get_y);

    if (item_prv_y > item_nxt_y)
    {
        return item_prv;
    }
    else if (item_prv_y == item_nxt_y)
    {
        if (item_prv_x >= item_nxt_x)
        {
            return item_nxt;
        }
        else
            return item_prv;
    }
    else
        return item_nxt;
}

static bool WidgetUIList_InsertItem(UI_GenCTL_Handle hdl, WidgetUI_Type_List type)
{
    WidgetUI_Item_TypeDef *UI_ItemData_tmp = NULL;
    item_obj *UI_Item = NULL;

    UI_ItemData_tmp = (WidgetUI_Item_TypeDef *)MMU_Malloc(sizeof(WidgetUI_Item_TypeDef));
    UI_Item = (item_obj *)MMU_Malloc(sizeof(item_obj));

    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    if ((hdl == 0) || (UI_Item == NULL) || (UI_ItemData_tmp == NULL) || (tmp == NULL))
        return false;

    UI_ItemData_tmp->Handler = hdl;
    UI_ItemData_tmp->type = type;

    List_ItemInit(UI_Item, UI_ItemData_tmp);

    if (tmp->UICtl_List != NULL)
    {
        List_Insert_Item(tmp->UICtl_List, UI_Item);
    }
    else
    {
        tmp->UICtl_List = UI_Item;
        tmp->CurSelected_CTL = UI_Item;

        List_Init(tmp->UICtl_List, NULL, by_condition, WidgetUI_InsertSequence_Callback);
    }

    /* insert list item */
    tmp->ui_ctl_num++;

    return true;
}

/************************************** widget Button interface ******************************************/

static WidgetUI_Button_Interface_TypeDef *WidgetUI_GetButton_Instance(void)
{
    return &WidgetUI_Button;
}

static UI_Button_Handle WidgetUI_Creat_Button(char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state)
{
    UI_ButtonObj_TypeDef *btn = NULL;

    btn = (UI_ButtonObj_TypeDef *)MMU_Malloc(sizeof(UI_ButtonObj_TypeDef));

    if ((btn == NULL) ||
        (!WidgetUIList_InsertItem(btn, UI_Type_Button)) ||
        (!UI_Button.init(btn, label, x, y, width, height, type, state)))
        return NULL;

    return (UI_Button_Handle)btn;
}

static bool WidgetUI_SetButton_OprLabel(UI_Button_Handle Btn_Hdl, char *psh_lbl, char *rls_lbl)
{
    if (Btn_Hdl == 0 ||
        !UI_Button.set_label(HandleToButtonObj(Btn_Hdl), UI_Btn_PushDwn, psh_lbl) ||
        !UI_Button.set_label(HandleToButtonObj(Btn_Hdl), UI_Btn_RlsUp, rls_lbl))
        return false;

    return true;
}

static bool WidgetUI_SetButton_TriggerCallback(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback Callback)
{
    if (Btn_Hdl == 0)
        return false;

    return UI_Button.set_trogger_callback(HandleToButtonObj(Btn_Hdl), type, Callback);
}

static bool WidgetUI_Move_Button(UI_Button_Handle Btn_Hdl, int16_t x, int16_t y)
{
    if (Btn_Hdl == 0)
        return false;

    return UI_Button.move(Btn_Hdl, x, y);
}

static bool WidgetUI_Fresh_Button(UI_Button_Handle Btn_Hdl)
{
    if (Btn_Hdl == 0)
        return false;

    HandleToButtonObj(Btn_Hdl)->Gen_Data.y += GetCur_Active_Widget()->UI_CoordY_Offset;
    if ((HandleToButtonObj(Btn_Hdl)->Gen_Data.y >= GetCur_Active_Widget()->height))
        return false;

    UI_Button.ctl(HandleToButtonObj(Btn_Hdl));
    return true;
}

/* button operate */
static bool WidgetUI_Button_Operate(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type)
{
    if ((Btn_Hdl == 0) || (type > Push_Trigger))
        return false;

    if (type == Push_Trigger)
    {
        UI_Button.push(HandleToButtonObj(Btn_Hdl));
    }
    else
    {
        UI_Button.release(HandleToButtonObj(Btn_Hdl));
    }
}
/************************************** widget Button interface ******************************************/

/************************************** widget CheckBox interface ******************************************/
static WidgetUI_CheckBox_Interface_TypeDef *WidgetUI_GetCheckBox_Instance(void)
{
    return &WidgetUI_CheckBox;
}

static UI_CheckBox_Handle WidgetUI_Create_CheckBox(char *label, int16_t x, int16_t y, bool state)
{
    UI_CheckBoxObj_TypeDef *checkbox = NULL;

    checkbox = (UI_CheckBoxObj_TypeDef *)MMU_Malloc(sizeof(UI_CheckBoxObj_TypeDef));

    if ((checkbox == NULL) ||
        (!UI_CheckBox.init(checkbox, label, x, y, state)) ||
        (!WidgetUIList_InsertItem(checkbox, UI_Type_CheckBox)))
        return NULL;

    return ((UI_CheckBox_Handle)checkbox);
}

static bool WidgetUI_CheckBox_SetCallBack(UI_CheckBox_Handle checkbox_hdl, UI_CheckBoxTrigger_Callback callback)
{
    if (checkbox_hdl == 0)
        return false;

    UI_CheckBox.Set_Callback(HandleToCheckBoxObj(checkbox_hdl), callback);

    return true;
}

static bool WidgetUI_CheckBox_Trigger(UI_CheckBox_Handle checkbox_hdl)
{
    if (checkbox_hdl == 0)
        return false;

    UI_CheckBox.Trigger(HandleToCheckBoxObj(checkbox_hdl));

    return true;
}

static bool WidgetUI_CheckBox_Move(UI_CheckBox_Handle checkbox_hdl, int16_t x, int16_t y)
{
    if (checkbox_hdl == 0)
        return false;

    return UI_CheckBox.Move(HandleToCheckBoxObj(checkbox_hdl), x, y);
}

static bool WidgetUI_Fresh_CheckBox(UI_CheckBox_Handle checkbox_hdl)
{
    if (checkbox_hdl == 0)
        return false;

    HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.y += GetCur_Active_Widget()->UI_CoordY_Offset;
    if ((HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.y >= GetCur_Active_Widget()->height))
        return false;

    UI_CheckBox.ctl(HandleToCheckBoxObj(checkbox_hdl));

    return true;
}

/************************************** widget CheckBox interface ******************************************/

/************************************** widget SlideBar interface ******************************************/
static WidgetUI_SlideBar_Interface_TypeDef *WidgetUI_GetSlideBar_Instance(void)
{
    return &WidgetUI_SlideBar;
}

static UI_SlideBar_Handle WidgetUI_Create_SlideBar(char *label, int16_t x, int16_t y, UI_SliderBar_Mode_List mode, int16_t max, int16_t min, int16_t start_val, int16_t step_len)
{
    UI_SlideBarObj_TypeDef *slidebar = NULL;

    slidebar = (UI_SlideBarObj_TypeDef *)MMU_Malloc(sizeof(UI_SlideBarObj_TypeDef));

    if ((slidebar == NULL) ||
        (!UI_SlideBar.init(slidebar, mode, label, x, y, max, min, start_val, step_len)) ||
        (!WidgetUIList_InsertItem(slidebar, UI_Type_SlideBar)))
        return NULL;

    return ((UI_SlideBar_Handle)slidebar);
}

static bool WidgetUI_SlideBar_Move(UI_SlideBar_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    UI_SlideBar.Move(HandleToSlideBarObj(hdl), x, y);

    return true;
}

static bool WidgetUI_SlideBar_SetCallBack(UI_SlideBar_Handle hdl, UI_SliderBarTrigger_Callback callback)
{
    if (hdl == 0)
        return false;

    UI_SlideBar.Set_Callbak(HandleToSlideBarObj(hdl), callback);

    return true;
}

static bool WidgetUI_SlideBar_Input(UI_SlideBar_Handle hdl, int16_t step)
{
    if (hdl == 0)
        return false;

    return UI_SlideBar.Input(HandleToSlideBarObj(hdl), step);
}

static bool WidgetUI_SlideBar_Trigger(UI_SlideBar_Handle hdl)
{
    int16_t CurBarVal;

    if (hdl == 0)
        return false;

    CurBarVal = HandleToSlideBarObj(hdl)->cur_val;

    if (HandleToSlideBarObj(hdl)->callback)
        HandleToSlideBarObj(hdl)->callback(CurBarVal);

    return true;
}

static bool WidgetUI_Select_SlideBar(UI_SlideBar_Handle hdl)
{
    bool select = false;

    if (hdl == 0)
        return false;

    if ((UI_SlideBar.Get_Select != NULL) && (UI_SlideBar.Set_Select != NULL))
    {
        select = UI_SlideBar.Get_Select(HandleToSlideBarObj(hdl));
        UI_SlideBar.Set_Select(HandleToSlideBarObj(hdl), !select);

        if (!select)
            WidgetUI_SlideBar_Trigger(hdl);

        return true;
    }

    return false;
}

static bool WidgetUI_Fresh_SlideBar(UI_SlideBar_Handle hdl)
{
    if (hdl == 0)
        return false;

    HandleToSlideBarObj(hdl)->Gen_Data.y += GetCur_Active_Widget()->UI_CoordY_Offset;
    if ((HandleToSlideBarObj(hdl)->Gen_Data.y >= GetCur_Active_Widget()->height) ||
        (HandleToSlideBarObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
        return false;

    UI_SlideBar.ctl(HandleToSlideBarObj(hdl));

    return true;
}

/************************************** widget SlideBar interface ******************************************/

/************************************** widget ProcessBar interface ******************************************/

static WidgetUI_ProcessBar_Interface_TypeDef *WidgetUI_GetProcessBar_Instance(void)
{
    return &WidgetUI_ProcessBar;
}

static UI_ProcessBar_Handle WidgetUI_Create_ProcessBar(char *label, int16_t x, int16_t y, uint8_t width, int16_t range)
{
    UI_ProcessBarObj_TypeDef *processbar = NULL;

    processbar = (UI_SlideBarObj_TypeDef *)MMU_Malloc(sizeof(UI_SlideBarObj_TypeDef));

    if ((processbar == NULL) ||
        (!UI_Process.init(processbar, label, x, y, width, range)))
        return NULL;

    return ((UI_ProcessBar_Handle)processbar);
}

static bool WidgetUI_ProcessBar_SetDspDir(UI_ProcessBar_Handle hdl, UI_ProcessBar_MoveDir_TypeDef dir)
{
    if (hdl == 0)
        return false;

    return true;
}

static bool WidgetUI_ProcessBar_SetCurValue(UI_ProcessBar_Handle hdl, int32_t val)
{
    if (hdl == 0)
        return false;

    return true;
}

static bool WidgetUI_ProcessBar_Move(UI_ProcessBar_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return true;
}

/************************************** widget ProcessBar interface ******************************************/

/************************************** widget UI interface ******************************************/
