#include "UI_Controller.h"

/* internal function */

/* external function */

/******************************* init function *********************************/

static bool UI_ProcessBar_Init(uint8_t **map, UI_ProcessBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t pcnt)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_VerticlBar_Init(uint8_t **map, UI_VerticalBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_HorizonBar_Init(uint8_t **map, UI_HorizonBarObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_ProcessCircle_Init(uint8_t **map, UI_ProcessCircleObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint8_t pcnt)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_CheckBox_Init(uint8_t **map, UI_CheckBoxObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool state)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

static bool UI_ComboBox_Init(uint8_t **map, UI_ComboBoxObj_TypeDef *Obj, char *label, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if ((map == NULL) || (Obj == NULL))
        return false;

    return true;
}

/******************************* control function *********************************/

static bool UI_ProcessBar_Ctl(UI_ProcessBarObj_TypeDef *Obj, uint8_t pcnt)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}

static bool UI_VerticlBar_Ctl(UI_VerticalBarObj_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}

static bool UI_HorizonBar_Ctl(UI_HorizonBarObj_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t unit_len)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}

static bool UI_ProcessCircle_Ctl(UI_ProcessCircleObj_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_width, uint8_t pcnt)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}

static bool UI_CheckBox_Ctl(UI_CheckBoxObj_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool state)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}

static bool UI_ComboBox_Ctl(UI_ComboBoxObj_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t radius, uint8_t state)
{
    if (Obj->Gen_Data.widget_hdl == 0)
        return false;

    return true;
}
