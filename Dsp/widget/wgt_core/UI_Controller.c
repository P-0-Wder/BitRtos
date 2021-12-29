#include "UI_Controller.h"
#include "runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mmu.h"

/* internal object */
static UI_DrawInterface_TypeDef UI_DspInterface;
static uint8_t base_font = Default_Font;

/* external function */
/* UI Get General Get Mathod */
UI_GetWidget_Width UI_Get_WidgetWidth = NULL;
// UI_GetWidget_Height UI_Get_WidgetHeight = NULL;

/* UI button section */
static bool UI_Button_Init(UI_ButtonObj_TypeDef *Obj, char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
static bool UI_Button_Set_Label(UI_ButtonObj_TypeDef *Obj, UI_Button_State_List state, char *label);
static bool UI_Button_Set_TriggerCallback(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback callback);
static bool UI_Button_Push(UI_ButtonObj_TypeDef *Obj);
static bool UI_Button_Release(UI_ButtonObj_TypeDef *Obj);
static bool UI_Button_Move(UI_ButtonObj_TypeDef *Obj, uint16_t x, uint16_t y);
static bool UI_Button_Ctl(UI_ButtonObj_TypeDef *Obj);

/* UI check box section */
static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, char *label, int16_t x, int16_t y, bool state);
static bool UI_CheckBox_SetCallback(UI_CheckBoxObj_TypeDef *Obj, UI_CheckBoxTrigger_Callback callback);
static bool UI_CheckBox_Move(UI_CheckBoxObj_TypeDef *Obj, int16_t x, int16_t y);
static bool UI_CheckBox_Trigger(UI_CheckBoxObj_TypeDef *Obj);
static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj);

/* UI slider bar section */
static bool UI_SlideBar_Init(UI_SlideBarObj_TypeDef *Obj, UI_SliderBar_Mode_List mode, char *label, int16_t x, int16_t y, int16_t limit_max, int16_t limit_min, int16_t start_val, int16_t step_len);
static bool UI_SlideBar_Move(UI_SlideBarObj_TypeDef *Obj, int16_t x, int16_t y);
static bool UI_SlideBar_SetCallBack(UI_SlideBarObj_TypeDef *Obj, UI_SliderBarTrigger_Callback callback);
static bool UI_SlideBar_Input(UI_SlideBarObj_TypeDef *Obj, int16_t *step);
static bool UI_SlideBar_Trigger(UI_SlideBarObj_TypeDef *Obj);
static bool UI_SlideBar_CTL(UI_SlideBarObj_TypeDef *Obj);
static void UI_SlideBar_SetSelect(UI_SlideBarObj_TypeDef *Obj, bool select);
static bool UI_SlideBar_IsSelected(UI_SlideBarObj_TypeDef *Obj);

/* UI Process bar section */
static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_DspType_List dsp_type, char *label, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max);
static bool UI_ProcessBar_SetDspDir(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_MoveDir_TypeDef Dir);
static bool UI_ProcessBar_SetCurVal(UI_ProcessBarObj_TypeDef *Obj, int32_t val);
static bool UI_ProcessBar_Ctl(UI_ProcessBarObj_TypeDef *Obj);
static bool UI_ProcessBar_Move(UI_ProcessBarObj_TypeDef *Obj, uint16_t x, uint16_t y);
static bool UI_ProcessBar_SetLabelPos(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_LabelPos_List Pos);

/* UI Drop Section */
static bool UI_Drop_Init(UI_DropObj_TypeDef *Obj, char *label, int16_t x, int16_t y);
static bool UI_Drop_Move(UI_DropObj_TypeDef *Obj, int16_t x, int16_t y);
static bool UI_Drop_AddDropItem(UI_DropObj_TypeDef *Obj, char *item_desc, void *data, uint16_t data_size, UI_Drop_Callback callback);
static bool UI_Drop_SetSelect(UI_DropObj_TypeDef *Obj, bool state);
static bool UI_Drop_GetSelect(UI_DropObj_TypeDef *Obj);
static bool UI_Drop_SelectItem(UI_DropObj_TypeDef *Obj, int8_t *offset);
static bool UI_Drop_Ctl(UI_DropObj_TypeDef *Obj);

/* UI Digital Input Section */
static bool UI_DigInput_Init(UI_DigInputObj_TypeDef *Obj, char *label, int16_t x, int16_t y, UI_DigInput_Type type);
static bool UI_DigInput_SetIntRange(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, int32_t max, int32_t min, int32_t cur);
static bool UI_DigInput_SetDouRange(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, uint8_t efft_point_len, double max, double min, double cur);
static bool UI_DigInput_GetDoubleVal(UI_DigInputObj_TypeDef *Obj, double *Out);
static bool UI_DigInput_GetIntVal(UI_DigInputObj_TypeDef *Obj, int32_t *Out);
static bool UI_DigInput_Move(UI_DigInputObj_TypeDef *Obj, int16_t x, int16_t y);
static bool UI_DigInput_SetSelectStatus(UI_DigInputObj_TypeDef *Obj, bool status);
static bool UI_DigInput_GetSelectStatus(UI_DigInputObj_TypeDef *Obj);

/* general function */
static bool UI_Get_InitSate(UI_GeneralData_TypeDef GenData);
static bool UI_Selecte(UI_GeneralData_TypeDef *GenData, bool select);
static bool UI_Get_Selected(UI_GeneralData_TypeDef GenData);
static bool UI_Move(UI_GeneralData_TypeDef *GenUI_Info, int16_t dst_x, int16_t dst_y);

/* UI custom display mathod */
static bool UI_Draw_HorDotLine(int16_t x, int16_t y, uint8_t len, uint8_t line_width);

