#include "periph_dma.h"
#include "stm32f4xx_dma.h"
#include "periph_gpio.h"

void periph_DMA_Serial(DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_Channel,uint32_t Periph_ADDR,uint32_t Buff_ADDR,uint32_t Buff_Size,Serial_DMA_Mode mode)
{
	DMA_InitStructure->DMA_Channel = DMA_Channel;
	DMA_InitStructure->DMA_PeripheralBaseAddr = Periph_ADDR;
	DMA_InitStructure->DMA_Memory0BaseAddr = Buff_ADDR;

	if(mode == Serial_DMA_RX)
	{
		DMA_InitStructure->DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure->DMA_Mode = DMA_Mode_Circular;
	}
	else if(mode == Serial_DMA_TX)
	{
		DMA_InitStructure->DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure->DMA_Mode = DMA_Mode_Normal;
	}
	
	DMA_InitStructure->DMA_BufferSize = Buff_Size;
	DMA_InitStructure->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure->DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  

	DMA_InitStructure->DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure->DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure->DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure->DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure->DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}

void periph_DMA_TIM(DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_Channel,uint32_t Periph_ADDR,uint32_t Buff_ADDR,uint32_t Buff_Size)
{
  DMA_InitStructure->DMA_Channel = DMA_Channel;  
  DMA_InitStructure->DMA_PeripheralBaseAddr = Periph_ADDR; 
  DMA_InitStructure->DMA_Memory0BaseAddr = Buff_ADDR; 
  DMA_InitStructure->DMA_DIR = DMA_DIR_MemoryToPeripheral; 
  DMA_InitStructure->DMA_BufferSize = Buff_Size; 
  DMA_InitStructure->DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  DMA_InitStructure->DMA_MemoryInc = DMA_MemoryInc_Enable; 
  DMA_InitStructure->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
  DMA_InitStructure->DMA_MemoryDataSize = DMA_PeripheralDataSize_Word; 
  DMA_InitStructure->DMA_Mode = DMA_Mode_Normal; 
  DMA_InitStructure->DMA_Priority = DMA_Priority_Medium; 
  DMA_InitStructure->DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure->DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure->DMA_MemoryBurst = DMA_MemoryBurst_Single; 
  DMA_InitStructure->DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
}

void periph_DMA_WithoutIRQ_Init(uint32_t DMA_CLK,DMA_Stream_TypeDef* DMA_Stream,DMA_InitTypeDef* DMA_InitStructure,FunctionalState State)
{
	RCC_AHB1PeriphClockCmd(DMA_CLK, ENABLE);
	DMA_DeInit(DMA_Stream);
	while(DMA_GetCmdStatus(DMA_Stream) != DISABLE);	
	DMA_Init(DMA_Stream, DMA_InitStructure);  
	DMA_Cmd(DMA_Stream, State);
}

void periph_DMA_WithIRQ_Init(uint32_t DMA_CLK,DMA_Stream_TypeDef* DMA_Stream,DMA_InitTypeDef* DMA_InitStructure,uint32_t DMA_IT,FunctionalState State)
{	
	RCC_AHB1PeriphClockCmd(DMA_CLK, ENABLE);
	DMA_DeInit(DMA_Stream);
	while(DMA_GetCmdStatus(DMA_Stream) != DISABLE);	
	DMA_Init(DMA_Stream, DMA_InitStructure);  
	DMA_ITConfig(DMA_Stream, DMA_IT, ENABLE);
	DMA_Cmd(DMA_Stream, State);
}

void periph_DMA_Set_State(DMA_Stream_TypeDef* DMA_Stream,FunctionalState State)
{
	DMA_Cmd(DMA_Stream, State);
}



