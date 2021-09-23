#include "periph_exti_irq.h"
#include "stm32f4xx_exti.h"
#include "periph_gpio.h"

void EXTI0_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line0))
    {
        EXTI_ClearFlag(EXTI_Line0);

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line1))
    {
        EXTI_ClearFlag(EXTI_Line1);

        EXTI_ClearITPendingBit(EXTI_Line1);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line2))
    {
        EXTI_ClearFlag(EXTI_Line2);

        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line3))
    {
        EXTI_ClearFlag(EXTI_Line3);

        EXTI_ClearITPendingBit(EXTI_Line3);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line4))
    {
        EXTI_ClearFlag(EXTI_Line4);

        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line5))
    {
        EXTI_ClearFlag(EXTI_Line5);

        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line6))
    {
        EXTI_ClearFlag(EXTI_Line6);

        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line7))
    {
        EXTI_ClearFlag(EXTI_Line7);

        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line8))
    {
        EXTI_ClearFlag(EXTI_Line8);

        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line9))
    {
        EXTI_ClearFlag(EXTI_Line9);

        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line10))
    {
        EXTI_ClearFlag(EXTI_Line10);

        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line11))
    {
        EXTI_ClearFlag(EXTI_Line11);

        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line12))
    {
        EXTI_ClearFlag(EXTI_Line12);

        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line13))
    {
        EXTI_ClearFlag(EXTI_Line13);

        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line14))
    {
        EXTI_ClearFlag(EXTI_Line14);

        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

void EXTI1_IRQHandler(void)
{
}

void EXTI2_IRQHandler(void)
{
}

void EXTI3_IRQHandler(void)
{
}

void EXTI4_IRQHandler(void)
{
}
