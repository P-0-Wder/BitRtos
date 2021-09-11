#include "UI_Controller.h"

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

static bool UI_GenData_Init(UI_GeneralData_TypeDef *GenData, char *label, UI_Draw UI_Fraw_Func)
{
    GenData->label = label;
    GenData->label_dsp = false;
    GenData->label_roll = false;
    GenData->DrawPoint = UI_Fraw_Func;
}

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t range)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func);

    Obj->Gen_Data.x = x;
    Obj->Gen_Data.y = y;

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
    UI_GenData_Init(&Obj->Gen_Data, label, UI_Fraw_Func);

    return true;
}

static bool UI_HorizonBar_Init(UI_HorizonBarObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

    return true;
}

static bool UI_ProcessCircle_Init(UI_ProcessCircleObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint8_t pcnt)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

    return true;
}

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool state)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

    return true;
}

static bool UI_ComboBox_Init(UI_ComboBoxObj_TypeDef *Obj, UI_Draw UI_Fraw_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if ((UI_Fraw_Func == NULL) || (Obj == NULL))
        return false;

    Obj->Gen_Data.DrawPoint = NULL;

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