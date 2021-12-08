#include "stm32f4xx_tim.h"
#include "periph_timer.h"
#include "periph_gpio.h"
#include "periph_dma.h"
#include "misc.h"
#include <stdbool.h>

TIM_TypeDef *Timer_Port[Timer_Port_Sum] = {TIM2,
										   TIM3,
										   TIM4,
										   TIM8};

static uint32_t Timer_CLK[Timer_Port_Sum] = {RCC_APB1Periph_TIM2,
											 RCC_APB1Periph_TIM3,
											 RCC_APB1Periph_TIM4,
											 RCC_APB2Periph_TIM8};

static uint8_t Timer_IRQ_Channel[Timer_Port_Sum] = {TIM2_IRQn,
													TIM3_IRQn,
													TIM4_IRQn};

static void (*PWM_Set_Value[PWM_SUM])(TIM_TypeDef *TIMx, uint32_t Compare1) = {TIM_SetCompare1,
																			   TIM_SetCompare2,
																			   TIM_SetCompare3,
																			   TIM_SetCompare4};

static void periph_Timer_IO_Init(Timer_list timerx, Timer_PWM_Channel_State CH1_State, Timer_PWM_Channel_State CH2_State, Timer_PWM_Channel_State CH3_State, Timer_PWM_Channel_State CH4_State)
{
	if (CH1_State == PWM_CH1_Enable)
	{
		switch ((uint8_t)timerx)
		{
		case Timer_2:
			GPIO_TIM2_PWMCH1_IO_Init();
			break;
		case Timer_3:
			GPIO_TIM3_PWMCH1_IO_Init();
			break;
		case Timer_4:
			GPIO_TIM4_PWMCH1_IO_Init();
			break;
		default:
			break;
		}
	}

	if (CH2_State == PWM_CH2_Enable)
	{
		switch ((uint8_t)timerx)
		{
		case Timer_2:
			GPIO_TIM2_PWMCH2_IO_Init();
			break;
		case Timer_3:
			GPIO_TIM3_PWMCH2_IO_Init();
			break;
		case Timer_4:
			GPIO_TIM4_PWMCH2_IO_Init();
			break;
		default:
			break;
		}
	}

	if (CH3_State == PWM_CH3_Enable)
	{
		switch ((uint8_t)timerx)
		{
		case Timer_2:
			GPIO_TIM2_PWMCH3_IO_Init();
			break;
		case Timer_3:
			GPIO_TIM3_PWMCH3_IO_Init();
			break;
		case Timer_4:
			GPIO_TIM4_PWMCH3_IO_Init();
			break;
		default:
			break;
		}
	}

	if (CH4_State == PWM_CH4_Enable)
	{
		switch ((uint8_t)timerx)
		{
		case Timer_2:
			GPIO_TIM2_PWMCH4_IO_Init();
			break;
		case Timer_3:
			GPIO_TIM3_PWMCH4_IO_Init();
			break;
		case Timer_4:
			GPIO_TIM4_PWMCH4_IO_Init();
			break;
		default:
			break;
		}
	}
}

