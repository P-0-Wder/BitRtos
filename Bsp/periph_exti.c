#include "periph_exti.h"
#include "periph_nvic.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include <string.h>

#if defined STM32F40_41xxx
#define GEN_EXTI_LINE_SUM 16
#endif

static exti_callback CallBack_List[GEN_EXTI_LINE_SUM] = {NULL};

void periph_exit_init(Periph_Exti_Config_TypeDef exti_cfg)
{
    EXTI_InitTypeDef EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(exti_cfg.exti_port, exti_cfg.exti_pinsource);

    EXTI_InitStruct.EXTI_Line = exti_cfg.exti_line;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;

    EXTI_Init(&EXTI_InitStruct);

    periph_nvic_Structure_Setting(exti_cfg.nvic_channel, exti_cfg.pre_priority, exti_cfg.sub_priority, ENABLE);
}

static uint8_t Get_EXTI_Index(uint32_t EXIT_LineX)
{
    float f_exti_id = (float)EXIT_LineX;
    uint32_t u32_exti_id = 0;
    uint8_t exp = 0;

    u32_exti_id = *((unsigned long *)(&f_exti_id));

    exp = (u32_exti_id >> 23) & 0xFF;
    exp -= 127;

    return exp;
}

static bool periph_exti_checkExti_Line(uint32_t EXTI_LineX)
{
    switch (EXTI_LineX)
    {
    case EXTI_Line0:
    case EXTI_Line1:
    case EXTI_Line2:
    case EXTI_Line3:
    case EXTI_Line4:
    case EXTI_Line5:
    case EXTI_Line6:
    case EXTI_Line7:
    case EXTI_Line8:
    case EXTI_Line9:
    case EXTI_Line10:
    case EXTI_Line11:
    case EXTI_Line12:
    case EXTI_Line13:
    case EXTI_Line14:
    case EXTI_Line15:
    case EXTI_Line16:
    case EXTI_Line17:
    case EXTI_Line18:
    case EXTI_Line19:
    case EXTI_Line20:
    case EXTI_Line21:
    case EXTI_Line22:
    case EXTI_Line23:
        return true;

    default:
        return false;
    }
}

bool periph_exti_SetCallback(uint32_t EXTI_LineX, exti_callback callback)
{
    if (!periph_exti_checkExti_Line(EXTI_LineX))
        return false;

    CallBack_List[Get_EXTI_Index(EXTI_LineX)] = callback;
    return true;
}

exti_callback periph_exti_GetCallback(uint32_t EXTI_LineX)
{
    if (!periph_exti_checkExti_Line(EXTI_LineX))
        return NULL;

    return CallBack_List[Get_EXTI_Index(EXTI_LineX)];
}