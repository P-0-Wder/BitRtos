#include "UI_Controller.h"
#include <stdlib.h>

#define DEFAULT_FONT font_12
#define DEFAULT_BUTTON_PUSH_LABEL "yes"
#define DEFAULT_BUTTON_RELEASE_LABEL "no"
#define DEFAULT_BUTTON_WIDTH 25
#define DEFAULT_BUTTON_HEIGHT 14
#define DEFAULT__BUTTON_RADIUS 3

/* internal object */
static UI_DrawInterface_TypeDef UI_DspInterface;

/* external function */
static bool UI_Button_Init(UI_ButtonObj_TypeDef *Obj, char *label, int8_t x, int8_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state);
static bool UI_Button_Set_Label(UI_ButtonObj_TypeDef *Obj, UI_Button_State_List state, char *label);
static bool UI_Button_Set_TriggerCallback(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_Trigger_Callback callback);
static bool UI_Button_Push(UI_ButtonObj_TypeDef *Obj);
static bool UI_Button_Release(UI_ButtonObj_TypeDef *Obj);
static bool UI_Button_Move(UI_ButtonObj_TypeDef *Obj, uint8_t x, uint8_t y);
static bool UI_Button_Ctl(UI_ButtonObj_TypeDef *Obj);

/* general function */
static bool UI_Get_InitSate(UI_GeneralData_TypeDef GenData);
static bool UI_Selecte(UI_GeneralData_TypeDef *GenData, bool select);
static bool UI_Get_Selected(UI_GeneralData_TypeDef GenData);
static bool UI_Move(UI_GeneralData_TypeDef *GenUI_Info, int8_t dst_x, int8_t dst_y);

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

/******************************* general function *********************************/

void UI_Set_DspInterface(UI_DrawPoint point,
                         UI_DrawLine line,
                         UI_DrawRectangle rectangle,
                         UI_DrawRadiusRectangle radius_rectangle,
                         UI_DrawCircle circle,
                         UI_DrawStr str,
                         UI_FillCircle fill_circle,
                         UI_FillRectangle fill_rectangle,
                         UI_FillRadiusRectangle fill_radius_rectangle)
{
    UI_DspInterface.draw_circle = circle;
    UI_DspInterface.draw_line = line;
    UI_DspInterface.draw_point = point;
    UI_DspInterface.draw_rectangle = rectangle;
    UI_DspInterface.draw_radius_rectangle = radius_rectangle;
    UI_DspInterface.draw_str = str;

    UI_DspInterface.fill_circle = fill_circle;
    UI_DspInterface.fill_radius_rectangle = fill_radius_rectangle;
    UI_DspInterface.fill_rectangle = fill_rectangle;
}

/* still in developing about this selector */
bool UI_ShowSelector(WidgetUI_Item_TypeDef *item)
{
    if (item == NULL)
        return false;

    switch ((uint8_t)(item->type))
    {
    case UI_Type_Button:
        // show button selector
        break;

    default:
        break;
    }
    return true;
}

static void UI_GenData_Init(UI_GeneralData_TypeDef *GenData, char *label, uint8_t x, uint8_t y)
{
    GenData->label = label;

    GenData->x = x;
    GenData->y = y;

    GenData->init = false;
}

static bool UI_Move(UI_GeneralData_TypeDef *GenUI_Info, int8_t dst_x, int8_t dst_y)
{
    if (GenUI_Info == NULL)
        return false;

    GenUI_Info->x = dst_x;
    GenUI_Info->y = dst_y;

    return true;
}

/********************************************** UI Button Object ***************************************************/
/*
*  the operation of button ctl is a async operation
*  exti irq or input signal triiger first then out of the trigger code 
*  check signal value doing process
*/
static bool UI_Button_Init(UI_ButtonObj_TypeDef *Obj, char *label, int8_t x, int8_t y, uint8_t width, uint8_t height, UI_Button_Type type, UI_Button_State_List state)
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

    Obj->Gen_Data.init = true;

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

static bool UI_Button_Set_TriggerCallback(UI_ButtonObj_TypeDef *Obj, UI_Button_Trigger_Type type, UI_Trigger_Callback callback)
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

static bool UI_Button_Move(UI_ButtonObj_TypeDef *Obj, uint8_t x, uint8_t y)
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
            Obj->push_callback();
        }
        else if (Obj->state == UI_Btn_RlsUp)
        {
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
            UI_DspInterface.fill_radius_rectangle(Obj->Gen_Data.x + 1, Obj->Gen_Data.y + 1, Obj->width - 1, Obj->height - 1, Default_Button_FrameRadius);

            /* invert string display */
            if (Obj->PushDown_Label != NULL)
                UI_DspInterface.draw_str(Default_Font, Obj->PushDown_Label, Obj->Gen_Data.x, Obj->Gen_Data.y, true);
        }
        else
        {
            /* draw button frame */
            UI_DspInterface.draw_radius_rectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->width, Obj->height, Default_Button_FrameRadius, 1);

            /* display label normally */
            if (Obj->Release_Label != NULL)
                UI_DspInterface.draw_str(Default_Font, Obj->Release_Label, Obj->Gen_Data.x, Obj->Gen_Data.y, false);
        }
    }
    else
        return false;

    return true;
}

