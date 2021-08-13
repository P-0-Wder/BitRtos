#include "widget_mng.h"

/* internal function */

/* external function */
Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name);
bool Widget_Deleted(Widget_Handle *hdl);
void Widget_Show(Widget_Handle *hdl);
void Widget_Hide(Widget_Handle *hdl);
void Widget_MovdeDis(int8_t x, int8_t y);
void Widget_MoveTo(uint8_t x, uint8_t y);

Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name)
{
    WidgetObj_TypeDef *widget_tmp;

    widget_tmp = (WidgetObj_TypeDef *)malloc(sizeof(WidgetObj_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->cord_x = cord_x;
    widget_tmp->cord_y = cord_y;

    widget_tmp->width = width;
    widget_tmp->height = height;

    widget_tmp->name = name;
    widget_tmp->on_layer = DEFAULT_LAYER;

    widget_tmp->pixel_map = (uint8_t **)malloc(sizeof(uint8_t *) * height);
    for (uint8_t h = 0; h < height; h++)
    {
        widget_tmp->pixel_map[h] = (uint8_t *)malloc(width);
    }

    if (widget_tmp->pixel_map == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->state = Widget_Created;

    return (Widget_Handle)widget_tmp;
}

bool Widget_Deleted(Widget_Handle *hdl)
{

    return false;
}
