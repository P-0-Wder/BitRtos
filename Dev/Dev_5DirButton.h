#ifndef __DEV_5DIRBUTTON_H
#define __DEV_5DIRBUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include "Input_IO_Def.h"

#define SetBit(x) 1 << x

typedef enum
{
    Axis_X_Invert = SetBit(0),
    Axis_Y_Invert = SetBit(1),
    Mid_Invert = SetBit(2),
    None_Invert = 0,
} DirButton_Invert_List;

typedef enum
{
    Dir_Up = SetBit(0),
    Dir_Dwn = SetBit(1),
    Dir_Lft = SetBit(2),
    Dir_Rgt = SetBit(3),
    Dir_Mid = SetBit(4),
    Dir_Err = 5,
} DirButton_Val_List;

typedef enum
{
    Dir_IO_Up = 0,
    Dir_IO_Dwn,
    Dir_IO_Lft,
    Dir_IO_Rgt,
    Dir_IO_Mid,
    Dir_IO_Sum,
} DirButton_IO_Serials_List;

typedef struct
{
    uint8_t invert_reg;

    DrvGPIO_Obj_TypeDef IO[Dir_IO_Sum];
} DirButton_Obj_TypeDef;

typedef struct
{
    bool (*open)(DirButton_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t io_num);
    bool (*invert)(DirButton_Obj_TypeDef *obj, uint8_t val);
    DirButton_Val_List (*get)(DirButton_Obj_TypeDef *obj);
} Dev5DirButton_TypeDef;

extern Dev5DirButton_TypeDef Dev5DirBtn;

#endif
