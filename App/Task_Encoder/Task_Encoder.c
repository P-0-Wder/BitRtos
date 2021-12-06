#include "Task_Encoder.h"
#include "Dev_Encoder.h"

static bool Task_Encoder_Init(void)
{
    GPIO_IO_Encoder_Init(Encoder_Port,Encoder_IO_CLK,GPIO_Pin_9,Encoder_IO_A_PinSource,GPIO_AF_TIM1);
    GPIO_IO_Encoder_Init(Encoder_Port,Encoder_IO_CLK,GPIO_Pin_11,Encoder_IO_B_PinSource,GPIO_AF_TIM1);
    periph_Timer_Encoder_Mode_Init(TIM1,TIM_Channel_1,TIM_Channel_2);
}