#include "periph_serial.h"
#include "periph_gpio.h"
#include "periph_nvic.h"
#include "periph_dma.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"

Serial_IRQ_Callback IRQ_Callback[Serial_Port_Sum] = {NULL};

#ifdef STM32F40XX
static void (*Serial_IO_Init[Serial_Port_Sum])(void) = {GPIO_USART1_IO_Init,
														GPIO_USART2_IO_Init,
														GPIO_USART3_IO_Init,
														GPIO_USART6_IO_Init};

static void (*RCC_APBPeriphClockCmd[Serial_Port_Sum])(uint32_t RCC_APB2Periph, FunctionalState NewState) = {RCC_APB2PeriphClockCmd,
																											RCC_APB1PeriphClockCmd,
																											RCC_APB1PeriphClockCmd,
																											RCC_APB2PeriphClockCmd};

static uint32_t RCC_APBPeriph_SerialCLK[Serial_Port_Sum] = {RCC_APB2Periph_USART1,
															RCC_APB1Periph_USART2,
															RCC_APB1Periph_USART3,
															RCC_APB2Periph_USART6};

USART_TypeDef *Serial_Port[Serial_Port_Sum] = {USART1,
											   USART2,
											   USART3,
											   USART6};

static uint8_t Serial_RX_IRQ_Channel[Serial_Port_Sum] = {USART1_IRQn,
														 USART2_IRQn,
														 USART3_IRQn,
														 USART6_IRQn};

static uint8_t Serial_DMA_TX_IRQ_Channel[Serial_Port_Sum] = {DMA2_Stream7_IRQn,
															 DMA1_Stream6_IRQn,
															 DMA1_Stream3_IRQn,
															 DMA2_Stream7_IRQn};

static uint32_t Serial_DMA_CLK[Serial_Port_Sum] = {Serial1_DMA_CLK,
												   Serial2_DMA_CLK,
												   Serial3_DMA_CLK,
												   Serial6_DMA_CLK};

static uint32_t Serial_DMA_RX_Channel[Serial_Port_Sum] = {Serial1_DMA_RX_Channel,
														  Serial2_DMA_RX_Channel,
														  Serial3_DMA_RX_Channel,
														  Serial6_DMA_RX_Channel};

static DMA_Stream_TypeDef *Serial_DMA_RX_Stream[Serial_Port_Sum] = {Serial1_DMA_RX_Stream,
																	Serial2_DMA_RX_Stream,
																	Serial3_DMA_RX_Stream,
																	Serial6_DMA_RX_Stream};

static uint32_t Serial_DMA_TX_Channel[Serial_Port_Sum] = {Serial1_DMA_TX_Channel,
														  Serial2_DMA_TX_Channel,
														  Serial3_DMA_TX_Channel,
														  Serial6_DMA_TX_Channel};

static DMA_Stream_TypeDef *Serial_DMA_TX_Stream[Serial_Port_Sum] = {Serial1_DMA_TX_Stream,
																	Serial2_DMA_TX_Stream,
																	Serial3_DMA_TX_Stream,
																	Serial6_DMA_TX_Stream};

static uint32_t DMA_TX_FinishFlag[Serial_Port_Sum] = {DMA_FLAG_TCIF7,
													  DMA_FLAG_TCIF6,
													  DMA_FLAG_TCIF3,
													  DMA_FLAG_TCIF7};
#endif

static void USART_Structure_Normal_Setting(USART_InitTypeDef *USART_InitStructure, uint32_t bound)
{
	USART_InitStructure->USART_BaudRate = bound;
	USART_InitStructure->USART_WordLength = USART_WordLength_8b;
	USART_InitStructure->USART_StopBits = USART_StopBits_1;
	USART_InitStructure->USART_Parity = USART_Parity_No;
	USART_InitStructure->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
}

static void USART_Structure_SBus_Setting(USART_InitTypeDef *USART_InitStructure, uint32_t bound)
{
	USART_InitStructure->USART_BaudRate = bound;
	USART_InitStructure->USART_WordLength = USART_WordLength_9b;
	USART_InitStructure->USART_StopBits = USART_StopBits_2;
	USART_InitStructure->USART_Parity = USART_Parity_Even;
	USART_InitStructure->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
}

static void (*Serial_Structure_Setting[Serial_Func_Sum])(USART_InitTypeDef *USART_InitStructure, uint32_t bound) = {USART_Structure_Normal_Setting,
																													USART_Structure_SBus_Setting};

