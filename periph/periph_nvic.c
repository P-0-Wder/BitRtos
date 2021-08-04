#include "periph_nvic.h"
#include "misc.h"

void periph_nvic_Structure_Setting(uint8_t channel,uint8_t PreemptionPriority,uint8_t SubPriority,FunctionalState state)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;			
	NVIC_Init(&NVIC_InitStructure);
}








