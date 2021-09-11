#include "UI_Controller.h"

/* internal function */

/* external function */

/******************************* init function *********************************/

static bool UI_ProcessBar_Init(UI_ProcessBarObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t pcnt)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_VerticlBar_Init(UI_VerticalBarObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_HorizonBar_Init(UI_HorizonBarObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_ProcessCircle_Init(UI_ProcessCircleObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint8_t pcnt)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_CheckBox_Init(UI_CheckBoxObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool state)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_ComboBox_Init(UI_ComboBoxObj_TypeDef *Obj, UI_DrawPonit UI_DrawPoint_Func, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    Obj->Gen_Data.DrawPoint = NULL;

    if ((UI_DrawPoint_Func == NULL) || (Obj == NULL))
        return false;

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