#include "widget_mng.h"

Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height)
{
    Widget_TypeDef *widget_tmp;

    widget_tmp = (Widget_TypeDef *)malloc(sizeof(Widget_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    return (Widget_Handle)widget_tmp;
}

bool Widget_Deleted(Widget_Handle *hdl)
{
}
