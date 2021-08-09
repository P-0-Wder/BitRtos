#include "widget_mng.h"

Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height, char *name)
{
    Widget_TypeDef *widget_tmp;

    widget_tmp = (Widget_TypeDef *)malloc(sizeof(Widget_TypeDef));

    if (widget_tmp == NULL)
        return WIDGET_CREATE_ERROR;

    widget_tmp->cord_x = cord_x;
    widget_tmp->cord_y = cord_y;

    widget_tmp->width = width;
    widget_tmp->height = height;

    widget_tmp->name = name;
    widget_tmp->on_layer = DEFAULT_LAYER;

    widget_tmp->cache = (uint8_t **)malloc(height);

    if (widget_tmp->cache == NULL)
        return WIDGET_CREATE_ERROR;

    for (uint8_t h = 0; h < height; h++)
    {
        widget_tmp->cache[h] = (uint8_t *)malloc(width);

        if (widget_tmp->cache[h] == NULL)
            return WIDGET_CREATE_ERROR;
    }

    widget_tmp->state = Widget_Created;

    return (Widget_Handle)widget_tmp;
}

bool Widget_Deleted(Widget_Handle *hdl)
{

    return false;
}
