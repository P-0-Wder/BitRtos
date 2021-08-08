#ifndef __WIDGET_MNG_H
#define __WIDGET_MNG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef uint32_t Widget_Handle;

#define WIDGET_OPR_ERROR 0
#define WIDGET_CREATE_ERROR WIDGET_OPR_ERROR
#define WIDGET_DELETE_ERROR WIDGET_OPR_ERROR
#define WIDGET_MAX_WIDTH 256
#define WIDGET_MAX_HEIGHT 64
#define MAX_WIDGET_CACHE_PAGE 10

typedef enum
{
    Widget_Show,
    Widget_Hide,
} Widget_State_TypeDef;

typedef struct
{
    Widget_State_TypeDef state;
    uint8_t on_layer;
    uint8_t cord_x;
    uint8_t cord_y;

    uint8_t width;
    uint8_t height;

    uint8_t **cache;

    char *name;
} Widget_TypeDef;

Widget_Handle Widget_Create(uint8_t cord_x, uint8_t cord_y, uint8_t width, uint8_t height);
bool Widget_Deleted(Widget_Handle *hdl);

#endif
