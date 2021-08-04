#include "periph_timer_irq.h"
#include "periph_serial.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
#include "task.h"

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		Task_CountRunningTime();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
