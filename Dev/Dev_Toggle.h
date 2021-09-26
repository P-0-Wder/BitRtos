#ifndef __DEV_TOGGLE_H
#define __DEV_TOGGLE_H

#include <stdint.h>
#include <stdbool.h>
#include "periph_gpio.h"

#define DEFAULT_TOGGLE_LEVEL 3

typedef enum
{
    Toggle_Up = 0,
    Toggle_Mid,
    Toggle_Dwn,
} DevToggle_Pos_List;

typedef enum
{
    Toggle_2Level = 2,
    Toggle_3Level = 3,
} DevToggle_Type_List;

typedef struct
{
    DevToggle_Type_List level_cnt;
    DevToggle_Pos_List pos;
} Toggle_Obj_TypeDef;

#endif