/* external Object var */
UI_Button_Interface_TypeDef UI_Button = {
    .ctl = UI_Button_Ctl,
    .init = UI_Button_Init,
    .push = UI_Button_Push,
    .move = UI_Button_Move,
    .release = UI_Button_Release,
    .set_label = UI_Button_Set_Label,
    .set_trogger_callback = UI_Button_Set_TriggerCallback,
};

UI_CheckBox_Interface_TypeDef UI_CheckBox = {
    .ctl = UI_CheckBox_Ctl,
    .init = UI_CheckBox_Init,
    .Set_Callback = UI_CheckBox_SetCallback,
    .Move = UI_CheckBox_Move,
    .Trigger = UI_CheckBox_Trigger,
};

UI_SliderBar_Interface_TypeDef UI_SlideBar = {
    .init = UI_SlideBar_Init,
    .Move = UI_SlideBar_Move,
    .Set_Callbak = UI_SlideBar_SetCallBack,
    .Input = UI_SlideBar_Input,
    .Trigger = UI_SlideBar_Trigger,
    .ctl = UI_SlideBar_CTL,
    .Set_Select = UI_SlideBar_SetSelect,
    .Get_Select = UI_SlideBar_IsSelected,
};

UI_ProcessBar_Interface_TypeDef UI_ProcessBar = {
    .init = UI_ProcessBar_Init,
    .Move = UI_ProcessBar_Move,
    .set_CurVal = UI_ProcessBar_SetCurVal,
    .set_DspDir = UI_ProcessBar_SetDspDir,
    .ctl = UI_ProcessBar_Ctl,
    .Set_LabelPos = UI_ProcessBar_SetLabelPos,
};

UI_Drop_Interface_TypeDef UI_Drop = {
    .init = UI_Drop_Init,
    .Move = UI_Drop_Move,
    .Select_DropItem = UI_Drop_SelectItem,
    .Set_Select = UI_Drop_SetSelect,
    .Get_Select = UI_Drop_GetSelect,
    .Add_drop_item = UI_Drop_AddDropItem,
    .ctl = UI_Drop_Ctl,
};

UI_DigInput_Interface_TypeDef UI_DigInput = {
    .init = UI_DigInput_Init,
    .input_part_select = NULL,
    .Select_UI = UI_DigInput_SetSelectStatus,
    .set_range_DouInput = UI_DigInput_SetDouRange,
    .set_range_IntInput = UI_DigInput_SetIntRange,
    .get_CurInout_Double = UI_DigInput_GetDoubleVal,
    .get_CurInput_Int = UI_DigInput_GetIntVal,
    .Move = UI_DigInput_Move,
    .ctl = NULL,
};

/******************************* general function *********************************/

bool UI_Set_FontType(uint8_t font)
{
    if (font < Font_8 || font > Font_16)
        return false;

    base_font = font;

    return true;
}

void UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawHLine line_h,
                         UI_DrawVLine line_v,
                         UI_DrawRectangle rectangle,
                         UI_DrawRadiusRectangle radius_rectangle,
                         UI_DrawCircle circle,
                         UI_DrawCircleSection circle_section,
                         UI_DrawStr str,
                         UI_DrawDig dig,
                         UI_FillCircle fill_circle,
                         UI_FillCircle_Section fillcircle_section,
                         UI_FillRectangle fill_rectangle,
                         UI_FillRadiusRectangle fill_radius_rectangle)
{
    UI_DspInterface.draw_circle = circle;
    UI_DspInterface.draw_circle_section = circle_section;
    UI_DspInterface.draw_line_h = line_h;
    UI_DspInterface.draw_line_v = line_v;
    UI_DspInterface.draw_point = point;
    UI_DspInterface.draw_rectangle = rectangle;
    UI_DspInterface.draw_radius_rectangle = radius_rectangle;
    UI_DspInterface.draw_str = str;
    UI_DspInterface.draw_dig = dig;

    UI_DspInterface.fill_circle = fill_circle;
    UI_DspInterface.fill_circle_section = fillcircle_section;
    UI_DspInterface.fill_radius_rectangle = fill_radius_rectangle;
    UI_DspInterface.fill_rectangle = fill_rectangle;
}

void UI_Set_GetWidgetWidthMathod(UI_GetWidget_Width mathod)
{
    UI_Get_WidgetWidth = mathod;
}

static bool UI_Draw_HorDotLine(int16_t x, int16_t y, uint8_t len, uint8_t line_width)
{
    if ((UI_DspInterface.draw_point == NULL) || (UI_Get_WidgetWidth == NULL))
        return false;

    if (x > UI_Get_WidgetWidth())
        return true;

    for (int16_t i = x; i < x + len; i++)
    {
        for (uint8_t width = 0; width < line_width; width++)
        {
            if (i % 2)
            {
                UI_DspInterface.draw_point(i, y + width, true);
            }
        }
    }

    return true;
}