Serial_IRQ_Callback Serial_Get_IRQ_Callback(Serial_List serial_id)
{
	if (serial_id >= Serial_Port_Sum)
		return NULL;

	return IRQ_Callback[serial_id];
}

bool Serial_Set_DMAIRQ_Callback(Serial_List serial_id, dma_irq_callback callback)
{
	uint8_t dma_id = 0;
	uint8_t stream_id = 0;

	if (serial_id >= Serial_Port_Sum)
		return false;

	switch (serial_id)
	{
	case Serial_1:
		dma_id = DMA_2;
		stream_id = DMA_Stream_7;
		break;

	case Serial_2:
		dma_id = DMA_1;
		stream_id = DMA_Stream_6;
		break;

	case Serial_3:
		dma_id = DMA_1;
		stream_id = DMA_Stream_3;
		break;

	case Serial_6:
		dma_id = DMA_2;
		stream_id = DMA_Stream_7;
		break;

	default:
		return;
	}

	periph_Set_DMA_IRQCallback(dma_id, stream_id, callback);

	return true;
}

bool Serial_Set_IRQ_Callback(Serial_List serial_id, Serial_IRQ_Callback callback)
{
	if (serial_id >= Serial_Port_Sum)
		return false;

	IRQ_Callback[serial_id] = callback;

	return true;
}

void Serial_IRQ_RX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, Serial_Func_Type type)
{
	USART_InitTypeDef USART_InitStructure;
	Serial_IO_Init[Serial]();

	RCC_APBPeriphClockCmd[Serial](RCC_APBPeriph_SerialCLK[Serial], ENABLE);

	Serial_Structure_Setting[type](&USART_InitStructure, bound);
	USART_Init(Serial_Port[Serial], &USART_InitStructure);
	USART_Cmd(Serial_Port[Serial], ENABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_RXNE, ENABLE);

	periph_nvic_Structure_Setting(Serial_RX_IRQ_Channel[Serial], PreemptionPriority, SubPriority, ENABLE);
}

void Serial_DMA_RX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t RX_Buff, uint16_t Buff_Size, Serial_Func_Type type)
{
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	Serial_IO_Init[Serial]();

	RCC_APBPeriphClockCmd[Serial](RCC_APBPeriph_SerialCLK[Serial], ENABLE);
	Serial_Structure_Setting[type](&USART_InitStructure, bound);
	USART_Init(Serial_Port[Serial], &USART_InitStructure);

	USART_ITConfig(Serial_Port[Serial], USART_IT_RXNE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TC, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TXE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_IDLE, ENABLE);

	USART_DMACmd(Serial_Port[Serial], USART_DMAReq_Rx, ENABLE);
	periph_DMA_Serial(&DMA_InitStructure, Serial_DMA_RX_Channel[Serial], (uint32_t)&Serial_Port[Serial]->DR, (uint32_t)RX_Buff, Buff_Size, Serial_DMA_RX);
	periph_DMA_WithoutIRQ_Init(Serial_DMA_CLK[Serial], Serial_DMA_RX_Stream[Serial], &DMA_InitStructure, ENABLE);

	periph_nvic_Structure_Setting(Serial_RX_IRQ_Channel[Serial], PreemptionPriority, SubPriority, ENABLE);

	USART_Cmd(Serial_Port[Serial], ENABLE);
}

void Serial_DMA_TX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t TX_Buff, uint16_t Buff_Size, Serial_Func_Type type)
{
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_TX_InitStructure;

	Serial_IO_Init[Serial]();

	RCC_APBPeriphClockCmd[Serial](RCC_APBPeriph_SerialCLK[Serial], ENABLE);
	Serial_Structure_Setting[type](&USART_InitStructure, bound);
	USART_Init(Serial_Port[Serial], &USART_InitStructure);

	USART_DMACmd(Serial_Port[Serial], USART_DMAReq_Tx, ENABLE);

	periph_DMA_Serial(&DMA_TX_InitStructure, Serial_DMA_TX_Channel[Serial], (uint32_t)&Serial_Port[Serial]->DR, (uint32_t)TX_Buff, Buff_Size, Serial_DMA_TX);
	periph_DMA_WithIRQ_Init(Serial_DMA_CLK[Serial], Serial_DMA_TX_Stream[Serial], &DMA_TX_InitStructure, DMA_IT_TC, DISABLE);
	periph_nvic_Structure_Setting(Serial_DMA_TX_IRQ_Channel[Serial], PreemptionPriority, SubPriority + 1, ENABLE);

	USART_Cmd(Serial_Port[Serial], ENABLE);
}

