#include "drv_timer.h"

/* internal function */

/* internal variable */

/* external function */
static bool DrvTimer_Obj_Init(DrvTimer_Obj_TypeDef *obj);
static bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint32_t len);
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
    obj->init = false;

    return true;
}

static bool DrvTimer_Ctl(DrvTimer_CMD_List cmd, uint32_t p_data, uint32_t len)
{
    if ((((DrvTimer_Obj_TypeDef *)p_data)->timerx < 0) || (((DrvTimer_Obj_TypeDef *)p_data)->timerx >= Timer_Port_Sum))
        return false;

    switch (cmd)
    {
    case DrvTimer_Encoder_Mode:
        if ((len != sizeof(DrvTimer_Obj_TypeDef)) || (p_data == 0))
            return false;

        ((DrvTimer_Obj_TypeDef *)p_data)->mode = DrvTimer_Encoder_Mode;

        if ((((DrvTimer_Obj_TypeDef *)p_data)->timerx < Timer_2) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->timerx > Timer_8) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a == ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a < TIM_Channel_1) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a > TIM_Channel_4) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b < TIM_Channel_1) ||
            (((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b > TIM_Channel_4))
            return false;

        periph_Timer_Encoder_Mode_Init(((DrvTimer_Obj_TypeDef *)p_data)->timerx,
                                       ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_a,
                                       ((DrvTimer_Obj_TypeDef *)p_data)->enc_ch_b);

        ((DrvTimer_Obj_TypeDef *)p_data)->init = true;
        return true;

    case DrvTimer_Counter_Mode:
        periph_Timer_CounterMode_Init(((DrvTimer_Obj_TypeDef *)p_data)->timerx,
                                      ((DrvTimer_Obj_TypeDef *)p_data)->Prescaler,
                                      ((DrvTimer_Obj_TypeDef *)p_data)->period,
                                      ((DrvTimer_Obj_TypeDef *)p_data)->PreemptionPriority,
                                      ((DrvTimer_Obj_TypeDef *)p_data)->SubPriority);

        periph_Timer_Counter_SetEnable(((DrvTimer_Obj_TypeDef *)p_data)->timerx, DISABLE);
        ((DrvTimer_Obj_TypeDef *)p_data)->init = true;
        return true;

    case DrvTimer_Counter_SetState:
        if (((DrvTimer_Obj_TypeDef *)p_data)->init == false)
            return false;

        periph_Timer_Counter_SetEnable(((DrvTimer_Obj_TypeDef *)p_data)->timerx, len);
        return true;

    case DrvTimer_Counter_SetIRQCallback:
        if (((DrvTimer_Obj_TypeDef *)p_data)->init == false)
            return false;

        return periph_Timer_SetCountIRQ_Callback(((DrvTimer_Obj_TypeDef *)p_data)->timerx, len);

    default:
        return false;
    }
}

static int32_t DrvTImer_GetEncoder(DrvTimer_Obj_TypeDef *obj)
{
    if ((obj == NULL) || (obj->mode != DrvTimer_Encoder_Mode) || !obj->init)
        return 0;

    obj->cnt += periph_Timer_GetEncoder_Input(obj->timerx);

    return obj->cnt;
}
