#include "drv_timer.h"

bool DrvTimer_Obj_Init(DrvTimer_Obj_TypeDef *obj)
{
    if (obj == NULL)
        return false;

    obj->irq = false;
    obj->period = 0;
    obj->PreemptionPriority = 0;
    obj->Prescaler = 0;
    obj->SubPriority = 0;
    obj->timerx = Timer_None;

    return true;
}

bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len)
{
    switch (cmd)
    {
    case DrvTimer_Encoder_Mode:
        if ((len != sizeof(DrvTimer_Obj_TypeDef)) || (p_data == 0))
            return false;

        periph_Timer_Encoder_Mode_Init(((DrvTimer_Obj_TypeDef *)p_data)->timerx);
        return true;

    case DrvTimer_Counter_Mode:
        /* TODO */
        /* need set Counter IRQ Callback */
        /* not inuse yet */
        return false;

    default:
        return false;
    }
}

int32_t DrvTImer_Get(DrvTimer_Obj_TypeDef *obj)
{
    if (obj == NULL)
        return 0;
}
