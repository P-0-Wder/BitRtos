#include "Dev_5DirButton.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t DirButton_Sum = 0;

/* internal function */
static bool Dev5DirButton_Open(DirButton_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t io_num);
static bool Dev5DirButton_Invert(DirButton_Obj_TypeDef *obj, uint8_t val);
static DirButton_Val_List Dev5DirButton_Get(DirButton_Obj_TypeDef *obj);

static uint8_t Dev5DirButton_Get_Sum(void);

/* external variable */
Dev5DirButton_TypeDef Dev5DirBtn = {
    .open = Dev5DirButton_Open,
    .invert = Dev5DirButton_Invert,
    .get = Dev5DirButton_Get,
};

static bool Dev5DirButton_Open(DirButton_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t io_num)
{
    if ((obj == NULL) || (io == NULL) || (io_num != Dir_IO_Sum))
    {
        obj->init_state = false;
        return false;
    }

    obj->invert_reg = None_Invert;

    for (uint8_t i = 0; i < io_num; i++)
    {
        /* ToDo IO Init */
        DrvGPIO.open(&obj[i], GPIO_Input, NULL);
    }

    obj->init_state = true;
    return true;
}

static uint8_t Dev5DirButton_Get_Sum(void)
{
    return DirButton_Sum;
}

static bool Dev5DirButton_Invert(DirButton_Obj_TypeDef *obj, uint8_t val)
{
    if ((obj == NULL) || (!obj->init_state))
        return false;

    obj->invert_reg = val;

    return true;
}

static DirButton_Val_List Dev5DirButton_Get(DirButton_Obj_TypeDef *obj)
{
    uint8_t io_val = 0;
    uint8_t io_val_tmp = 0;
    uint8_t hit_cnt = 0;

    if ((obj == NULL) || (!obj->init_state))
        return Dir_Err;

    for (uint8_t i = 0; i < Dir_IO_Sum; i++)
    {
        if (DrvGPIO.get(&obj->IO[i]))
        {
            io_val |= SetBit(i);
            hit_cnt++;
        }

        /* except mid button, other button detect been hit at same time then return error */
        if ((hit_cnt > 1) && (i < Dir_IO_Mid))
        {
            return Dir_Err;
        }
    }

    if (obj->invert_reg & Axis_X_Invert)
    {
        io_val_tmp = io_val;

        io_val_tmp &= 0x0C;
        io_val_tmp = ~io_val_tmp;

        io_val &= ~0x0C;
        io_val |= io_val_tmp;
    }

    if (obj->invert_reg & Axis_Y_Invert)
    {
        io_val_tmp = io_val;

        io_val_tmp &= 0x03;
        io_val_tmp = ~io_val_tmp;

        io_val &= ~0x03;
        io_val |= io_val_tmp;
    }

    if (obj->invert_reg & Mid_Invert)
    {
        io_val_tmp = io_val;

        io_val_tmp &= 0x10;
        io_val_tmp = ~io_val_tmp;

        io_val &= ~0x10;
        io_val |= io_val_tmp;
    }

    return io_val;
}