/***************************************************************** still developing down below **********************************************************************/

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t range)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->width = width;
    Obj->height = height;

    Obj->cur_val = 0;
    Obj->range = range;

    Obj->percent = 0.0;

    Obj->Mv_Dir = ProcBar_MoveDir_Default;
    Obj->Gen_Data.init = true;

    return true;
}

static bool UI_VerticlBar_Init(UI_VerticalBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
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

static bool UI_ProcessCircle_Init(UI_ProcessCircleObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint32_t range)
{
    if (Obj == NULL)
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->radius = radius;
    Obj->percent = 0.0;
    Obj->range = range;
    Obj->cur_val = 0;

    return true;
}

static bool UI_ComboBoxGroup_Init(UI_ComboBox_Group_TypeDef *group, char *label)
{
    if (group == NULL)
        return false;

    group->item_num = 0;
    group->label = label;

    for (uint8_t i = 0; i < MAX_COMBOBOX_ITEM; i++)
    {
        group->Obj[i] = NULL;
    }

    return true;
}

static UI_ComboBox_Group_TypeDef *UI_ComboGroup_Create(char *label)
{
    UI_ComboBox_Group_TypeDef *ComboGroup_Tmp = NULL;

    ComboGroup_Tmp = (UI_ComboBox_Group_TypeDef *)malloc(sizeof(UI_ComboBox_Group_TypeDef));

    if (ComboGroup_Tmp == NULL)
        return NULL;

    UI_ComboBoxGroup_Init(ComboGroup_Tmp, label);

    return ComboGroup_Tmp;
}

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t frame_size, bool state)
{
    if ((Obj == NULL) || (frame_size <= 4))
        return false;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->checked = false;
    Obj->frame_size = frame_size;

    return true;
}

static bool UI_ComboBox_Init(UI_ComboBoxObj_TypeDef *Obj, UI_ComboBox_Group_TypeDef *group, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if ((Obj == NULL) || (group == NULL) || (group->item_num >= MAX_COMBOBOX_ITEM))
        return false;

    Obj->radius = radius;

    UI_GenData_Init(&Obj->Gen_Data, label, x, y);

    Obj->item_id = group->item_num;

    if (group->item_num == 0)
    {
        Obj->checked = true;
    }
    else
        Obj->checked = false;

    Obj->group_ptr = group;

    group->Obj[group->item_num] = Obj;
    group->item_num++;

    return true;
}

static bool UI_DigInput_Init()
{
    return true;
}

static bool UI_StrInput_Init()
{
    return true;
}

static bool UI_Drop_Init()
{

    return true;
}

/******************************* control function *********************************/

static bool UI_ProcessBar_Ctl(UI_ProcessBarObj_TypeDef *Obj, uint8_t pcnt)
{
    return true;
}

static bool UI_VerticlBar_Ctl(UI_VerticalBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if (Obj->DrawLine == NULL)
        return false;

    return true;
}

static bool UI_HorizonBar_Ctl(UI_HorizonBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if (Obj->DrawLine == NULL)
        return false;

    return true;
}

static bool UI_ProcessCircle_Ctl(UI_ProcessCircleObj_TypeDef *Obj, uint8_t pcnt)
{
    if (Obj->DrawPoint == NULL)
        return false;

    return true;
}

static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj, bool state)
{
    if (Obj->DrawRectangle == NULL)
        return false;

    Obj->checked = state;

    return true;
}

/* use group control the combo box */
static bool UI_ComboBox_Ctl(UI_ComboBoxObj_TypeDef *Obj, uint8_t state)
{
    UI_ComboBox_Group_TypeDef *group = NULL;

    if (Obj->DrawCircle == NULL)
        return false;

    group = (UI_ComboBox_Group_TypeDef *)Obj->group_ptr;

    Obj->checked = state;

    for (uint8_t i = 0; i < group->item_num; i++)
    {
        if (Obj->checked)
        {
            group->Obj[i]->checked = false;
        }
    }

    for (uint8_t i = 0; i < Obj->radius; i++)
    {
        if (i != 1)
        {
            if (i == 0)
            {
                Obj->DrawCircle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->radius, 1);
            }
            else if (Obj->checked)
            {
                Obj->DrawCircle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->radius - i, 1);
            }
        }
    }

    group->Obj[Obj->item_id]->checked = Obj->checked;

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

static bool UI_Drop_Ctl()
{

    return true;
}
