#ifndef __PERIPH_NVIC_H
#define __PERIPH_NVIC_H

#include "stm32f4xx.h"

#define NVIC_PreemptionPriority_Level_0			0
#define NVIC_PreemptionPriority_Level_1			1
#define NVIC_PreemptionPriority_Level_2			2
#define NVIC_PreemptionPriority_Level_3			3

#define NVIC_SubPriority_Level_0						0
#define NVIC_SubPriority_Level_1						1
#define NVIC_SubPriority_Level_2						2
#define NVIC_SubPriority_Level_3						3

void periph_nvic_Structure_Setting(uint8_t channel,uint8_t PreemptionPriority,uint8_t SubPriority,FunctionalState state);

#endif

