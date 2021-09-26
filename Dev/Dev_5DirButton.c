#include "Dev_5DirButton.h"
#include "Dev_Button.h"

/* internal variable */
static uint8_t DirButton_Sum = 0;

/* internal function */
static uint8_t Dev5DirButton_Get_Sum(void);

/* external variable */

static bool Dev5DirButton_Open()
{
}

static uint8_t Dev5DirButton_Get_Sum(void)
{
    return DirButton_Sum;
}

static bool Dev5DirButton_Invert()
{
}

static DirButton_Val_List Dev5DirButton_Get()
{
}
