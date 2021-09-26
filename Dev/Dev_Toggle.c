#include "Dev_Toggle.h"
#include "periph_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t toggle_2level_sum = 0;
static uint8_t toggle_3level_sum = 0;

/* internal function */

/* external variable */

static bool Toggle_Init(Toggle_Obj_TypeDef *obj, char *name)
{
    if (obj == NULL)
        return false;

    /* ToDo IO init */

    return true;
}
