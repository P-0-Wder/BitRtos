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
static Widget_Handle Widget_Create(int16_t cord_x, int16_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame, bool show_name);
static Widget_Handle Widget_CreateSub(Widget_Handle ori, uint8_t width, uint8_t height, char *name, bool show_frame, bool show_name);
static bool Widget_SetName(Widget_Handle hdl, char *name);
static Widget_Control_TypeDef *Widget_CtlInterface(Widget_Handle hdl);
static bool Widget_Deleted(Widget_Handle *hdl);
static bool Widget_DeletedSub(Widget_Handle *hdl);
static bool Widget_ResetCurUICtl(void);
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
static WidgetUI_Drop_Interface_TypeDef *WidgetUI_GetDrop_Instance(void);
static WidgetUI_Button_Interface_TypeDef *WidgetUI_GetButton_Instance(void);
static WidgetUI_CheckBox_Interface_TypeDef *WidgetUI_GetCheckBox_Instance(void);
static WidgetUI_SlideBar_Interface_TypeDef *WidgetUI_GetSlideBar_Instance(void);
static WidgetUI_DigInput_Interface_TypeDef *WidgetUI_GetDigInput_Instance(void);
static WidgetUI_StrInput_Interface_TypeDef *WidgetUI_GetStrInput_Instance(void);
static WidgetUI_ProcessBar_Interface_TypeDef *WidgetUI_GetProcessBar_Instance(void);
static WidgetUI_TriggerLabel_Interface_TypeDef *WidgetUI_GetTriggerLabel_Instance(void);

/* general UI Mathod */
static void WidgetUI_Init(void);
static WidgetUI_Utils_TypeDef *WidgetUI_GetUtil(void);
static bool WidgetUI_SelectCtl(int8_t *search_offset);
static void WidgetUI_Fresh(void);
static void WidgetUI_SetAll_CoordY_Offset(int8_t offset);
static UI_GenCTL_Handle WidgetUI_GetCurSelected_UICtl(void);

