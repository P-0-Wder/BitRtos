#include "periph_exti_irq.h"
#include "periph_exti.h"
#include "stm32f4xx_exti.h"
#include "periph_gpio.h"

void EXTI0_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line0))
    {
        EXTI_ClearFlag(EXTI_Line0);

        if (periph_exti_GetCallback(EXTI_Line0) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line0)();
        }

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line1))
    {
        EXTI_ClearFlag(EXTI_Line1);

        if (periph_exti_GetCallback(EXTI_Line1) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line1)();
        }

        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line2))
    {
        EXTI_ClearFlag(EXTI_Line2);

        if (periph_exti_GetCallback(EXTI_Line2) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line2)();
        }

        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line3))
    {
        EXTI_ClearFlag(EXTI_Line3);

        if (periph_exti_GetCallback(EXTI_Line3) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line3)();
        }

        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line4))
    {
        EXTI_ClearFlag(EXTI_Line4);

        if (periph_exti_GetCallback(EXTI_Line4) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line4)();
        }

        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line5))
    {
        EXTI_ClearFlag(EXTI_Line5);

        if (periph_exti_GetCallback(EXTI_Line5) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line5)();
        }

        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line6))
    {
        EXTI_ClearFlag(EXTI_Line6);

        if (periph_exti_GetCallback(EXTI_Line6) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line6)();
        }

        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line7))
    {
        EXTI_ClearFlag(EXTI_Line7);

        if (periph_exti_GetCallback(EXTI_Line7) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line7)();
        }

        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line8))
    {
        EXTI_ClearFlag(EXTI_Line8);

        if (periph_exti_GetCallback(EXTI_Line8) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line8)();
        }

        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line9))
    {
        EXTI_ClearFlag(EXTI_Line9);

        if (periph_exti_GetCallback(EXTI_Line9) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line9)();
        }

        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (SET == EXTI_GetITStatus(EXTI_Line10))
    {
        EXTI_ClearFlag(EXTI_Line10);

        if (periph_exti_GetCallback(EXTI_Line10) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line10)();
        }

        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line11))
    {
        EXTI_ClearFlag(EXTI_Line11);

        if (periph_exti_GetCallback(EXTI_Line11) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line11)();
        }

        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line12))
    {
        EXTI_ClearFlag(EXTI_Line12);

        if (periph_exti_GetCallback(EXTI_Line12) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line12)();
        }

        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line13))
    {
        EXTI_ClearFlag(EXTI_Line13);

        if (periph_exti_GetCallback(EXTI_Line13) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line13)();
        }

        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line14))
    {
        EXTI_ClearFlag(EXTI_Line14);

        if (periph_exti_GetCallback(EXTI_Line14) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line14)();
        }

        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    else if (SET == EXTI_GetITStatus(EXTI_Line15))
    {
        EXTI_ClearFlag(EXTI_Line15);

        if (periph_exti_GetCallback(EXTI_Line15) != NULL)
        {
            periph_exti_GetCallback(EXTI_Line15)();
        }

        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}