/* still in developing about this selector */
/* FYI different UI Item Got different selector */
bool UI_ShowSelector(WidgetUI_Item_TypeDef *item)
{
    /* button selector coordinate */
    int16_t Btn_Slct_LftUp_X = 0;
    int16_t Btn_Slct_LftUp_Y = 0;
    int16_t Btn_Slct_LftDwn_X = 0;
    int16_t Btn_Slct_LftDwn_Y = 0;
    int16_t Btn_Slct_RgtUp_X = 0;
    int16_t Btn_Slct_RgtUp_Y = 0;
    int16_t Btn_Slct_RgtDwn_X = 0;
    int16_t Btn_Slct_RgtDwn_Y = 0;

    /* selector block coordinate */
    int16_t block_x;
    int16_t block_y;

    uint8_t widget_width = 128;
    uint8_t selector_height = 0;

    if (UI_Get_WidgetWidth != NULL)
    {
        widget_width = UI_Get_WidgetWidth();

        if (widget_width == 0)
            return false;
    }

    if (item == NULL)
        return false;

    switch (base_font)
    {
    case Font_8:
        selector_height = Font_8 + 2;
        break;

    case Font_12:
    case Font_16:
        selector_height = Font_12 - 1;
        break;

    default:
        return false;
    }

    switch ((uint8_t)(item->type))
    {
    case UI_Type_Button:

        if (UI_DspInterface.fill_circle_section == NULL)
            return false;

        /* comput button selector coordinate first */
        Btn_Slct_LftUp_X = HandleToButtonObj(item->Handler)->Gen_Data.x;
        Btn_Slct_LftUp_Y = HandleToButtonObj(item->Handler)->Gen_Data.y + HandleToButtonObj(item->Handler)->height - 1;

        Btn_Slct_LftDwn_X = Btn_Slct_LftUp_X;
        Btn_Slct_LftDwn_Y = HandleToButtonObj(item->Handler)->Gen_Data.y;

        Btn_Slct_RgtUp_X = HandleToButtonObj(item->Handler)->Gen_Data.x + HandleToButtonObj(item->Handler)->width;
        Btn_Slct_RgtUp_Y = Btn_Slct_LftUp_Y;

        Btn_Slct_RgtDwn_X = Btn_Slct_RgtUp_X;
        Btn_Slct_RgtDwn_Y = Btn_Slct_LftDwn_Y;

        // show button selector
        UI_DspInterface.draw_circle_section(Btn_Slct_LftUp_X, Btn_Slct_LftUp_Y, BUTTON_SELECTOR_RADIUS, UICircle_Left_Down, BUTTON_SELECTOR_LINE_SIZE, true);
        UI_DspInterface.draw_circle_section(Btn_Slct_LftDwn_X, Btn_Slct_LftDwn_Y, BUTTON_SELECTOR_RADIUS, UICircle_Left_Up, BUTTON_SELECTOR_LINE_SIZE, true);
        UI_DspInterface.draw_circle_section(Btn_Slct_RgtUp_X, Btn_Slct_RgtUp_Y, BUTTON_SELECTOR_RADIUS, UICircle_Right_Down, BUTTON_SELECTOR_LINE_SIZE, true);
        UI_DspInterface.draw_circle_section(Btn_Slct_RgtDwn_X, Btn_Slct_RgtDwn_Y, BUTTON_SELECTOR_RADIUS, UICircle_Right_Up, BUTTON_SELECTOR_LINE_SIZE, true);
        break;

    case UI_Type_CheckBox:
        block_x = HandleToCheckBoxObj(item->Handler)->Gen_Data.x + 3;
        block_y = HandleToCheckBoxObj(item->Handler)->Gen_Data.y + 1;

        UI_DspInterface.fill_rectangle(block_x, block_y, (widget_width - 6), selector_height, true);
        break;

    case UI_Type_SlideBar:
        block_y = HandleToSlideBarObj(item->Handler)->Gen_Data.y + 1;

        if (!UI_SlideBar_IsSelected(HandleToButtonObj(item->Handler)))
        {
            block_x = HandleToSlideBarObj(item->Handler)->Gen_Data.x + 3;

            UI_DspInterface.fill_rectangle(block_x, block_y, (widget_width - 6), selector_height, true);
        }
        else
        {
            block_x = HandleToSlideBarObj(item->Handler)->BarCoord_X - 3;

            UI_DspInterface.fill_rectangle(block_x, block_y, (widget_width - block_x - 6), selector_height, true);
        }
        break;

    case UI_Type_ProcBar:
        /* show next */
        break;

    case UI_Type_Drop:
        block_x = HandleToDropObj(item->Handler)->Gen_Data.x + 3;

        if (HandleToDropObj(item->Handler)->is_selected)
        {
            block_y = HandleToDropObj(item->Handler)->Gen_Data.y;
        }
        else
        {
            block_y = HandleToDropObj(item->Handler)->Gen_Data.y + base_font + 2;
            UI_DspInterface.fill_rectangle(block_x, block_y, (widget_width - 6), UICTL_DROP_HEIGHT, true);
        }
        break;

    default:
        return false;
    }

    return true;
}

static void UI_GenData_Init(UI_GeneralData_TypeDef *GenData, char *label, int16_t x, int16_t y)
{
    GenData->label = label;

    GenData->x = x;
    GenData->y = y;
    GenData->height = 0;

    GenData->operatable = false;
}

static bool UI_Move(UI_GeneralData_TypeDef *GenUI_Info, int16_t dst_x, int16_t dst_y)
{
    if (GenUI_Info == NULL)
        return false;

    GenUI_Info->x = dst_x;
    GenUI_Info->y = dst_y;

    return true;
}

/********************************************** UI Button Object ***************************************************/
static bool UI_Button_Init(UI_ButtonObj_TypeDef *Obj, char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state)
{
    if (Obj == NULL)
        return false;

    Obj->default_state = state;
    Obj->state = state;
    Obj->type = type;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->width = width;
    Obj->height = height;

    Obj->PushDown_Label = DEFAULT_BUTTON_PUSH_LABEL;
    Obj->Release_Label = DEFAULT_BUTTON_RELEASE_LABEL;

    return true;
}

static bool UI_Button_Set_Label(UI_ButtonObj_TypeDef *Obj, UI_Button_State_List state, char *label)
{
    if (Obj == NULL)
        return false;

    if (state == UI_Btn_PushDwn)
        Obj->PushDown_Label = label;
    else
        Obj->Release_Label = label;

    return true;
}

