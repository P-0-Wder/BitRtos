#include "UI_Controller.h"
#include <stdlib.h>

/* internal function */

/* external function */

/******************************* init function *********************************/

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

static bool UI_GenData_Init(UI_GeneralData_TypeDef *GenData, char *label, UI_Draw UI_Fraw_Func, uint8_t x, uint8_t y)
{
    GenData->label = label;
    GenData->label_dsp = false;
    GenData->label_roll = false;
    GenData->DrawPoint = UI_Fraw_Func;

    GenData->x = x;
    GenData->y = y;
}

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t range)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

    Obj->width = width;
    Obj->height = height;

    Obj->cur_val = 0;
    Obj->range = range;

    Obj->percent = 0.0;

    Obj->Mv_Dir = ProcBar_MoveDir_Default;

    return true;
}

static bool UI_VerticlBar_Init(UI_VerticalBarObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

    return true;
}

static bool UI_HorizonBar_Init(UI_HorizonBarObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

    return true;
}

static bool UI_ProcessCircle_Init(UI_ProcessCircleObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint32_t range)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

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

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t frame_size, bool state)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

    Obj->checked = false;
    Obj->frame_size = frame_size;

    return true;
}

static bool UI_ComboBox_Init(UI_ComboBoxObj_TypeDef *Obj, UI_ComboBox_Group_TypeDef *group, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL) || (group == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func, x, y);

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

/******************************* control function *********************************/

static bool UI_ProcessBar_Ctl(UI_ProcessBarObj_TypeDef *Obj, uint8_t pcnt)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

    return true;
}

static bool UI_VerticlBar_Ctl(UI_VerticalBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

    return true;
}

static bool UI_HorizonBar_Ctl(UI_HorizonBarObj_TypeDef *Obj, uint8_t unit_len)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

    return true;
}

static bool UI_ProcessCircle_Ctl(UI_ProcessCircleObj_TypeDef *Obj, uint8_t pcnt)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

    return true;
}

static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj, bool state)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

    Obj->checked = state;

    return true;
}

/* use group control the combo box */
static bool UI_ComboBox_Ctl(UI_ComboBoxObj_TypeDef *Obj, uint8_t state)
{
    if (Obj->Gen_Data.DrawPoint == NULL)
        return false;

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