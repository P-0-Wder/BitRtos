#include "drv_timer.h"

/* internal function */

/* internal variable */

/* external function */
static bool DrvTimer_Obj_Init(DrvTimer_Obj_TypeDef *obj);
static bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len);
static int32_t DrvTImer_GetEncoder(DrvTimer_Obj_TypeDef *obj);

/* external variable */
DrvTimer_TypeDef DrvTimer = {
    .obj_clear = DrvTimer_Obj_Init,
    .ctl = DrvTimer_Ctl,
    .get = DrvTImer_GetEncoder,
};

static bool DrvTimer_Obj_Init(DrvTimer_Obj_TypeDef *obj)
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

static bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len)
{
    switch (cmd)
    {
    case DrvTimer_Encoder_Mode:
        if ((len != sizeof(DrvTimer_Obj_TypeDef)) || (p_data == 0))
            return false;

        ((DrvTimer_Obj_TypeDef *)p_data)->mode = DrvTimer_Encoder_Mode;

        if ((((DrvTimer_Obj_TypeDef *)p_data)->timerx < Timer_2) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->timerx > Timer_4) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a == ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a < TIM_Channel_1) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a < TIM_Channel_4) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b < TIM_Channel_1) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b < TIM_Channel_4))
            return false;

        periph_Timer_Encoder_Mode_Init(((DrvTimer_Obj_TypeDef *)p_data)->timerx,
                                       ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a,
                                       ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b);
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

static int32_t DrvTImer_GetEncoder(DrvTimer_Obj_TypeDef *obj)
{
    if ((obj == NULL) || (obj->mode != DrvTimer_Encoder_Mode))
        return 0;

    obj->cnt += periph_Timer_GetEncoder_Input(obj->timerx);

    return obj->cnt;
}