void periph_Timer_CounterMode_Init(Timer_list timerx, uint32_t Period, uint32_t Prescaler, uint8_t PreemptionPriority, uint8_t SubPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(Timer_CLK[timerx], ENABLE);
	TIM_TimeBaseStructure.TIM_Period = Period;		 //1000
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; //83
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(Timer_Port[timerx], &TIM_TimeBaseStructure);

	TIM_ITConfig(Timer_Port[timerx], TIM_IT_Update, ENABLE);
	TIM_Cmd(Timer_Port[timerx], ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = Timer_IRQ_Channel[timerx];
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_Init(&NVIC_InitStructure);
}

/* need modify */
void periph_Timer_Encoder_Mode_Init(Timer_list timerx, uint16_t channel_a, uint16_t channel_b)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(Timer_CLK[timerx], ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(Timer_Port[timerx], &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_Channel = channel_a;
	TIM_ICInit(Timer_Port[timerx], &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = channel_b; 
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(Timer_Port[timerx], &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(Timer_Port[timerx], TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);

	Timer_Port[timerx]->CNT = 0;
	TIM_Cmd(Timer_Port[timerx], ENABLE);
}

int8_t periph_Timer_GetEncoder_Input(Timer_list timerx)
{
	return (int8_t)Timer_Port[timerx]->CNT;
}

void periph_Timer_PWMOutPut_Mode_Init(Timer_list timerx, PWM_Hz hz, Timer_PWM_Channel_State CH1_State, Timer_PWM_Channel_State CH2_State, Timer_PWM_Channel_State CH3_State, Timer_PWM_Channel_State CH4_State)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	periph_Timer_IO_Init(timerx, CH1_State, CH2_State, CH3_State, CH4_State);

	RCC_APB1PeriphClockCmd(Timer_CLK[timerx], ENABLE);
	TIM_DeInit(Timer_Port[timerx]);

	if (hz == PWM_50hz)
	{
		TIM_TimeBaseStructure.TIM_Period = TimerPWM_50Hz_Period;
		TIM_TimeBaseStructure.TIM_Prescaler = TimerPWM_50Hz_Prescaler;
	}
	else if (hz == PWM_38Khz)
	{
		TIM_TimeBaseStructure.TIM_Period = TimerPWM_38KHz_Period;
		TIM_TimeBaseStructure.TIM_Prescaler = TimerPWM_38KHz_Prescaler;
	}

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(Timer_Port[timerx], &TIM_TimeBaseStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	if (CH1_State == PWM_CH1_Enable)
	{
		TIM_OC1Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH2_State == PWM_CH2_Enable)
	{
		TIM_OC2Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH3_State == PWM_CH3_Enable)
	{
		TIM_OC3Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH4_State == PWM_CH4_Enable)
	{
		TIM_OC4Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(Timer_Port[timerx], ENABLE);
	TIM_ClearFlag(Timer_Port[timerx], TIM_FLAG_Update);
	TIM_ITConfig(Timer_Port[timerx], TIM_IT_Update, ENABLE);
	TIM_Cmd(Timer_Port[timerx], ENABLE);
}

void periph_Timer_PWM_SetEnable(Timer_list timerx, uint8_t state)
{
	if (state)
	{
		TIM_Cmd(Timer_Port[timerx], ENABLE);
	}
	else
	{
		TIM_Cmd(Timer_Port[timerx], DISABLE);
	}
}

void periph_Timer_DShotOutPut_Mode_Init(Timer_list timerx, Timer_PWM_Channel_State CH1_State, Timer_PWM_Channel_State CH2_State, Timer_PWM_Channel_State CH3_State, Timer_PWM_Channel_State CH4_State,
										uint32_t Buff_Size, uint32_t CH1_Buff, uint32_t CH2_Buff, uint32_t CH3_Buff, uint32_t CH4_Buff)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	periph_Timer_IO_Init(timerx, CH1_State, CH2_State, CH3_State, CH4_State);

	RCC_APB1PeriphClockCmd(Timer_CLK[timerx], ENABLE);
	TIM_DeInit(Timer_Port[timerx]);

	TIM_TimeBaseStructure.TIM_Period = TIM_DShot600_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_DShot600_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(Timer_Port[timerx], &TIM_TimeBaseStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	if (CH1_State == PWM_CH1_Enable)
	{
		TIM_OC1Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH2_State == PWM_CH2_Enable)
	{
		TIM_OC2Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH3_State == PWM_CH3_Enable)
	{
		TIM_OC3Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	if (CH4_State == PWM_CH4_Enable)
	{
		TIM_OC4Init(Timer_Port[timerx], &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(Timer_Port[timerx], TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(Timer_Port[timerx], ENABLE);
	TIM_Cmd(Timer_Port[timerx], ENABLE);

	if (CH1_State == PWM_CH1_Enable)
	{
		TIM_DMACmd(Timer_Port[timerx], TIM_DMA_CC1, ENABLE);

		if (timerx == Timer_2)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM2_CH1_DMA_Channel, (uint32_t) & (TIM2->CCR1), CH1_Buff, Buff_Size);
			periph_DMA_Init(TIM2_DMA_CLK, TIM2_CH1_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
		else if (timerx == Timer_3)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM3_CH1_DMA_Channel, (uint32_t) & (TIM3->CCR1), CH1_Buff, Buff_Size);
			periph_DMA_Init(TIM3_DMA_CLK, TIM3_CH1_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
	}

	if (CH2_State == PWM_CH2_Enable)
	{
		TIM_DMACmd(Timer_Port[timerx], TIM_DMA_CC2, ENABLE);

		if (timerx == Timer_2)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM2_CH2_DMA_Channel, (uint32_t) & (TIM2->CCR2), CH2_Buff, Buff_Size);
			periph_DMA_Init(TIM2_DMA_CLK, TIM2_CH2_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
		else if (timerx == Timer_3)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM3_CH2_DMA_Channel, (uint32_t) & (TIM3->CCR2), CH2_Buff, Buff_Size);
			periph_DMA_Init(TIM3_DMA_CLK, TIM3_CH2_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
	}

	if (CH3_State == PWM_CH3_Enable)
	{
		TIM_DMACmd(Timer_Port[timerx], TIM_DMA_CC3, ENABLE);

		if (timerx == Timer_2)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM2_CH3_DMA_Channel, (uint32_t) & (TIM2->CCR3), CH3_Buff, Buff_Size);
			periph_DMA_Init(TIM2_DMA_CLK, TIM2_CH3_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
		else if (timerx == Timer_3)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM3_CH2_DMA_Channel, (uint32_t) & (TIM3->CCR3), CH3_Buff, Buff_Size);
			periph_DMA_Init(TIM3_DMA_CLK, TIM3_CH3_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
	}

	if (CH4_State == PWM_CH4_Enable)
	{
		TIM_DMACmd(Timer_Port[timerx], TIM_DMA_CC4, ENABLE);

		if (timerx == Timer_2)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM2_CH4_DMA_Channel, (uint32_t) & (TIM2->CCR4), CH4_Buff, Buff_Size);
			periph_DMA_Init(TIM2_DMA_CLK, TIM2_CH4_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
		else if (timerx == Timer_3)
		{
			periph_DMA_TIM(&DMA_InitStructure, TIM3_CH4_DMA_Channel, (uint32_t) & (TIM3->CCR4), CH4_Buff, Buff_Size);
			periph_DMA_Init(TIM3_DMA_CLK, TIM3_CH4_DMA_Stream, &DMA_InitStructure, DISABLE);
		}
	}
}

void periph_Timer_Set_PWMOutPut(Timer_list timerx, PWM_OutPut_Channel Channel, uint32_t value)
{
	PWM_Set_Value[Channel](Timer_Port[timerx], value);
}

void periph_Timer_Counter_SetEnable(Timer_list timerx, uint8_t state)
{
	if (state)
	{
		TIM_Cmd(Timer_Port[timerx], ENABLE);
	}
	else
	{
		TIM_Cmd(Timer_Port[timerx], DISABLE);
	}
}
