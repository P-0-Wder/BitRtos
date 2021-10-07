#ifndef __DRV_TIMER_H
#define __DRV_TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "periph_timer.h"

typedef void (*Timer_IRQ_Callback)(uint8_t *p_data, uint16_t len);

typedef enum
{
    DrvTimer_Counter_Mode = 0,
    DrvTimer_PWM_Mode,
    DrvTimer_DMA_Mode,
    DrvTimer_Encoder_Mode,
} DrvTimer_CMD_List;

typedef struct
{
    DrvTimer_CMD_List mode;
    Timer_list timerx;

    uint32_t period;
    uint32_t Prescaler;
    bool irq;
    uint8_t PreemptionPriority;
    uint8_t SubPriority;

    uint16_t enc_ch_a;
    uint16_t enc_ch_b;

    uint32_t cnt;
} DrvTimer_Obj_TypeDef;

typedef struct
{
    bool (*obj_clear)(DrvTimer_Obj_TypeDef *obj);
    bool (*ctl)(DrvTimer_CMD_List cmd, uint32_t p_data, uint16_t len);
    int32_t (*get)(DrvTimer_Obj_TypeDef *obj);
} DrvTimer_TypeDef;

extern DrvTimer_TypeDef DrvTimer;

#endif
