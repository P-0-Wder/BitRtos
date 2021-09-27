#ifndef __DEV_BUTTON_H
#define __DEV_BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include "Input_IO_Def.h"

typedef struct
{
    bool init_state;
    bool invert;
    DrvGPIO_Obj_TypeDef *io;
} Button_Obj_TypeDef;

typedef enum
{
    Btn_Up = 0,
    Btn_Dwn,
    Btn_Err,
} Button_State_List;

typedef struct
{
    bool (*Open)(Button_Obj_TypeDef *Obj, DrvGPIO_Obj_TypeDef *io);
    bool (*Invert)(Button_Obj_TypeDef *Obj);
    Button_State_List (*Get)(Button_Obj_TypeDef *Obj);
    uint8_t (*get_button_sum)(void);
} DevButton_TypeDef;

extern DevButton_TypeDef DrvButton;

#endif
