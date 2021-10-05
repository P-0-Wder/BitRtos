#include "drv_timer.h"

/* internal function */

/* internal variable */

/* external function */
bool DrvTimer_Obj_Init(DrvTimer_Obj_TypeDef *obj);
bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len);
int32_t DrvTImer_GetEncoder(DrvTimer_Obj_TypeDef *obj);

/* external variable */
DrvTimer_TypeDef DrvTimer = {
    .obj_clear = DrvTimer_Obj_Init,
    .ctl = DrvTimer_Ctl,
    .get = DrvTImer_GetEncoder,
};

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
    obj->cnt = 0;

    return true;
}

bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len)
{

    switch (cmd)
    {
    case DrvTimer_Encoder_Mode:
        if ((len != sizeof(DrvTimer_Obj_TypeDef)) || (p_data == 0))
            return false;

        ((DrvTimer_Obj_TypeDef *)p_data)->mode = DrvTimer_Encoder_Mode;

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

int32_t DrvTImer_GetEncoder(DrvTimer_Obj_TypeDef *obj)
{
    if ((obj == NULL) || (obj->mode != DrvTimer_Encoder_Mode))
        return 0;

    obj->cnt += periph_Timer_GetEncoder_Input(obj->timerx);

    return obj->cnt;
}