static bool UI_Button_Set_TriggerCallback(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_ButtonTrigger_Callback callback)
{
    if (Obj == NULL)
        return false;

    if (type == Push_Trigger)
    {
        Obj->push_callback = callback;
    }
    else if (type == Release_Trigger)
    {
        Obj->release_callback = callback;
    }
    else
        return false;

    return true;
}

static bool UI_Button_Push(UI_ButtonObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    if (Obj->type == Lock_Btn)
    {
        if (Obj->state == UI_Btn_RlsUp)
            Obj->state = UI_Btn_PushDwn;
        else
            Obj->state = UI_Btn_RlsUp;
    }
    else if (Obj->type == Reset_Btn)
    {
        if (Obj->default_state == Obj->state)
        {
            Obj->state = !Obj->default_state;

            if (Obj->push_callback != NULL)
                Obj->push_callback();
        }
    }
    else
        return false;

    return true;
}

static bool UI_Button_Release(UI_ButtonObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    if (Obj->type == Lock_Btn)
        return true;

    if (Obj->default_state != Obj->state)
    {
        Obj->state = Obj->default_state;

        if (Obj->release_callback != NULL)
            Obj->release_callback();
    }

    return true;
}

static bool UI_Button_Move(UI_ButtonObj_TypeDef *Obj, uint16_t x, uint16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static bool UI_Button_Ctl(UI_ButtonObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    if (Obj->type == Lock_Btn)
    {
        if (Obj->state == UI_Btn_PushDwn)
        {
            if (Obj->push_callback)
                Obj->push_callback();
        }
        else if (Obj->state == UI_Btn_RlsUp)
        {
            if (Obj->release_callback)
                Obj->release_callback();
        }
        else
            return false;
    }

    /* display button on screen object */
    /* invert display color when button been push down */
    if ((UI_DspInterface.draw_str != NULL) && (UI_DspInterface.fill_radius_rectangle != NULL) && (UI_DspInterface.draw_radius_rectangle != NULL))
    {
        if (Obj->state == UI_Btn_PushDwn)
        {
            /* fill button frame */
            UI_DspInterface.fill_radius_rectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->width, Obj->height, DEFAULT_BUTTON_RADIUS, true);

            /* invert string display */
            if (Obj->PushDown_Label != NULL)
                UI_DspInterface.draw_str(base_font, Obj->PushDown_Label, Obj->Gen_Data.x + 5, Obj->Gen_Data.y, true);
        }
        else
        {
            /* draw button frame */
            UI_DspInterface.draw_radius_rectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->width, Obj->height, DEFAULT_BUTTON_RADIUS, 1, true);

            if (Obj->type == Lock_Btn)
            {
                /* display label normally */
                if (Obj->Release_Label != NULL)
                    UI_DspInterface.draw_str(base_font, Obj->Release_Label, Obj->Gen_Data.x + 9, Obj->Gen_Data.y, true);
            }
            else
            {
                if (Obj->PushDown_Label != NULL)
                    UI_DspInterface.draw_str(base_font, Obj->PushDown_Label, Obj->Gen_Data.x + 5, Obj->Gen_Data.y, true);
            }
        }
    }
    else
        return false;

    return true;
}

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, char *label, int16_t x, int16_t y, bool state)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->checked = state;
    Obj->callback = NULL;

    return true;
}

static bool UI_CheckBox_SetCallback(UI_CheckBoxObj_TypeDef *Obj, UI_CheckBoxTrigger_Callback callback)
{
    if (Obj == NULL)
        return false;

    Obj->callback = callback;

    return true;
}

static bool UI_CheckBox_Move(UI_CheckBoxObj_TypeDef *Obj, int16_t x, int16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static bool UI_CheckBox_Trigger(UI_CheckBoxObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    Obj->checked = !Obj->checked;

    if (Obj->callback)
        Obj->callback(Obj->checked);

    return true;
}

static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj)
{
    int16_t frame_x = 0;
    int16_t frame_y = 0;
    int16_t str_x = 0;
    int16_t str_y = 0;

    if ((Obj == NULL) ||
        (UI_DspInterface.draw_str == NULL) ||
        (UI_DspInterface.fill_rectangle == NULL) ||
        (UI_DspInterface.draw_rectangle == NULL))
        return false;

    frame_x = strlen(Obj->Gen_Data.label) * STR_DIS;
    frame_y = Obj->Gen_Data.y + 3;
    frame_x += DEFAULT_CHECKBOX_OFFSET;

    str_x = Obj->Gen_Data.x + 3;
    str_y = Obj->Gen_Data.y;

    if (base_font == Font_8)
        str_y += 1;

    UI_DspInterface.draw_str(base_font, Obj->Gen_Data.label, str_x, str_y, true);
    UI_DspInterface.draw_rectangle(frame_x, frame_y, DEFAULT_CHECKBOX_FRAME_SIZE, DEFAULT_CHECKBOX_FRAME_SIZE, 1, true);

    if (Obj->checked)
        UI_DspInterface.fill_rectangle(frame_x + 2, frame_y + 2, DEFAULT_CHECKBOX_FILLFRAME, DEFAULT_CHECKBOX_FILLFRAME, true);

    return true;
}

static bool UI_SlideBar_Init(UI_SlideBarObj_TypeDef *Obj, UI_SliderBar_Mode_List mode, char *label, int16_t x, int16_t y, int16_t limit_max, int16_t limit_min, int16_t start_val, int16_t step_len)
{
    if ((Obj == NULL) ||
        (limit_max <= limit_min) ||
        (start_val < limit_min) ||
        (start_val > limit_max) ||
        (mode > SliderBar_Vertical_Mode))
        return false;

    Obj->mode = mode;
    Obj->is_selected = false;

    UI_GenData_Init(&(Obj->Gen_Data), label, x, y);

    Obj->limit_max = limit_max;
    Obj->limit_min = limit_min;

    Obj->step_len = step_len;
    Obj->scale = (limit_max - limit_min) / (float)step_len;
    Obj->cur_val = start_val;

    Obj->BarCoord_X = 0;
    Obj->BarCoord_Y = 0;

    return true;
}

