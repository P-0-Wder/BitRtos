#include "stm32f4xx.h"
#include "reboot.h"

void ReBoot(void)
{
    __set_FAULTMASK(1);

    NVIC_SystemReset();
}
