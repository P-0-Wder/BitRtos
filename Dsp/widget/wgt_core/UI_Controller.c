#include "UI_Controller.h"
#include <stdlib.h>

/* internal function */

/* external function */

/******************************* general function *********************************/

static bool UI_LabelRoll_Control(UI_GeneralData_TypeDef *GenData, bool state)
{
    if ((GenData == NULL) || (!GenData->label_dsp))
        return false;

    GenData->label_roll = state;

    return true;
}

static bool UI_LabelDsp_Control(UI_GeneralData_TypeDef *GenData, bool state)
{
    if (GenData == NULL)
        return false;

    GenData->label_dsp = state;

    return true;
}

static void UI_GenData_Init(UI_GeneralData_TypeDef *GenData, uint32_t widget, char *label, uint8_t x, uint8_t y)
{
    GenData->label = label;
    GenData->label_dsp = false;
    GenData->label_roll = false;

    GenData->x = x;
    GenData->y = y;

    GenData->selected = false;
    GenData->widget_addr = widget;
}

static bool UI_Selecte(UI_GeneralData_TypeDef *GenData, bool select)
{
    if (GenData == NULL)
        return false;

    GenData->selected = select;

    return true;
}

/******************************* ui init function *********************************/

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, uint32_t widget, UI_DrawPoint UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t range)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->DrawPoint = NULL;
    Obj->DrawPoint = UI_Draw_Func;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);

    Obj->width = width;
    Obj->height = height;

    Obj->cur_val = 0;
    Obj->range = range;

    Obj->percent = 0.0;

    Obj->Mv_Dir = ProcBar_MoveDir_Default;

    return true;
}

static bool UI_VerticlBar_Init(UI_VerticalBarObj_TypeDef *Obj, uint32_t widget, UI_DrawLine UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->DrawLine = NULL;
    Obj->DrawLine = UI_Draw_Func;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);

    return true;
}

static bool UI_HorizonBar_Init(UI_HorizonBarObj_TypeDef *Obj, uint32_t widget, UI_DrawLine UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->DrawLine = NULL;
    Obj->DrawLine = UI_Draw_Func;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);

    return true;
}

static bool UI_ProcessCircle_Init(UI_ProcessCircleObj_TypeDef *Obj, uint32_t widget, UI_DrawPoint UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint32_t range)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->DrawPoint = NULL;
    Obj->DrawPoint = UI_Draw_Func;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);

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

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, uint32_t widget, UI_DrawRectangle UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t frame_size, bool state)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL) || (frame_size <= 4))
        return false;

    Obj->DrawRectangle = NULL;
    Obj->DrawRectangle = UI_Draw_Func;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);
    Obj->Gen_Data.label_dsp = true;

    Obj->checked = false;
    Obj->frame_size = frame_size;

    return true;
}

static bool UI_ComboBox_Init(UI_ComboBoxObj_TypeDef *Obj, uint32_t widget, UI_ComboBox_Group_TypeDef *group, UI_DrawCircle UI_Draw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if ((UI_Draw_Func == NULL) || (Obj == NULL) || (group == NULL) || (group->item_num >= MAX_COMBOBOX_ITEM))
        return false;

    Obj->DrawCircle = NULL;
    Obj->DrawCircle = UI_Draw_Func;
    Obj->radius = radius;

    UI_GenData_Init(&Obj->Gen_Data, widget, label, x, y);
    Obj->Gen_Data.label_dsp = true;

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
    if ((Obj->DrawPoint == NULL) || (!Obj->Gen_Data.selected))
        return false;

    return true;
}

static bool UI_VerticlBar_Ctl(UI_VerticalBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if ((Obj->DrawLine == NULL) || (!Obj->Gen_Data.selected))
        return false;

    return true;
}

static bool UI_HorizonBar_Ctl(UI_HorizonBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if ((Obj->DrawLine == NULL) || (!Obj->Gen_Data.selected))
        return false;

    return true;
}

static bool UI_ProcessCircle_Ctl(UI_ProcessCircleObj_TypeDef *Obj, uint8_t pcnt)
{
    if ((Obj->DrawPoint == NULL) && (!Obj->Gen_Data.selected))
        return false;

    return true;
}

static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj, bool state)
{
    if ((Obj->DrawRectangle == NULL) || (!Obj->Gen_Data.selected))
        return false;

    Obj->checked = state;

    for (uint8_t i = 0; i < Obj->frame_size; i++)
    {
        if (((i > 1) && (Obj->checked)) || (i == 0))
        {
            Obj->DrawRectangle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->frame_size - i, Obj->frame_size - i, 1);
        }
    }

    return true;
}

/* use group control the combo box */
static bool UI_ComboBox_Ctl(UI_ComboBoxObj_TypeDef *Obj, uint8_t state)
{
    UI_ComboBox_Group_TypeDef *group = NULL;

    if ((Obj->DrawCircle == NULL) || (!Obj->Gen_Data.selected))
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
                Obj->DrawCircle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->radius);
            }
            else if (Obj->checked)
            {
                Obj->DrawCircle(Obj->Gen_Data.x, Obj->Gen_Data.y, Obj->radius - i);
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

/******************************* general function *********************************/

static bool UI_Move(UI_GeneralData_TypeDef *Obj, uint8_t x, uint8_t y)
{
    if (Obj == NULL)
        return false;

    Obj->x = x;
    Obj->y = y;

    return true;
}
