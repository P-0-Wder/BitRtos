#include "Dev_Encoder.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include <string.h>

/* internal variable */
static uint8_t encoder_sum = 0;

/* internal function */
static void DevEncoder_EXTI_InternalCallBack(void);

/* external funtion */
static bool DevEncoder_Open(DevEncoder_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t btn_enable, uint8_t timerx, uint16_t ch_a, uint16_t ch_b);
static bool DevEncoder_Invert(DevEncoder_Obj_TypeDef *obj, uint8_t invert_val);
static bool DevEncoder_Set_BtnCallback(DevEncoder_Obj_TypeDef *obj, Encoder_Btn_Callback callback);
static bool DevEncoder_ButtonTrigger(DevEncoder_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *btn_io);
static Encoder_Data_TypeDef DevEncoder_Get(DevEncoder_Obj_TypeDef *obj);
static uint8_t DevEncoder_GetNum(void);

/* external variable */
DevEncoder_TypeDef DevEncoder = {
    .get = DevEncoder_Get,
    .invert = DevEncoder_Invert,
    .open = DevEncoder_Open,
    .num = DevEncoder_GetNum,
    .set_btn_callback = DevEncoder_Set_BtnCallback,
    .trigger_button = DevEncoder_ButtonTrigger,
};

static uint8_t DevEncoder_GetNum(void)
{
    return encoder_sum;
}

static bool DevEncoder_Open(DevEncoder_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t btn_enable, uint8_t timerx, uint16_t ch_a, uint16_t ch_b)
{
    if ((obj == NULL) || (io == NULL))
    {
        obj->init_state = false;
        return false;
    }

    obj->btn_en = btn_enable;
    obj->invert_reg = Encoder_None_Invert;

    if (btn_enable)
    {
        DrvGPIO.open(&io[Encoder_IO_Btn], GPIO_EXTI_Input, (uint32_t)obj->Btn_Callback);
    }

    DrvGPIO.open(&io[Encoder_IO_A], GPIO_Encoder, NULL);
    DrvGPIO.open(&io[Encoder_IO_B], GPIO_Encoder, NULL);

    DrvTimer.obj_clear(&obj->TimerObj);

    obj->TimerObj.timerx = timerx;
    obj->TimerObj.enc_ch_a = ch_a;
    obj->TimerObj.enc_ch_b = ch_b;

    if (DrvTimer.ctl(DrvTimer_Encoder_Mode, (uint32_t)&obj->TimerObj, sizeof(obj->TimerObj)))
    {
        obj->init_state = true;
        encoder_sum++;
    }
    else
    {
        obj->init_state = false;
    }

    return obj->init_state;
}

static bool DevEncoder_Set_BtnCallback(DevEncoder_Obj_TypeDef *obj, Encoder_Btn_Callback callback)
{
    if (obj != NULL)
    {
        obj->Btn_Callback = callback;
        return true;
    }

    return false;
}

static bool DevEncoder_ButtonTrigger(DevEncoder_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *btn_io)
{
    if (DrvGPIO.get(btn_io) == GPIO_LOW)
    {
    }
    else if (DrvGPIO.get(btn_io) == GPIO_HIGH)
    {
    }
}

static bool DevEncoder_Invert(DevEncoder_Obj_TypeDef *obj, uint8_t invert_val)
{
    if ((obj == NULL) || (!obj->init_state) || (invert_val > 0x03))
        return false;

    obj->invert_reg = invert_val;

    return true;
}

static Encoder_Data_TypeDef DevEncoder_Get(DevEncoder_Obj_TypeDef *obj)
{
    Encoder_Data_TypeDef data_tmp;

    data_tmp.state = Encoder_Normal;
    data_tmp.val = 0;

    if ((obj == NULL) || (!obj->init_state))
    {
        data_tmp.state = Encoder_Error;
        return data_tmp;
    }

    data_tmp.val = DrvTimer.get(&obj->TimerObj);

    if (obj->invert_reg & Encoder_Dir_Invert)
    {
        data_tmp.val *= -1;
    }

    return data_tmp;
}
