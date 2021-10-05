#ifndef __PERIPH_TIMER_H
#define __PERIPH_TIMER_H

#include "stm32f4xx.h"

#define Timer_Port_Sum 3

#define TimerCounter_1us_Period 1
#define TimerCounter_1ms_Period 1000
#define TimerCounter_2ms_Period 2000
#define TimerCounter_3ms_Period 3000

#define TimerCounter_1M_Prescaler 83

#define TimerPWM_50Hz_Period 20000
#define TimerPWM_50Hz_Prescaler 83

#define TimerPWM_38KHz_Period 2211
#define TimerPWM_38KHz_Prescaler 0

#define TIM_DShot600_Period 69
#define TIM_DShot600_Prescaler 3

typedef enum
{
	Timer_Encoder,
	Timer_Counter,
	Timer_PWM_Output,
	Timer_DShot_Output,
} Timer_Func_Type;

typedef enum
{
	PWM_50hz,
	PWM_38Khz,
} PWM_Hz;

typedef enum
{
	PWM_CH1_Disable = 0,
	PWM_CH1_Enable = 1,
	PWM_CH2_Disable = 0,
	PWM_CH2_Enable = 1,
	PWM_CH3_Disable = 0,
	PWM_CH3_Enable = 1,
	PWM_CH4_Disable = 0,
	PWM_CH4_Enable = 1,
} Timer_PWM_Channel_State;

typedef enum
{
	Timer_2 = 0,
	Timer_3,
	Timer_4,
	Timer_None,
} Timer_list;

typedef enum
{
	PWM_CH_1,
	PWM_CH_2,
	PWM_CH_3,
	PWM_CH_4,

	PWM_SUM,
} PWM_OutPut_Channel;

void periph_Timer_Encoder_Mode_Init(Timer_list timerx);
int8_t periph_Timer_GetEncoder_Input(Timer_list timerx);

void periph_Timer_CounterMode_Init(Timer_list timerx, uint32_t Period, uint32_t Prescaler, uint8_t PreemptionPriority, uint8_t SubPriority);
void periph_Timer_PWMOutPut_Mode_Init(Timer_list timerx, PWM_Hz hz, Timer_PWM_Channel_State CH1_State, Timer_PWM_Channel_State CH2_State, Timer_PWM_Channel_State CH3_State, Timer_PWM_Channel_State CH4_State);
void periph_Timer_DShotOutPut_Mode_Init(Timer_list timerx, Timer_PWM_Channel_State CH1_State, Timer_PWM_Channel_State CH2_State, Timer_PWM_Channel_State CH3_State, Timer_PWM_Channel_State CH4_State,
										uint32_t Buff_Size, uint32_t CH1_Buff, uint32_t CH2_Buff, uint32_t CH3_Buff, uint32_t CH4_Buff);
void periph_Timer_PWM_SetEnable(Timer_list timerx, uint8_t state);
void periph_Timer_Set_PWMOutPut(Timer_list timerx, PWM_OutPut_Channel Channel, uint32_t value);

extern TIM_TypeDef *Timer_Port[Timer_Port_Sum];
#endif