bool Serial_Deinit(Serial_List Serial)
{
	if (Serial >= Serial_Port_Sum)
		return false;

	IRQ_Callback[Serial] = NULL;

	USART_DeInit(Serial_Port[Serial]);

	USART_ITConfig(Serial_Port[Serial], USART_IT_RXNE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TC, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TXE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_IDLE, DISABLE);

	USART_Cmd(Serial_Port[Serial], DISABLE);

	return true;
}

void Serial_DMA_RXTX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t RX_Buff, uint32_t TX_Buff, uint16_t Buff_Size, Serial_Func_Type type)
{
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_RX_InitStructure;
	DMA_InitTypeDef DMA_TX_InitStructure;

	Serial_IO_Init[Serial]();

	RCC_APBPeriphClockCmd[Serial](RCC_APBPeriph_SerialCLK[Serial], ENABLE);
	Serial_Structure_Setting[type](&USART_InitStructure, bound);
	USART_Init(Serial_Port[Serial], &USART_InitStructure);

	USART_ITConfig(Serial_Port[Serial], USART_IT_RXNE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TC, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_TXE, DISABLE);
	USART_ITConfig(Serial_Port[Serial], USART_IT_IDLE, ENABLE);

	USART_DMACmd(Serial_Port[Serial], USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

	//Rx DMA Setting
	periph_DMA_Serial(&DMA_RX_InitStructure, Serial_DMA_RX_Channel[Serial], (uint32_t)&Serial_Port[Serial]->DR, (uint32_t)RX_Buff, Buff_Size, Serial_DMA_RX);
	periph_DMA_WithoutIRQ_Init(Serial_DMA_CLK[Serial], Serial_DMA_RX_Stream[Serial], &DMA_RX_InitStructure, ENABLE);
	periph_nvic_Structure_Setting(Serial_RX_IRQ_Channel[Serial], PreemptionPriority, SubPriority, ENABLE);

	//Tx DMA Setting
	periph_DMA_Serial(&DMA_TX_InitStructure, Serial_DMA_TX_Channel[Serial], (uint32_t)&Serial_Port[Serial]->DR, (uint32_t)TX_Buff, Buff_Size, Serial_DMA_TX);
	periph_DMA_WithIRQ_Init(Serial_DMA_CLK[Serial], Serial_DMA_TX_Stream[Serial], &DMA_TX_InitStructure, DMA_IT_TC, DISABLE);
	periph_nvic_Structure_Setting(Serial_DMA_TX_IRQ_Channel[Serial], PreemptionPriority, SubPriority + 1, ENABLE);

	USART_Cmd(Serial_Port[Serial], ENABLE);
}

void Serial_SendStr(USART_TypeDef *Serial_port, const char *Str_Output)
{
	while (*Str_Output != '\0')
	{
		while (USART_GetFlagStatus(Serial_port, USART_FLAG_TC) == RESET)
			;
		USART_SendData(Serial_port, *Str_Output);
		Str_Output++;
	}
}

void Serial_SendBuff(USART_TypeDef *Serial_port, char *Buff, uint16_t Len)
{
	for (uint8_t Buff_index = 0; Buff_index < Len; Buff_index++)
	{
		while (USART_GetFlagStatus(Serial_port, USART_FLAG_TC) == RESET)
			;
		USART_SendData(Serial_port, Buff[Buff_index]);
	}
}

void Serial_DMA_TX_IRQSetting(Serial_List serial_id)
{
	if (DMA_GetFlagStatus(Serial_DMA_TX_Stream[serial_id], DMA_TX_FinishFlag[serial_id]) == SET)
	{
		DMA_ClearFlag(Serial_DMA_TX_Stream[serial_id], DMA_TX_FinishFlag[serial_id]);
		DMA_Cmd(Serial_DMA_TX_Stream[serial_id], DISABLE);

		USART_ITConfig(Serial_Port[serial_id], USART_IT_TC, ENABLE);
	}
}

void Serial_DMA_SendBuff(Serial_List serial_id, uint16_t len)
{
	DMA_Cmd(Serial_DMA_TX_Stream[serial_id], DISABLE);
	DMA_SetCurrDataCounter(Serial_DMA_TX_Stream[serial_id], (uint16_t)len);
	DMA_Cmd(Serial_DMA_TX_Stream[serial_id], ENABLE);
	USART_DMACmd(Serial_Port[serial_id], USART_DMAReq_Tx, ENABLE);
}
