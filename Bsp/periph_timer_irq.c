#include "periph_timer_irq.h"
#include "periph_timer.h"
#include "stm32f4xx_tim.h"
#include "delay.h"

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (periph_Timer_GetCountIRQ_Callback(Timer_2) != NULL)
			periph_Timer_GetCountIRQ_Callback(Timer_2)(NULL, 0);
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		if (periph_Timer_GetCountIRQ_Callback(Timer_3) != NULL)
			periph_Timer_GetCountIRQ_Callback(Timer_3)(NULL, 0);
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if (periph_Timer_GetCountIRQ_Callback(Timer_4) != NULL)
			periph_Timer_GetCountIRQ_Callback(Timer_4)(1, 0);
	}
}
