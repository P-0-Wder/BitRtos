#ifndef __DEV_TOGGLE_H
#define __DEV_TOGGLE_H

#include <stdint.h>
#include <stdbool.h>
#include "Input_IO_Def.h"

#define DEFAULT_TOGGLE_LEVEL Toggle_3Level

typedef enum
{
    Toggle_Up = 0,
    Toggle_Dwn,
    Toggle_Mid,
    Toggle_Err,
} DevToggle_Pos_List;

typedef enum
{
    Toggle_2Level = 1,
    Toggle_3Level = 2,
} DevToggle_Type_List;

typedef struct
{
    DevToggle_Type_List level;
    bool invert;
    DrvGPIO_Obj_TypeDef *IO_Ptr[DEFAULT_TOGGLE_LEVEL];
} Toggle_Obj_TypeDef;

typedef struct
{
    bool (*Open)(Toggle_Obj_TypeDef *Obj, DevToggle_Type_List type, DrvGPIO_Obj_TypeDef *io);
    bool (*Invert)(Toggle_Obj_TypeDef *Obj);
    DevToggle_Pos_List (*Get)(Toggle_Obj_TypeDef *Obj);
} DevToggle_TypeDef;

extern DevToggle_TypeDef DevToggle;

#endif