static bool UI_SlideBar_Move(UI_SlideBarObj_TypeDef *Obj, int16_t x, int16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static bool UI_SlideBar_SetCallBack(UI_SlideBarObj_TypeDef *Obj, UI_SliderBarTrigger_Callback callback)
{
    if (Obj == NULL)
        return false;

    Obj->callback = callback;

    return true;
}

static bool UI_SlideBar_Input(UI_SlideBarObj_TypeDef *Obj, int16_t *step)
{
    float val_tmp = 0;

    if ((Obj == NULL) || (step == NULL))
        return false;

    val_tmp = Obj->cur_val + ((*step) * Obj->scale);

    if (val_tmp >= Obj->limit_max)
    {
        Obj->cur_val = Obj->limit_max;
    }
    else if (val_tmp <= Obj->limit_min)
    {
        Obj->cur_val = Obj->limit_min;
    }
    else
        Obj->cur_val = val_tmp;

    (*step) = 0;

    return true;
}

static bool UI_SlideBar_Trigger(UI_SlideBarObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    if (Obj->callback != NULL)
        Obj->callback(Obj->cur_val);

    return true;
}

static bool UI_SlideBar_CTL(UI_SlideBarObj_TypeDef *Obj)
{
    int16_t Block_CoordX = 0;
    int16_t Block_CoordY = 0;
    int16_t input_trip = 0;
    int16_t str_y = Obj->Gen_Data.y;

    if ((Obj == NULL) ||
        (UI_DspInterface.draw_str == NULL) ||
        (UI_DspInterface.fill_rectangle == NULL))
        return false;

    if (base_font == Font_8)
        str_y += 1;

    input_trip = Obj->limit_max - Obj->limit_min;

    UI_DspInterface.draw_str(base_font, Obj->Gen_Data.label, Obj->Gen_Data.x + 3, str_y, true);

    if (Obj->mode == SliderBar_Horizon_Mode)
    {
        if (Obj->BarCoord_X == 0)
        {
            Obj->BarCoord_X = Obj->Gen_Data.x + strlen(Obj->Gen_Data.label) * STR_DIS + DEFAULT_SLIDERBAR_OFFSET;
        }

        if (Obj->BarCoord_Y == 0)
        {
            Obj->BarCoord_Y = Obj->Gen_Data.y + 5;
        }

        Block_CoordX = Obj->BarCoord_X;
        Block_CoordX += (Obj->cur_val - Obj->limit_min) / ((float)(input_trip)) * (DEFAULT_SLIDERBAR_LEN - DEFAULT_SLIDERBAR_BLOCK_WIDTH);

        UI_DspInterface.fill_rectangle(Obj->BarCoord_X, Obj->BarCoord_Y, DEFAULT_SLIDERBAR_LEN, DEFAULT_SLIDERBAR_LINESIZE, true);

        Block_CoordY = Obj->Gen_Data.y + 3;
        UI_DspInterface.fill_rectangle(Block_CoordX, Block_CoordY, DEFAULT_SLIDERBAR_BLOCK_WIDTH, DEFAULT_SLIDERBAR_BLOCK_HIGH, true);
        UI_DspInterface.fill_rectangle(Block_CoordX, Block_CoordY + (DEFAULT_SLIDERBAR_BLOCK_HIGH / 3), DEFAULT_SLIDERBAR_BLOCK_WIDTH, DEFAULT_SLIDERBAR_LINESIZE, true);
    }
    else if (Obj->mode == SliderBar_Vertical_Mode)
    {
    }
    else
        return false;

    return true;
}

static void UI_SlideBar_SetSelect(UI_SlideBarObj_TypeDef *Obj, bool select)
{
    if (Obj == NULL)
        return;

    Obj->is_selected = true;
}

static bool UI_SlideBar_IsSelected(UI_SlideBarObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    return Obj->is_selected;
}

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_DspType_List dsp_type, char *label, int16_t x, int16_t y, uint8_t width, int32_t min, int32_t max)
{
    if ((Obj == NULL) || (min > max))
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->width = width;

    switch (base_font)
    {
    case Font_8:
        Obj->height = base_font + 6;
        break;

    case Font_12:
    case Font_16:
        /* need modify */
        Obj->height = base_font + 6;
        break;

    default:
        Obj->height = base_font + 6;
        break;
    }

    Obj->height = base_font + 6;

    Obj->cur_val = 0;
    Obj->max = max;
    Obj->min = min;
    Obj->range = max - min;

    Obj->percent = 0.0;

    Obj->Dsp_Type = dsp_type;
    Obj->Mv_Dir = UI_ProcBar_GrothDir_Default;

    return true;
}

static bool UI_ProcessBar_SetDspDir(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_MoveDir_TypeDef Dir)
{
    if (Obj == NULL)
        return false;

    if ((Dir > UI_ProcBar_GrothFrom_Mid) || (Dir < 0))
        Obj->Mv_Dir = UI_ProcBar_GrothDir_Default;
    else
        Obj->Mv_Dir = Dir;

    return true;
}

static bool UI_ProcessBar_SetCurVal(UI_ProcessBarObj_TypeDef *Obj, int32_t val)
{
    if (Obj == NULL)
        return false;

    int32_t range = Obj->max - Obj->min;

    if (val >= Obj->max)
        Obj->cur_val = Obj->max;
    else if (val <= Obj->min)
        Obj->cur_val = Obj->min;
    else
        Obj->cur_val = val;

    Obj->percent = ((float)val) / range;

    return true;
}

