#include "Dev_Gimbal.h"
#include "drv_adc.h"
#include "drv_gpio.h"

/* internal variable */
static uint8_t gimbal_cnt = 0;

/* internal function */

/* external variable */
DevGimbal_TypeDef DevGimbal_Left = {};
DevGimbal_TypeDef DevGimbal_Right = {};

static bool DevGimbal_Open()
{
    return true;
}
