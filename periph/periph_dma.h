#ifndef __PERIPH_DMA_H
#define __PERIPH_DMA_H

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#define TIM2_DMA_CLK			RCC_AHB1Periph_DMA1
#define TIM3_DMA_CLK			RCC_AHB1Periph_DMA1

#define TIM2_CH1_DMA_Stream DMA1_Stream5
#define TIM2_CH2_DMA_Stream	DMA1_Stream6
#define TIM2_CH3_DMA_Stream DMA1_Stream1
#define TIM2_CH4_DMA_Stream DMA1_Stream6

#define TIM3_CH1_DMA_Stream	DMA1_Stream4
#define TIM3_CH2_DMA_Stream	DMA1_Stream5
#define TIM3_CH3_DMA_Stream DMA1_Stream7
#define TIM3_CH4_DMA_Stream DMA1_Stream2

#define TIM2_CH1_DMA_Channel DMA_Channel_3
#define TIM2_CH2_DMA_Channel DMA_Channel_3
#define TIM2_CH3_DMA_Channel DMA_Channel_3
#define TIM2_CH4_DMA_Channel DMA_Channel_3

#define TIM3_CH1_DMA_Channel DMA_Channel_5
#define TIM3_CH2_DMA_Channel DMA_Channel_5
#define TIM3_CH3_DMA_Channel DMA_Channel_5
#define TIM3_CH4_DMA_Channel DMA_Channel_5

#define TIM2_CH1_TCIF_FLAG DMA_FLAG_TCIF5
#define TIM2_CH2_TCIF_FLAG DMA_FLAG_TCIF6
#define TIM2_CH3_TCIF_FLAG DMA_FLAG_TCIF1
#define TIM2_CH4_TCIF_FLAG DMA_FLAG_TCIF6

#define TIM3_CH1_TCIF_FLAG DMA_FLAG_TCIF4
#define TIM3_CH2_TCIF_FLAG DMA_FLAG_TCIF5
#define TIM3_CH3_TCIF_FLAG DMA_FLAG_TCIF7
#define TIM3_CH4_TCIF_FLAG DMA_FLAG_TCIF2

//#define TIM4_CH1_TCIF_FLAG 
//#define TIM4_CH2_TCIF_FLAG
//#define TIM4_CH3_TCIF_FLAG
//#define TIM4_CH4_TCIF_FLAG

#define Serial1_DMA_CLK		RCC_AHB1Periph_DMA2
#define Serial2_DMA_CLK		RCC_AHB1Periph_DMA1
#define Serial3_DMA_CLK		RCC_AHB1Periph_DMA1
#define Serial6_DMA_CLK		RCC_AHB1Periph_DMA2

#define Serial1_DMA_RX_Stream	DMA2_Stream5
#define Serial2_DMA_RX_Stream	DMA1_Stream5
#define Serial3_DMA_RX_Stream	DMA1_Stream1
#define Serial6_DMA_RX_Stream	DMA2_Stream1

#define Serial1_DMA_RX_Channel	DMA_Channel_4
#define Serial2_DMA_RX_Channel	DMA_Channel_4
#define Serial3_DMA_RX_Channel	DMA_Channel_4
#define Serial6_DMA_RX_Channel	DMA_Channel_5

#define Serial1_DMA_TX_Stream DMA2_Stream7
#define Serial2_DMA_TX_Stream DMA1_Stream6
#define Serial3_DMA_TX_Stream DMA1_Stream3
#define Serial6_DMA_TX_Stream DMA2_Stream7

#define Serial1_DMA_TX_Channel DMA_Channel_4
#define Serial2_DMA_TX_Channel DMA_Channel_4
#define Serial3_DMA_TX_Channel DMA_Channel_4
#define Serial6_DMA_TX_Channel DMA_Channel_5

typedef enum
{
    Serial_DMA_TX = 1,
    Serial_DMA_RX,
}Serial_DMA_Mode;

void periph_DMA_Serial(DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_Channel,uint32_t Periph_ADDR,uint32_t Buff_ADDR,uint32_t Buff_Size,Serial_DMA_Mode mode);
void periph_DMA_TIM(DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_Channel,uint32_t Periph_ADDR,uint32_t Buff_ADDR,uint32_t Buff_Size);

void periph_DMA_WithoutIRQ_Init(uint32_t DMA_CLK,DMA_Stream_TypeDef* DMA_Stream,DMA_InitTypeDef* DMA_InitStructure,FunctionalState State);
void periph_DMA_WithIRQ_Init(uint32_t DMA_CLK,DMA_Stream_TypeDef* DMA_Stream,DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_IT,FunctionalState State);
void periph_DMA_Set_State(DMA_Stream_TypeDef* DMA_Stream,FunctionalState State);

#endif