static bool UI_ProcessBar_Move(UI_ProcessBarObj_TypeDef *Obj, uint16_t x, uint16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static bool UI_ProcessBar_DspLoadBar(UI_ProcessBarObj_TypeDef *Obj)
{
    uint8_t frame_radius = 0;
    int16_t Str_CoordX = 0;
    int16_t Str_CoordY = 0;
    float Pcnt_Val = 0;
    char dsp_str[20];

    if (Obj == NULL)
        return false;

    memset(dsp_str, '\0', 20);

    Pcnt_Val = (Obj->cur_val / (float)Obj->range);
    sprintf(dsp_str, "On Process : %d%%", (uint8_t)(Pcnt_Val * 100));

    Str_CoordX = Obj->Gen_Data.x + (Obj->width - (strlen(dsp_str) * STR_DIS)) / 2;
    Str_CoordY = Obj->Gen_Data.y - base_font - 2;

    //display direction from left
    UI_DspInterface.draw_radius_rectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->width, DEFAULT_PROCESSBAR_DOWNLOADTYPE_HEIGHT, DEFAULT_PROCESSBAR_DOWNLOADTYPE_RADIUS, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
    UI_DspInterface.draw_str(base_font, dsp_str, Str_CoordX, Str_CoordY, true);
    UI_DspInterface.draw_line_h(Obj->Gen_Data.x + 2, Obj->Gen_Data.y + 2, Pcnt_Val * (Obj->width - 6), DEFAULT_PROCESSBAR_LINE_WIDTH, true);

    return true;
}

static bool UI_ProcessBar_DspDotBar(UI_ProcessBarObj_TypeDef *Obj)
{
    uint8_t bar_width = 0;
    int16_t bar_start_x = 0;
    int16_t mid_val = Obj->min + Obj->range / 2;
    float Pcnt_Val = 0;

    if (Obj == NULL)
        return false;

    if (Obj->width % 2 == 0)
        bar_width = Obj->width + 1;

    /* step  1 draw dot line */
    UI_Draw_HorDotLine(Obj->Gen_Data.x, Obj->Gen_Data.y + 1, Obj->width, DEFAULT_PROCESSBAR_LINE_WIDTH);

    UI_DspInterface.draw_str(base_font, Obj->Gen_Data.label, Obj->Gen_Data.x, Obj->Gen_Data.y - base_font - 3, true);

    if (Obj->Mv_Dir == UI_ProcBar_GrothFrom_Left)
    {
        bar_start_x = Obj->Gen_Data.x;
        Pcnt_Val = (Obj->cur_val / (float)Obj->range);

        UI_DspInterface.draw_line_v(Obj->Gen_Data.x, Obj->Gen_Data.y - 1, 4, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y, Obj->width * Pcnt_Val, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y + 2, Obj->width * Pcnt_Val, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
    }
    else if (Obj->Mv_Dir == UI_ProcBar_GrothFrom_Mid)
    {
        UI_DspInterface.draw_line_v(Obj->Gen_Data.x + bar_width / 2, Obj->Gen_Data.y - 1, 4, DEFAULT_PROCESSBAR_LINE_WIDTH, true);

        if (Obj->cur_val < mid_val)
        {
            Pcnt_Val = (((Obj->range / 2) - Obj->cur_val) / ((float)(Obj->range / 2)));
            bar_start_x = Obj->Gen_Data.x + bar_width / 2 - (Pcnt_Val * bar_width / 2);
            UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y, (Pcnt_Val * bar_width / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
            UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y + 2, (Pcnt_Val * bar_width / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        }
        else if (Obj->cur_val > mid_val)
        {
            Pcnt_Val = ((Obj->cur_val - (Obj->range / 2)) / ((float)(Obj->range / 2)));
            bar_start_x = Obj->Gen_Data.x + bar_width / 2;
            UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y, (Pcnt_Val * bar_width / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
            UI_DspInterface.draw_line_h(bar_start_x, Obj->Gen_Data.y + 2, (Pcnt_Val * bar_width / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        }
    }
    else
        return false;

    return true;
}

static bool UI_ProcessBar_DspFrameBar(UI_ProcessBarObj_TypeDef *Obj)
{
    int16_t frame_CoordX = 0;
    uint8_t frame_width = 0;
    uint8_t frame_Height = 0;
    int16_t block_start_CoordX = 0;
    int16_t block_start_CoordY = 0;
    int16_t bar_len = 0;
    int16_t mid_val = Obj->min + (Obj->range / 2);
    float Pcnt_Val = 0;

    if ((Obj == NULL) &&
        (UI_DspInterface.draw_str == NULL) &&
        (UI_DspInterface.draw_rectangle == NULL) &&
        (UI_DspInterface.draw_line_h == NULL) &&
        (UI_DspInterface.fill_rectangle == NULL))
        return false;

    frame_CoordX = Obj->Gen_Data.x + strlen(Obj->Gen_Data.label) * STR_DIS + 5;

    switch (base_font)
    {
    case Font_8:
        frame_Height = 5;
        break;

    case Font_12:
    case Font_16:
    default:
        return false;
    }

    if (Obj->width % 2 == 0)
        frame_width = Obj->width + 1;

    UI_DspInterface.draw_str(base_font, Obj->Gen_Data.label, Obj->Gen_Data.x, Obj->Gen_Data.y - base_font - 3, true);
    UI_DspInterface.draw_rectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, frame_width, frame_Height, DEFAULT_PROCESSBAR_LINE_WIDTH, true);

    if (Obj->Mv_Dir == UI_ProcBar_GrothFrom_Left)
    {
        block_start_CoordX = Obj->Gen_Data.x + 2;
        block_start_CoordY = Obj->Gen_Data.y + 2;

        bar_len = (Obj->cur_val / (float)Obj->range) * (frame_width - 5);

        UI_DspInterface.draw_line_h(block_start_CoordX, block_start_CoordY, bar_len, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
    }
    else if (Obj->Mv_Dir == UI_ProcBar_GrothFrom_Mid)
    {
        UI_DspInterface.draw_line_v(Obj->Gen_Data.x + frame_width / 2, Obj->Gen_Data.y, frame_Height - 1, DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        block_start_CoordY = Obj->Gen_Data.y + 2;

        if (Obj->cur_val < mid_val)
        {
            Pcnt_Val = (((Obj->range / 2) - Obj->cur_val) / ((float)(Obj->range / 2)));
            block_start_CoordX = Obj->Gen_Data.x + 1 + frame_width / 2 - (Pcnt_Val * (frame_width - 4) / 2);
            UI_DspInterface.draw_line_h(block_start_CoordX, block_start_CoordY, (Pcnt_Val * (frame_width - 4) / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        }
        else if (Obj->cur_val > mid_val)
        {
            Pcnt_Val = ((Obj->cur_val - (Obj->range / 2)) / ((float)(Obj->range / 2)));

            block_start_CoordX = Obj->Gen_Data.x + frame_width / 2;
            UI_DspInterface.draw_line_h(block_start_CoordX, block_start_CoordY, (Pcnt_Val * (frame_width - 4) / 2), DEFAULT_PROCESSBAR_LINE_WIDTH, true);
        }
    }
    else
        return false;

    return true;
}

static bool UI_ProcessBar_SetLabelPos(UI_ProcessBarObj_TypeDef *Obj, UI_ProcessBar_LabelPos_List Pos)
{
    if ((Obj == NULL) || (Obj->Dsp_Type == UI_ProcBar_DspType_LoadBar))
        return false;

    return true;
}

static bool UI_ProcessBar_Ctl(UI_ProcessBarObj_TypeDef *Obj)
{
    char dig_str[3];
    memset(dig_str, NULL, 3);

    if ((UI_DspInterface.draw_radius_rectangle == NULL) ||
        (UI_DspInterface.draw_str == NULL))
        return false;

    switch (Obj->Dsp_Type)
    {
    case UI_ProcBar_DspType_LoadBar:
        return UI_ProcessBar_DspLoadBar(Obj);

    case UI_ProcBar_DspType_DotBar:
        return UI_ProcessBar_DspDotBar(Obj);

    case UI_ProcBar_DspType_FrameBar:
        return UI_ProcessBar_DspFrameBar(Obj);

    default:
        return false;
    }
}

static bool UI_Drop_Init(UI_DropObj_TypeDef *Obj, char *label, int16_t x, int16_t y)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->CurDrop_Item = NULL;
    Obj->item_cnt = 0;
    Obj->DropList = NULL;
    Obj->is_selected = false;

    return true;
}

static bool UI_Drop_Move(UI_DropObj_TypeDef *Obj, int16_t x, int16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static item_obj *UI_Drop_CreateDropItem(uint8_t id, char *item_desc, void *data, uint16_t data_size, UI_Drop_Callback callback)
{
    item_obj *item_tmp = NULL;
    UI_DropItemDataObj_TypeDef *itemdata_tmp = NULL;

    itemdata_tmp = (UI_DropItemDataObj_TypeDef *)MMU_Malloc(sizeof(UI_DropItemDataObj_TypeDef));
    item_tmp = (item_obj *)MMU_Malloc(sizeof(item_obj));

    if ((item_tmp == NULL) || (itemdata_tmp == NULL))
        return NULL;

    itemdata_tmp->callback = callback;
    itemdata_tmp->data = data;
    itemdata_tmp->describe = item_desc;
    itemdata_tmp->id = id;
    itemdata_tmp->data_size = data_size;

    List_ItemInit(item_tmp, itemdata_tmp);

    return item_tmp;
}

static bool UI_Drop_AddDropItem(UI_DropObj_TypeDef *Obj, char *item_desc, void *data, uint16_t data_size, UI_Drop_Callback callback)
{
    item_obj *item_temp = NULL;

    if (Obj == NULL)
        return false;

    item_temp = UI_Drop_CreateDropItem(Obj->item_cnt, item_desc, data, data_size, callback);

    if (item_temp == NULL)
        return false;

    if (Obj->DropList == NULL)
    {
        Obj->DropList = item_temp;
        Obj->CurDrop_Item = Obj->DropList;
        List_Init(Obj->DropList, item_temp, by_order, NULL);
    }
    else
        List_Insert_Item(Obj->DropList, item_temp);

    Obj->item_cnt++;

    return true;
}

static bool UI_Drop_SetSelect(UI_DropObj_TypeDef *Obj, bool state)
{
    if (Obj == NULL)
        return false;

    Obj->is_selected = state;

    if (!state && (Obj->CurDrop_Item->data != NULL) && (((UI_DropItemDataObj_TypeDef *)Obj->CurDrop_Item->data)->callback != NULL))
    {
        /* trigger drop item callback */
        ((UI_DropItemDataObj_TypeDef *)Obj->CurDrop_Item->data)->callback(((UI_DropItemDataObj_TypeDef *)Obj->CurDrop_Item->data)->data);
    }

    return true;
}

static bool UI_Drop_GetSelect(UI_DropObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    return Obj->is_selected;
}

static bool UI_Drop_SelectItem(UI_DropObj_TypeDef *Obj, int8_t *offset)
{
    item_obj *tmp = NULL;

    if ((Obj == NULL) || (offset > Obj->item_cnt) || (Obj->CurDrop_Item == NULL))
        return false;

    tmp = Obj->CurDrop_Item;

    while (*offset)
    {
        if (tmp == NULL)
            return false;

        if (*offset > 0)
        {
            tmp = tmp->nxt;
            *offset--;
        }
        else if (*offset < 0)
        {
            tmp = tmp->prv;
            *offset++;
        }
    }

    Obj->CurDrop_Item = tmp;

    return true;
}

static bool UI_Drop_Ctl(UI_DropObj_TypeDef *Obj)
{
    int16_t str_x = 0;
    int16_t str_y = 0;
    int16_t dropitem_x = 0;
    int16_t dropitem_y = 0;

    if ((Obj == NULL) &&
        (UI_DspInterface.draw_str == NULL) &&
        (UI_DspInterface.draw_dig == NULL))
        return false;

    str_x = Obj->Gen_Data.x + 3;
    str_y = Obj->Gen_Data.y;

    if (base_font == Font_8)
        str_y += 1;

    /* show label */
    UI_DspInterface.draw_str(base_font, Obj->Gen_Data.label, str_x, str_y, true);

    dropitem_y = str_y + base_font + 2;
    dropitem_x = Obj->Gen_Data.x + base_font * 2;

    /* show drop item id */
    UI_DspInterface.draw_dig(base_font, ((UI_DropItemDataObj_TypeDef *)(Obj->CurDrop_Item->data))->id + 1, dropitem_x, dropitem_y, true);

    /* show drop item label */
    dropitem_x += base_font + 2;
    UI_DspInterface.draw_str(base_font, ((UI_DropItemDataObj_TypeDef *)(Obj->CurDrop_Item->data))->describe, dropitem_x, dropitem_y, true);

    return true;
}

static bool UI_DigInput_Init(UI_DigInputObj_TypeDef *Obj, char *label, int16_t x, int16_t y, UI_DigInput_Type type)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&(Obj->Gen_Data), label, x, y);

    Obj->type = type;
    Obj->selected = false;
    Obj->InputData_Int.CurVal = 0;

    Obj->InputData_Int.Max = 0;
    Obj->InputData_Int.Min = 0;
    Obj->InputData_Int.CurVal = 0;

    Obj->InputData_Dou.Max = 0.0;
    Obj->InputData_Dou.Min = 0.0;
    Obj->InputData_Dou.CurVal = 0.0;

    Obj->InputData_Dou.IntPart = 0;
    Obj->InputData_Dou.PointPart = 0;
    Obj->InputData_Dou.selected_part = DigInput_DefaultPart;

    return true;
}

static bool UI_DigInput_SetIntRange(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, int32_t max, int32_t min, int32_t cur)
{
    if ((Obj == NULL) ||
        (max <= min) ||
        (Obj->type == UI_DoubleDig_Input))
        return false;

    Obj->InputData_Int.Max = max;
    Obj->InputData_Int.Min = min;
    Obj->InputData_Int.effective_len = efft_int_len;
    Obj->InputData_Int.CurVal = cur;

    return true;
}

static bool UI_DigInput_SetDouRange(UI_DigInputObj_TypeDef *Obj, uint8_t efft_int_len, uint8_t efft_point_len, double max, double min, double cur)
{
    if ((Obj == NULL) ||
        (efft_point_len == 0) ||
        (max - min <= 0.00001) ||
        (Obj->type == UI_IntDig_Input))
        return false;

    Obj->InputData_Dou.Max = max;
    Obj->InputData_Dou.Min = min;
    Obj->InputData_Dou.CurVal = cur;
    Obj->InputData_Dou.effective_int_len = efft_int_len;
    Obj->InputData_Dou.effective_point_len = efft_point_len;

    Obj->InputData_Dou.IntPart = (int32_t)cur;
    Obj->InputData_Dou.PointPart = cur - (int32_t)cur;

    Obj->InputData_Dou.selected_part = DigInput_PointPart;

    return true;
}

static bool UI_DigInput_GetDoubleVal(UI_DigInputObj_TypeDef *Obj, double *Out)
{
    if ((Obj == NULL) || (Obj->type == UI_IntDig_Input))
        return false;

    *Out = Obj->InputData_Dou.CurVal;

    return true;
}

static bool UI_DigInput_GetIntVal(UI_DigInputObj_TypeDef *Obj, int32_t *Out)
{
    if ((Obj == NULL) || (Obj->type == UI_DoubleDig_Input))
        return false;

    *Out = Obj->InputData_Int.CurVal;

    return true;
}

static bool UI_DigInput_SetSelectStatus(UI_DigInputObj_TypeDef *Obj, bool status)
{
    if (Obj == NULL)
        return false;

    Obj->selected = status;

    return true;
}

static bool UI_DigInput_GetSelectStatus(UI_DigInputObj_TypeDef *Obj)
{
    if (Obj == NULL)
        return false;

    return Obj->selected;
}

static bool UI_DigInput_Move(UI_DigInputObj_TypeDef *Obj, int16_t x, int16_t y)
{
    if (Obj == NULL)
        return false;

    return UI_Move(&(Obj->Gen_Data), x, y);
}

static bool UI_StrInput_Init()
{
    return true;
}

/***************************************************************** still developing down below **********************************************************************/

static bool UI_VerticlBar_Init(UI_VerticalBarObj_TypeDef *Obj, char *label, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    return true;
}

static bool UI_HorizonBar_Init(UI_HorizonBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    return true;
}

/******************************* control function *********************************/

static bool UI_VerticlBar_Ctl(UI_VerticalBarObj_TypeDef *Obj, uint8_t unit_len)
{
    return true;
}

static bool UI_HorizonBar_Ctl(UI_HorizonBarObj_TypeDef *Obj, uint8_t unit_len)
{
    return true;
}

static bool UI_DigInput_Ctl()
{

    return true;
}

static bool UI_StrInput_Ctl()
{

    return true;
}