/* Widget UI Button Mathod */
static UI_Button_Handle WidgetUI_Creat_Button(char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
static bool WidgetUI_SetButton_OprLabel(UI_Button_Handle Btn_Hdl, char *psh_lbl, char *rls_lbl);
static bool WidgetUI_SetButton_TriggerCallback(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback Callback);
static bool WidgetUI_Move_Button(UI_Button_Handle Btn_Hdl, int16_t x, int16_t y);
static bool WidgetUI_SetButton_DspType(UI_Button_Handle Btn_Hdl, UI_ButtonDsp_TypeList type);
static bool WidgetUI_Button_Operate(UI_Button_Handle Btn_Hdl, UI_Button_Trigger_Type type);
static bool WidgetUI_Fresh_Button(UI_Button_Handle Btn_Hdl);

/* Widget UI CheckBox Mathod */
static UI_CheckBox_Handle WidgetUI_Create_CheckBox(char *label, int16_t x, int16_t y, bool state);
static bool WidgetUI_CheckBox_Trigger(UI_CheckBox_Handle checkbox_hdl);
static bool WidgetUI_CheckBox_Move(UI_CheckBox_Handle checkbox_hdl, int16_t x, int16_t y);
static bool WidgetUI_CheckBox_SetCallBack(UI_CheckBox_Handle checkbox_hdl, UI_CheckBoxTrigger_Callback callback);
static bool WidgetUI_Fresh_CheckBox(UI_CheckBox_Handle checkbox_hdl);

/* Widget UI SlideBar Mathod */
static UI_SlideBar_Handle WidgetUI_Create_SlideBar(char *label, int16_t x, int16_t y, UI_SliderBar_Mode_List mode, int16_t max, int16_t min, int16_t start_val, int16_t step_len);
static bool WidgetUI_SlideBar_Move(UI_SlideBar_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_SlideBar_SetCallBack(UI_SlideBar_Handle hdl, UI_SliderBarTrigger_Callback callback);
static bool WidgetUI_SlideBar_Input(UI_SlideBar_Handle hdl, int16_t step);
static bool WidgetUI_SlideBar_Trigger(UI_SlideBar_Handle hdl);
static bool WidgetUI_Select_SlideBar(UI_SlideBar_Handle hdl);
static bool WidgetUI_Fresh_SlideBar(UI_SlideBar_Handle hdl);

/* Widget UI ProcessBar Mathod */
static UI_ProcessBar_Handle WidgetUI_Create_ProcessBar(char *label, UI_ProcessBar_DspType_List dsp_type, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max);
static bool WidgetUI_ProcessBar_SetDspDir(UI_ProcessBar_Handle hdl, UI_ProcessBar_MoveDir_TypeDef dir);
static bool WidgetUI_ProcessBar_SetCurValue(UI_ProcessBar_Handle hdl, int32_t val);
static bool WidgetUI_ProcessBar_Move(UI_ProcessBar_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_Fresh_ProcessBar(UI_ProcessBar_Handle hdl);

/* Widget UI Drop Mathod */
static UI_Drop_Handle WidgetUI_Create_Drop(char *label, int16_t x, int16_t y);
static bool WidgetUI_Drop_Move(UI_Drop_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_Drop_Select(UI_Drop_Handle hdl, bool state);
static bool WidgetUI_Drop_SelectItem(UI_Drop_Handle hdl, uint8_t *offset);
static bool WidgetUI_Add_DropItem(UI_Drop_Handle hdl, char *label, void *data, uint16_t data_size, UI_Drop_Callback callback);
static bool WidgetUI_Fresh_Drop(UI_Drop_Handle hdl);

/* Widget UI Digital Input Mathod */
static UI_DigInput_Handle WidgetUI_Create_DigInput(char *label, int16_t x, int16_t y, UI_DigInput_Type type);
static bool WidgetUI_DigInput_Setcallback(UI_DigInput_Handle hdl, UI_DigInput_Callback callback);
static bool WidgetUI_DigInput_SetIntRange(UI_DigInput_Handle hdl, uint8_t eff_len, int32_t min, int32_t max, int32_t default_val);
static bool WidgetUI_DigInput_SetDouRange(UI_DigInput_Handle hdl, uint8_t int_eff_len, uint8_t dou_eff_len, double max, double min, double cur);
static bool WidgetUI_DigInput_Move(UI_DigInput_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_DigInput_Select(UI_DigInput_Handle hdl, bool state);
static bool WidgetUI_DigInput_Value(UI_DigInput_Handle hdl, uint8_t pos, int8_t *val);
static bool WidgetUI_Fresh_DigInput(UI_DigInput_Handle hdl);

/* Widget UI String Input Mathod */
static UI_StrInput_Handle WidgetUI_Create_StrInput(char *label, int16_t x, int16_t y);
static bool WidgetUI_StrInput_Move(UI_StrInput_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_StrInput_SetCallback(UI_StrInput_Handle hdl, UI_StrInput_Callback callback);
static bool WidgetUI_StrInput_Select(UI_StrInput_Handle hdl, bool state);
static bool WidgetUI_StrInput_StrChar(UI_StrInput_Handle hdl, uint8_t pos, char input);
static bool WidgetUI_Fresh_StrInput(UI_StrInput_Handle hdl);

/* Widget UI TriggerLabel Mathod */
static UI_TriggerLabel_Handle WidgetUI_Cteate_TriggerLabel(char *label, int16_t x, int16_t y);
static bool WidgetUI_TriggerLabel_Move(UI_TriggerLabel_Handle hdl, int16_t x, int16_t y);
static bool WidgetUI_TriggerLabel_SetCallback(UI_TriggerLabel_Handle hdl, UI_TriggerLabel_Callback callback);
static bool WidgetUI_TriggerLabel_Triggered(UI_TriggerLabel_Handle hdl);
static bool WidgetUI_Fresh_TriggerLabel(UI_TriggerLabel_Handle hdl);

/* Widget Button object Interface */
static WidgetUI_Button_Interface_TypeDef WidgetUI_Button = {
    .create = WidgetUI_Creat_Button,
    .Set_OprLabel = WidgetUI_SetButton_OprLabel,
    .Set_DspType = WidgetUI_SetButton_DspType,
    .Set_TriggerCallback = WidgetUI_SetButton_TriggerCallback,
    .Move = WidgetUI_Move_Button,
    .Operate = WidgetUI_Button_Operate,
};

static WidgetUI_CheckBox_Interface_TypeDef WidgetUI_CheckBox = {
    .create = WidgetUI_Create_CheckBox,
    .Move = WidgetUI_CheckBox_Move,
    .Set_CallBack = WidgetUI_CheckBox_SetCallBack,
    .Trigger = WidgetUI_CheckBox_Trigger,
};

static WidgetUI_SlideBar_Interface_TypeDef WidgetUI_SlideBar = {
    .create = WidgetUI_Create_SlideBar,
    .Move = WidgetUI_SlideBar_Move,
    .Trigger = WidgetUI_SlideBar_Trigger,
    .Set_CallBack = WidgetUI_SlideBar_SetCallBack,
    .Input = WidgetUI_SlideBar_Input,
    .Select = WidgetUI_Select_SlideBar,
};

static WidgetUI_ProcessBar_Interface_TypeDef WidgetUI_ProcessBar = {
    .create = WidgetUI_Create_ProcessBar,
    .Set_DspDir = WidgetUI_ProcessBar_SetDspDir,
    .Set_Value = WidgetUI_ProcessBar_SetCurValue,
    .Move = WidgetUI_ProcessBar_Move,
};

static WidgetUI_Drop_Interface_TypeDef WidgetUI_Drop = {
    .create = WidgetUI_Create_Drop,
    .Move = WidgetUI_Drop_Move,
    .Select = WidgetUI_Drop_Select,
    .select_item = WidgetUI_Drop_SelectItem,
    .add_item = WidgetUI_Add_DropItem,
};

static WidgetUI_DigInput_Interface_TypeDef WidgetUI_DigInput = {
    .create = WidgetUI_Create_DigInput,
    .Move = WidgetUI_DigInput_Move,
    .set_callback = WidgetUI_DigInput_Setcallback,
    .set_DouInput_Range = WidgetUI_DigInput_SetDouRange,
    .set_IntInput_Range = WidgetUI_DigInput_SetIntRange,
    .input = WidgetUI_DigInput_Value,
    .select = WidgetUI_DigInput_Select,
};

static WidgetUI_StrInput_Interface_TypeDef WidgetUI_StrInput = {
    .create = WidgetUI_Create_StrInput,
    .Move = WidgetUI_StrInput_Move,
    .set_callback = WidgetUI_StrInput_SetCallback,
    .Select = WidgetUI_StrInput_Select,
    .set_char = WidgetUI_StrInput_StrChar,
};

static WidgetUI_TriggerLabel_Interface_TypeDef WidgetUI_TirggerLabel = {
    .create = WidgetUI_Cteate_TriggerLabel,
    .Move = WidgetUI_TriggerLabel_Move,
    .set_callback = WidgetUI_TriggerLabel_SetCallback,
    .trigger = WidgetUI_TriggerLabel_Triggered,
};

/* for temp we init each var as null */
static WidgetUI_Utils_TypeDef WidgetUI_Interface = {
    .Show_Selector = WidgetUI_SelectCtl,
    .Fresh = WidgetUI_Fresh,
    .Get_CurSelected_UI = WidgetUI_GetCurSelected_UICtl,
    .Reset_SelectUICtl = Widget_ResetCurUICtl,
    .Button = WidgetUI_GetButton_Instance,
    .CheckBox = WidgetUI_GetCheckBox_Instance,
    .SlideBar = WidgetUI_GetSlideBar_Instance,
    .ProcessBar = WidgetUI_GetProcessBar_Instance,
    .DigInput = WidgetUI_GetDigInput_Instance,
    .StrInput = WidgetUI_GetStrInput_Instance,
    .Drop = WidgetUI_GetDrop_Instance,
    .TriggerLabel = WidgetUI_GetTriggerLabel_Instance,
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
    .Create_Sub = Widget_CreateSub,
    .Rename = Widget_SetName,
    .Delete = Widget_Deleted,
    .DeleteSub = Widget_DeletedSub,
    .Control = Widget_CtlInterface,
    .fresh_all = Widget_FreshAll,
    .trigger_fresh = Widget_CheckFlashTrigger,
};

static Widget_Handle Widget_Create(int16_t cord_x, int16_t cord_y, uint8_t width, uint8_t height, char *name, bool show_frame, bool show_name)
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

    widget_tmp->UI_CoordY_Offset = 0;

    widget_tmp->frame_line_size = 1;
    widget_tmp->is_selected = false;

    widget_tmp->name = name;
    widget_tmp->show_widget_name = show_name;

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
    widget_tmp->CurSelected_CTL = NULL;

    return (Widget_Handle)widget_tmp;
}

static Widget_Handle Widget_CreateSub(Widget_Handle ori, uint8_t width, uint8_t height, char *name, bool show_frame, bool show_name)
{
    WidgetObj_TypeDef *widget_tmp;

    widget_tmp = (WidgetObj_TypeDef *)MMU_Malloc(sizeof(WidgetObj_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->cord_x = HandleToWidgetObj(ori)->cord_x;
    widget_tmp->cord_y = HandleToWidgetObj(ori)->cord_y;

    if ((width > HandleToWidgetObj(ori)->width) || (height > HandleToWidgetObj(ori)->height))
        return WIDGET_CREATE_ERROR;

    widget_tmp->width = width;
    widget_tmp->height = height;

    widget_tmp->UI_CoordY_Offset = 0;

    widget_tmp->frame_line_size = 1;
    widget_tmp->is_selected = false;

    widget_tmp->name = name;
    widget_tmp->show_widget_name = show_name;

    widget_tmp->Dsp = &WidgetDraw_Interface;
    widget_tmp->Ctl = &WidgetCtl_Interface;

    widget_tmp->pixel_map = HandleToWidgetObj(ori)->pixel_map;

    widget_tmp->dsp_item = (item_obj *)MMU_Malloc(sizeof(item_obj));

    if (widget_tmp->dsp_item == NULL)
        return WIDGET_CREATE_ERROR;

    List_ItemInit(widget_tmp->dsp_item, widget_tmp);

    widget_tmp->use_frame = show_frame;
    widget_tmp->show_state = false;

    //clear ui controller first
    widget_tmp->UICtl_List == NULL;
    widget_tmp->ui_ctl_num = 0;
    widget_tmp->CurSelected_CTL = NULL;

    return (Widget_Handle)widget_tmp;
}

static bool Widget_ResetCurUICtl(void)
{
    WidgetObj_TypeDef *widget = GetCur_Active_Widget();

    if (widget == NULL)
        return false;

    widget->CurSelected_CTL = widget->UICtl_List;

    return true;
}

static bool Widget_SetName(Widget_Handle hdl, char *name)
{
    if (hdl == 0)
        return false;

    memset(HandleToWidgetObj(hdl)->name, NULL, strlen(HandleToWidgetObj(hdl)->name));
    HandleToWidgetObj(hdl)->name = name;

    return true;
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

static bool Widget_Reset_MonitorList(item_obj *item)
{
    if (item == NULL)
        return false;

    if (item->prv == NULL)
    {
        MonitorDataObj.widget_dsp_list->data = NULL;
        MonitorDataObj.widget_dsp_list = item->nxt;
        MonitorDataObj.widget_dsp_list->prv = NULL;
    }

    return false;
}

static bool Widget_Deleted(Widget_Handle *hdl)
{
    uint8_t height = 0;
    uint8_t width = 0;

    if ((hdl == NULL) || ((*hdl) == 0))
        return false;

    height = (HandleToWidgetObj(*hdl))->height;
    width = (HandleToWidgetObj(*hdl))->width;

    HandleToWidgetObj(*hdl)->show_state = false;

    List_Delete_Item(HandleToWidgetObj(*hdl)->dsp_item, Widget_Reset_MonitorList);

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

static bool Widget_DeletedSub(Widget_Handle *hdl)
{
    if ((hdl == NULL) || ((*hdl) == 0))
        return false;

    HandleToWidgetObj(*hdl)->show_state = false;

    List_Delete_Item(HandleToWidgetObj(*hdl)->dsp_item, Widget_Reset_MonitorList);

    MMU_Free((HandleToWidgetObj(*hdl))->dsp_item);

    MMU_Free((HandleToWidgetObj(*hdl))->pixel_map);

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

    if (GetCur_Active_Widget()->show_widget_name)
        Widget_DrawStr(Font_8, GetCur_Active_Widget()->name, 0, 0, true);

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

static UI_GenCTL_Handle WidgetUI_GetCurSelected_UICtl(void)
{
    return ((WidgetUI_Item_TypeDef *)((GetCur_Active_Widget()->CurSelected_CTL)->data))->Handler;
}

static bool Widget_Hide(void)
{
    if ((GetCur_Active_Widget() == NULL) || (MonitorDataObj.on_show <= 1))
        return false;

    if (GetCur_Active_Widget()->show_state)
    {
        GetCur_Active_Widget()->show_state = false;
        List_Delete_Item(GetCur_Active_Widget()->dsp_item, NULL);
        Widget_SetFreshState(Fresh_State_Prepare);

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
    if (!obj->show_state)
        return 0;

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
                        WidgetDraw_Interface.draw_hline,
                        WidgetDraw_Interface.draw_vline,
                        WidgetDraw_Interface.draw_rectangle,
                        WidgetDraw_Interface.draw_radius_rectangle,
                        WidgetDraw_Interface.draw_circle,
                        WidgetDraw_Interface.draw_circle_section,
                        WidgetDraw_Interface.draw_str,
                        WidgetDraw_Interface.draw_num,
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

/* still in developing */
static void WidgetUI_SetAll_CoordY_Offset(int8_t offset)
{
    list_obj *ctl_list = GetCur_Active_Widget()->UICtl_List;
    item_obj *ui_item = NULL;

    if (ctl_list != NULL)
    {
        ui_item = ctl_list;

        while (ui_item != NULL)
        {
            if (ui_item->data != NULL)
            {
                UI_GenCTL_Handle ui_handle = ((WidgetUI_Item_TypeDef *)(ui_item->data))->Handler;

                switch (((WidgetUI_Item_TypeDef *)(ui_item->data))->type)
                {
                case UI_Type_Button:
                    UI_Button.move(HandleToButtonObj(ui_handle), HandleToButtonObj(ui_handle)->Gen_Data.x, HandleToButtonObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_CheckBox:
                    UI_CheckBox.Move(HandleToCheckBoxObj(ui_handle), HandleToCheckBoxObj(ui_handle)->Gen_Data.x, HandleToCheckBoxObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_SlideBar:
                    UI_SlideBar.Move(HandleToSlideBarObj(ui_handle), HandleToSlideBarObj(ui_handle)->Gen_Data.x, HandleToSlideBarObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_ProcBar:
                    UI_ProcessBar.Move(HandleToProcessBarObj(ui_handle), HandleToProcessBarObj(ui_handle)->Gen_Data.x, HandleToProcessBarObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_Drop:
                    UI_Drop.Move(HandleToDropObj(ui_handle), HandleToDropObj(ui_handle)->Gen_Data.x, HandleToDropObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_DigInput:
                    UI_DigInput.Move(HandleToDigInputObj(ui_handle), HandleToDigInputObj(ui_handle)->Gen_Data.x, HandleToDigInputObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_StrInput:
                    UI_StrInput.Move(HandleToStrInputObj(ui_handle), HandleToStrInputObj(ui_handle)->Gen_Data.x, HandleToStrInputObj(ui_handle)->Gen_Data.y + offset);
                    break;

                case UI_Type_TriggerLabel:
                    UI_TriggerLabel.Move(HandleToTriggerLabelObj(ui_handle), HandleToTriggerLabelObj(ui_handle)->Gen_Data.x, HandleToTriggerLabelObj(ui_handle)->Gen_Data.y + offset);
                    break;

                default:
                    break;
                }
            }

            ui_item = ui_item->nxt;
        }
    }
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
    WidgetUI_FreshState_List fresh_state;
    int8_t offset = *search_offset;

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

    switch (((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->type)
    {
    case UI_Type_Button:
        if (HandleToButtonObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_CheckBox:
        if (HandleToCheckBoxObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_SlideBar:
        if (HandleToSlideBarObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_Drop:
        if (HandleToDropObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_ProcBar:
        if (HandleToProcessBarObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_DigInput:
        if (HandleToDigInputObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    case UI_Type_StrInput:
        if (HandleToStrInputObj(((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data))->Handler)->Gen_Data.y < UI_Get_FontType())
            return true;
        else
            break;

    default:
        break;
    }

    fresh_state = UI_ShowSelector((WidgetUI_Item_TypeDef *)(tmp->CurSelected_CTL->data));

    switch ((uint8_t)fresh_state)
    {
    case UI_Fresh_Done:
        return true;

    case UI_Fresh_Error:
        return false;

    case UI_Fresh_Skip:
        return true;

    default:
        return false;
    }
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

    case UI_Type_ProcBar:
        return WidgetUI_Fresh_ProcessBar(UI_Data->Handler);

    case UI_Type_Drop:
        return WidgetUI_Fresh_Drop(UI_Data->Handler);

    case UI_Type_DigInput:
        return WidgetUI_Fresh_DigInput(UI_Data->Handler);

    case UI_Type_StrInput:
        return WidgetUI_Fresh_StrInput(UI_Data->Handler);

    case UI_Type_TriggerLabel:
        return WidgetUI_Fresh_TriggerLabel(UI_Data->Handler);

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

    case UI_Type_ProcBar:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToProcessBarObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToProcessBarObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }

        break;

    case UI_Type_Drop:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToDropObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToDropObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }
        break;

    case UI_Type_DigInput:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToDigInputObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToDigInputObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }
        break;

    case UI_Type_StrInput:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToStrInputObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToStrInputObj(item->Handler)->Gen_Data.y;

        default:
            return 0;
        }
        break;

    case UI_Type_TriggerLabel:
        switch ((uint8_t)option)
        {
        case WidgetUI_get_x:
            return HandleToTriggerLabelObj(item->Handler)->Gen_Data.x;

        case WidgetUI_get_y:
            return HandleToTriggerLabelObj(item->Handler)->Gen_Data.y;

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

    if (item_prv_y < item_nxt_y)
    {
        return item_prv;
    }
    else if (item_prv_y == item_nxt_y)
    {
        if (item_prv_x <= item_nxt_x)
        {
            return item_nxt;
        }
        else
            return item_prv;
    }
    else
        return item_nxt;
}

/* still in developing */
static bool WidgetUIList_DeleteItem(UI_GenCTL_Handle *hdl)
{
    WidgetObj_TypeDef *tmp = GetCur_Active_Widget();

    tmp->ui_ctl_num--;
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

        List_Init(tmp->UICtl_List, NULL, by_condition, WidgetUI_InsertSequence_Callback);
    }

    tmp->CurSelected_CTL = tmp->UICtl_List;

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

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

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

static bool WidgetUI_SetButton_DspType(UI_Button_Handle Btn_Hdl, UI_ButtonDsp_TypeList type)
{
    if (Btn_Hdl == 0)
        return false;

    return UI_Button.set_DspType(HandleToButtonObj(Btn_Hdl), type);
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

static bool WidgetUI_Fresh_Button(UI_Button_Handle Btn_Hdl)
{
    if (Btn_Hdl == 0)
        return false;

    if (HandleToButtonObj(Btn_Hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToButtonObj(Btn_Hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == Btn_Hdl)
        {
            WidgetUI_SetAll_CoordY_Offset(UICTL_SLIDERBAR_HEIGHT);
        }

        return true;
    }

    if ((HandleToButtonObj(Btn_Hdl)->Gen_Data.y >= (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToButtonObj(Btn_Hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == Btn_Hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_SLIDERBAR_HEIGHT);

        return false;
    }

    return UI_Button.ctl(HandleToButtonObj(Btn_Hdl));
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

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

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

    if (HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == checkbox_hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_SLIDERBAR_HEIGHT);

        return true;
    }

    if ((HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.y >= (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToCheckBoxObj(checkbox_hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == checkbox_hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_SLIDERBAR_HEIGHT);

        return false;
    }

    return UI_CheckBox.ctl(HandleToCheckBoxObj(checkbox_hdl));
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

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

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

    if (HandleToSlideBarObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToSlideBarObj(hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_SLIDERBAR_HEIGHT);

        return true;
    }

    if ((HandleToSlideBarObj(hdl)->Gen_Data.y >= (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToSlideBarObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_SLIDERBAR_HEIGHT);

        return false;
    }

    return UI_SlideBar.ctl(HandleToSlideBarObj(hdl));
}

/************************************** widget SlideBar interface ******************************************/

/************************************** widget ProcessBar interface ******************************************/

static WidgetUI_ProcessBar_Interface_TypeDef *WidgetUI_GetProcessBar_Instance(void)
{
    return &WidgetUI_ProcessBar;
}

/* process bar can`t be selected */
static UI_ProcessBar_Handle WidgetUI_Create_ProcessBar(char *label, UI_ProcessBar_DspType_List dsp_type, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max)
{
    UI_ProcessBarObj_TypeDef *processbar = NULL;

    processbar = (UI_SlideBarObj_TypeDef *)MMU_Malloc(sizeof(UI_SlideBarObj_TypeDef));

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

    if ((processbar == NULL) ||
        (!UI_ProcessBar.init(processbar, dsp_type, label, x, y, width, min, max)) ||
        (!WidgetUIList_InsertItem(processbar, UI_Type_ProcBar)))
        return NULL;

    return ((UI_ProcessBar_Handle)processbar);
}

static bool WidgetUI_ProcessBar_SetDspDir(UI_ProcessBar_Handle hdl, UI_ProcessBar_MoveDir_TypeDef dir)
{
    if (hdl == 0)
        return false;

    return UI_ProcessBar.set_DspDir(HandleToProcessBarObj(hdl), dir);
}

static bool WidgetUI_ProcessBar_SetCurValue(UI_ProcessBar_Handle hdl, int32_t val)
{
    if (hdl == 0)
        return false;

    return UI_ProcessBar.set_CurVal(HandleToProcessBarObj(hdl), val);
}

static bool WidgetUI_ProcessBar_Move(UI_ProcessBar_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return UI_ProcessBar.Move(HandleToProcessBarObj(hdl), x, y);
}

static bool WidgetUI_Fresh_ProcessBar(UI_SlideBar_Handle hdl)
{
    int16_t offset = 0;

    if (hdl == 0)
        return false;

    if (HandleToProcessBarObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToProcessBarObj(hdl)->Gen_Data.x < 0)
    {
        switch (HandleToProcessBarObj(hdl)->Dsp_Type)
        {
        case UI_ProcBar_DspType_LoadBar:
            WidgetUI_SetAll_CoordY_Offset(UICTL_PROCESSBAR_DOWNLOAD_HEIGHT);
            break;

        case UI_ProcBar_DspType_DotBar:
            WidgetUI_SetAll_CoordY_Offset(UICTL_PROCESSBAR_DOT_HEIGHT);
            break;

        case UI_ProcBar_DspType_FrameBar:
            WidgetUI_SetAll_CoordY_Offset(UICTL_PROCESSBAR_FRAME_HEIGHT);
            break;

        default:
            return true;
        }

        return true;
    }

    if ((HandleToProcessBarObj(hdl)->Gen_Data.y >= (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToProcessBarObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            switch (HandleToProcessBarObj(hdl)->Dsp_Type)
            {
            case UI_ProcBar_DspType_LoadBar:
                WidgetUI_SetAll_CoordY_Offset(-UICTL_PROCESSBAR_DOWNLOAD_HEIGHT);
                break;

            case UI_ProcBar_DspType_DotBar:
                WidgetUI_SetAll_CoordY_Offset(-UICTL_PROCESSBAR_DOT_HEIGHT);
                break;

            case UI_ProcBar_DspType_FrameBar:
                WidgetUI_SetAll_CoordY_Offset(-UICTL_PROCESSBAR_FRAME_HEIGHT);
                break;

            default:
                return false;
            }

        return false;
    }

    return UI_ProcessBar.ctl(HandleToSlideBarObj(hdl));
}

/************************************** widget ProcessBar interface ******************************************/

/************************************** widget Drop interface ******************************************/
static WidgetUI_Drop_Interface_TypeDef *WidgetUI_GetDrop_Instance(void)
{
    return &WidgetUI_Drop;
}

static UI_Drop_Handle WidgetUI_Create_Drop(char *label, int16_t x, int16_t y)
{
    UI_DropObj_TypeDef *drop = NULL;

    drop = (UI_DropObj_TypeDef *)MMU_Malloc(sizeof(UI_DropObj_TypeDef));

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

    if ((drop == NULL) ||
        (!UI_Drop.init(drop, label, x, y)) ||
        (!WidgetUIList_InsertItem(drop, UI_Type_Drop)))
        return NULL;

    return ((UI_Drop_Handle)drop);
}

static bool WidgetUI_Add_DropItem(UI_Drop_Handle hdl, char *label, void *data, uint16_t data_size, UI_Drop_Callback callback)
{
    if (hdl == 0)
        return false;

    return UI_Drop.Add_drop_item(HandleToDropObj(hdl), label, data, data_size, callback);
}

static bool WidgetUI_Drop_Move(UI_Drop_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return UI_Drop.Move(HandleToDropObj(hdl), x, y);
}

static bool WidgetUI_Drop_Select(UI_Drop_Handle hdl, bool state)
{
    if (hdl == 0)
        return false;

    return UI_Drop.Set_Select(HandleToDropObj(hdl), state);
}

static bool WidgetUI_Drop_SelectItem(UI_Drop_Handle hdl, uint8_t *offset)
{
    if (hdl == 0)
        return false;

    if (UI_Drop.Get_Select(hdl))
    {
        UI_Drop.Select_DropItem(HandleToDropObj(hdl), offset);
    }
    else
        return false;
}

static bool WidgetUI_Fresh_Drop(UI_Drop_Handle hdl)
{
    int16_t offset = 0;

    if (hdl == 0)
        return false;

    if (HandleToDropObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToDropObj(hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_DROP_HEIGHT);

        return true;
    }

    if ((HandleToDropObj(hdl)->Gen_Data.y > (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToDropObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_DROP_HEIGHT);

        return false;
    }

    return UI_Drop.ctl(HandleToDropObj(hdl));
}

/************************************** widget Drop interface ******************************************/

/************************************** widget DigInput interface ******************************************/
static WidgetUI_DigInput_Interface_TypeDef *WidgetUI_GetDigInput_Instance(void)
{
    return &WidgetUI_DigInput;
}

static UI_DigInput_Handle WidgetUI_Create_DigInput(char *label, int16_t x, int16_t y, UI_DigInput_Type type)
{
    UI_DigInputObj_TypeDef *dig_input = NULL;

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

    dig_input = (UI_DigInputObj_TypeDef *)MMU_Malloc(sizeof(UI_DigInputObj_TypeDef));
    if ((dig_input == NULL) ||
        (!UI_DigInput.init(dig_input, label, x, y, type)) ||
        (!WidgetUIList_InsertItem(dig_input, UI_Type_DigInput)))
        return NULL;

    return ((UI_DigInput_Handle)dig_input);
}

static bool WidgetUI_DigInput_Setcallback(UI_DigInput_Handle hdl, UI_DigInput_Callback callback)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.set_callback(HandleToDigInputObj(hdl), callback);
}

static bool WidgetUI_DigInput_SetIntRange(UI_DigInput_Handle hdl, uint8_t eff_len, int32_t min, int32_t max, int32_t default_val)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.set_range_IntInput(HandleToDigInputObj(hdl), eff_len, max, min, default_val);
}

static bool WidgetUI_DigInput_SetDouRange(UI_DigInput_Handle hdl, uint8_t int_eff_len, uint8_t dou_eff_len, double max, double min, double cur)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.set_range_DouInput(HandleToDigInputObj(hdl), int_eff_len, dou_eff_len, max, min, cur);
}

static bool WidgetUI_DigInput_Move(UI_DigInput_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.Move(HandleToDigInputObj(hdl), x, y);
}

static bool WidgetUI_DigInput_Select(UI_DigInput_Handle hdl, bool state)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.Set_Select(HandleToDigInputObj(hdl), state);
}

static bool WidgetUI_DigInput_Value(UI_DigInput_Handle hdl, uint8_t pos, int8_t *val)
{
    if (hdl == 0)
        return false;

    return UI_DigInput.input_val(HandleToDigInputObj(hdl), pos, val);
}

static bool WidgetUI_Fresh_DigInput(UI_DigInput_Handle hdl)
{
    if (hdl == 0)
        return false;

    if (HandleToDigInputObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToDigInputObj(hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_DIGINPUT_HEIGHT);

        return true;
    }

    if ((HandleToDigInputObj(hdl)->Gen_Data.y > (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToDigInputObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_DIGINPUT_HEIGHT);

        return false;
    }

    return UI_DigInput.ctl(HandleToDigInputObj(hdl));
}

/************************************** widget DigInput interface ******************************************/

/************************************** widget StrInput interface ******************************************/

static WidgetUI_StrInput_Interface_TypeDef *WidgetUI_GetStrInput_Instance(void)
{
    return &WidgetUI_StrInput;
}

static UI_StrInput_Handle WidgetUI_Create_StrInput(char *label, int16_t x, int16_t y)
{
    UI_StrInputObj_TypeDef *str_input = NULL;

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

    str_input = (UI_StrInputObj_TypeDef *)MMU_Malloc(sizeof(UI_StrInputObj_TypeDef));
    if ((str_input == NULL) ||
        (!UI_StrInput.init(str_input, label, x, y, UI_StrCTLtype_Input)) ||
        (!WidgetUIList_InsertItem(str_input, UI_Type_StrInput)))
        return NULL;

    return ((UI_StrInput_Handle)str_input);
}

static bool WidgetUI_StrInput_Move(UI_StrInput_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return UI_StrInput.Move(HandleToStrInputObj(hdl), x, y);
}

static bool WidgetUI_StrInput_SetCallback(UI_StrInput_Handle hdl, UI_StrInput_Callback callback)
{
    if (hdl == 0)
        return false;

    return UI_StrInput.set_callback(HandleToStrInputObj(hdl), callback);
}

static bool WidgetUI_StrInput_Select(UI_StrInput_Handle hdl, bool state)
{
    if (hdl == 0)
        return false;

    return UI_StrInput.Set_Select(HandleToStrInputObj(hdl), state);
}

static bool WidgetUI_StrInput_StrChar(UI_StrInput_Handle hdl, uint8_t pos, char input)
{
    if (hdl == 0)
        return false;

    return UI_StrInput.input_char(HandleToStrInputObj(hdl), pos, input);
}

static bool WidgetUI_Fresh_StrInput(UI_StrInput_Handle hdl)
{
    if (hdl == 0)
        return false;

    if (HandleToStrInputObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToStrInputObj(hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_STRINPUT_HEIGHT);

        return true;
    }

    if ((HandleToStrInputObj(hdl)->Gen_Data.y > (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToStrInputObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_STRINPUT_HEIGHT);

        return false;
    }

    return UI_StrInput.ctl(HandleToStrInputObj(hdl));
}

/************************************** widget StrInput interface ******************************************/

/************************************** widget TriggerLabel interface ******************************************/
static WidgetUI_TriggerLabel_Interface_TypeDef *WidgetUI_GetTriggerLabel_Instance(void)
{
    return &WidgetUI_TirggerLabel;
}

static UI_TriggerLabel_Handle WidgetUI_Cteate_TriggerLabel(char *label, int16_t x, int16_t y)
{
    UI_TriggerLabelObj_TypeDef *triggerlabel = NULL;

    if (GetCur_Active_Widget()->show_widget_name)
        y += UI_Get_FontType();

    triggerlabel = (UI_TriggerLabelObj_TypeDef *)MMU_Malloc(sizeof(UI_TriggerLabelObj_TypeDef));
    if ((triggerlabel == NULL) ||
        (!UI_TriggerLabel.init(triggerlabel, label, x, y)) ||
        (!WidgetUIList_InsertItem(triggerlabel, UI_Type_TriggerLabel)))
        return NULL;

    return ((UI_TriggerLabel_Handle)triggerlabel);
}

/* still in developing */
static void WidgetUI_Delete_TriggerLabel(UI_TriggerLabel_Handle *hdl)
{
}

static bool WidgetUI_TriggerLabel_Move(UI_TriggerLabel_Handle hdl, int16_t x, int16_t y)
{
    if (hdl == 0)
        return false;

    return UI_TriggerLabel.Move(HandleToTriggerLabelObj(hdl), x, y);
}

static bool WidgetUI_TriggerLabel_SetCallback(UI_TriggerLabel_Handle hdl, UI_TriggerLabel_Callback callback)
{
    if (hdl == 0)
        return false;

    return UI_TriggerLabel.Set_Callback(HandleToTriggerLabelObj(hdl), callback);
}

static bool WidgetUI_TriggerLabel_Triggered(UI_TriggerLabel_Handle hdl)
{
    if (hdl == 0)
        return false;

    return UI_TriggerLabel.trigger(HandleToTriggerLabelObj(hdl));
}

static bool WidgetUI_Fresh_TriggerLabel(UI_TriggerLabel_Handle hdl)
{
    if (hdl == 0)
        return false;
    if (HandleToTriggerLabelObj(hdl)->Gen_Data.y < UI_Get_FontType() ||
        HandleToTriggerLabelObj(hdl)->Gen_Data.x < 0)
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(UICTL_STRINPUT_HEIGHT);

        return true;
    }

    if ((HandleToTriggerLabelObj(hdl)->Gen_Data.y > (GetCur_Active_Widget()->height - UI_Get_FontType())) ||
        (HandleToTriggerLabelObj(hdl)->Gen_Data.x >= GetCur_Active_Widget()->width))
    {
        if (WidgetUI_GetCurSelected_UICtl() == hdl)
            WidgetUI_SetAll_CoordY_Offset(-UICTL_STRINPUT_HEIGHT);

        return false;
    }

    return UI_TriggerLabel.ctl(HandleToTriggerLabelObj(hdl));
}

/************************************** widget TriggerLabel interface ******************************************/

/************************************** widget UI interface ******************************************/
