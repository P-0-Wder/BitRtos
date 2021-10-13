#include "periph_serial.h"
#include "periph_gpio.h"
#include "periph_nvic.h"
#include "periph_dma.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"

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

#if (MATEKF411)
static void (*Seril_IO_Init[Serial_Port_Sum])(void) = {GPIO_USART1_IO_Init,
													   GPIO_USART2_IO_Init,
													   GPIO_USART6_IO_Init};

static void (*RCC_APBPeriphClockCmd[Serial_Port_Sum])(uint32_t RCC_APB2Periph, FunctionalState NewState) = {RCC_APB2PeriphClockCmd,
																											RCC_APB1PeriphClockCmd,
																											RCC_APB2PeriphClockCmd};

static uint32_t RCC_APBPeriph_SerialCLK[Serial_Port_Sum] = {RCC_APB2Periph_USART1,
															RCC_APB1Periph_USART2,
															RCC_APB2Periph_USART6};

static USART_TypeDef *Serial_Port[Serial_Port_Sum] = {USART1,
													  USART2,
													  USART6};

static uint8_t Serial_IRQ_Channel[Serial_Port_Sum] = {USART1_IRQn,
													  USART2_IRQn,
													  USART6_IRQn};

static uint32_t Serial_DMA_CLK[Serial_Port_Sum] = {Serial1_DMA_CLK,
												   Serial2_DMA_CLK,
												   Serial6_DMA_CLK};

static uint32_t Serial_DMA_Channel[Serial_Port_Sum] = {Serial1_DMA_Channel,
													   Serial2_DMA_Channel,
													   Serial6_DMA_Channel};

static DMA_Stream_TypeDef *Serial_DMA_Stream[Serial_Port_Sum] = {Serial1_DMA_Stream,
																 Serial2_DMA_Stream,
																 Serial6_DMA_Stream};
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
	while (DMA_GetCmdStatus(Serial_DMA_TX_Stream[serial_id]) != DISABLE)
	{
	} //确保DMA可以被设置
	DMA_SetCurrDataCounter(Serial_DMA_TX_Stream[serial_id], (uint16_t)len);
	DMA_Cmd(Serial_DMA_TX_Stream[serial_id], ENABLE);

	while (DMA_GetFlagStatus(Serial_DMA_TX_Stream[serial_id], DMA_TX_FinishFlag[serial_id]) != RESET)
		;
}

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{

	DMA_InitTypeDef DMA_InitStructure;

	if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
	}
	else
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA1时钟使能
	}
	DMA_DeInit(DMA_Streamx);

	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
	} //等待DMA可配置

	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;									//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;							//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;							//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = ndtr;								//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// 使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
	DMA_Init(DMA_Streamx, &DMA_InitStructure);							//初始化DMA Stream
}
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//ndtr:数据传输量
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr)
{

	DMA_Cmd(DMA_Streamx, DISABLE); //关闭DMA传输

	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
	} //确保DMA可以被设置

	DMA_SetCurrDataCounter(DMA_Streamx, ndtr); //数据传输量

	DMA_Cmd(DMA_Streamx, ENABLE); //开启DMA传输
}
