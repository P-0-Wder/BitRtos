#include "Dev_5DirButton.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t DirButton_Sum = 0;

/* internal function */
static uint8_t Dev5DirButton_Get_Sum(void);

/* external variable */

static bool Dev5DirButton_Open(DirButton_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t io_num)
{
    if ((obj == NULL) || (io_num != Dir_IO_Sum))
        return false;

    obj->invert_reg = None_Invert;

    for (uint8_t i = 0; i < io_num; i++)
    {
        /* ToDo IO Init */
    }

    return true;
}

static uint8_t Dev5DirButton_Get_Sum(void)
{
    return DirButton_Sum;
}

static bool Dev5DirButton_Invert(DirButton_Obj_TypeDef *obj, uint8_t val)
{
    if (obj == NULL)
        return false;

    obj->invert_reg = val;

    return true;
}

static DirButton_Val_List Dev5DirButton_Get(DirButton_Obj_TypeDef *obj)
{
    uint8_t io_val_reg = 0;

    if (obj == NULL)
        return Dir_Err;

    for (uint8_t i = 0; i < Dir_IO_Sum; i++)
    {
        io_val_reg |= DrvGPIO.get(&obj->IO[i]) << i;
    }
    /* except mid button, other button detect been hit at same time then return error */
}
