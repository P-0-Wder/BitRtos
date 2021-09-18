#include "periph_serial_irq.h"
#include "periph_serial.h"
#include "periph_gpio.h"
#include "periph_dma.h"
#include "string.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "shell.h"
#include "shell_port.h"

#define PCK_MAX 256
uint8_t shell_recv;
void USART1_IRQHandler(void)
{
	uint8_t rec;
	static u8 rec_len = 0;
	static u8 rec_enable = 0;

	if (USART_GetITStatus(USART1, USART_IT_TXE) == RESET)
	{
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	}

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		rec = USART_ReceiveData(USART1);

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial1_DMA_RX_Stream, DISABLE);

		rec = USART1->DR;
		rec = USART1->SR;

		rec = DMA_GetCurrDataCounter(Serial1_DMA_RX_Stream);
		shellHandler(&shell, shell_recv);
		DMA_ClearFlag(Serial1_DMA_RX_Stream, DMA_FLAG_TCIF5);
		DMA_SetCurrDataCounter(Serial1_DMA_RX_Stream, PCK_MAX);
		DMA_Cmd(Serial1_DMA_RX_Stream, ENABLE);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART2, USART_IT_TXE) == RESET)
	{
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	}

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rec = USART_ReceiveData(USART2);
		USART_SendData(USART2, rec);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial2_DMA_RX_Stream, DISABLE);

		rec = USART2->DR;
		rec = USART2->SR;

		rec = DMA_GetCurrDataCounter(Serial2_DMA_RX_Stream);
		DMA_ClearFlag(Serial2_DMA_RX_Stream, DMA_FLAG_TCIF5);
		DMA_SetCurrDataCounter(Serial2_DMA_RX_Stream, PCK_MAX);
		DMA_Cmd(Serial2_DMA_RX_Stream, ENABLE);
	}
}

void USART3_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART3, USART_IT_TXE) == RESET)
	{
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	}

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rec = USART_ReceiveData(USART3);
		USART_SendData(USART3, rec);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial3_DMA_RX_Stream, DISABLE);

		rec = USART3->DR;
		rec = USART3->SR;

		rec = DMA_GetCurrDataCounter(Serial3_DMA_RX_Stream);
		DMA_ClearFlag(Serial3_DMA_RX_Stream, DMA_FLAG_TCIF1);
		DMA_SetCurrDataCounter(Serial3_DMA_RX_Stream, PCK_MAX);
		DMA_Cmd(Serial3_DMA_RX_Stream, ENABLE);
	}
}

void USART6_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART6, USART_IT_TXE) == RESET)
	{
		USART_ITConfig(USART6, USART_IT_TC, DISABLE);
	}

	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		rec = USART_ReceiveData(USART6);
		USART_SendData(USART6, rec);
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial6_DMA_RX_Stream, DISABLE);

		rec = USART6->DR;
		rec = USART6->SR;

		rec = DMA_GetCurrDataCounter(Serial6_DMA_RX_Stream);
		DMA_ClearFlag(Serial6_DMA_RX_Stream, DMA_FLAG_TCIF1);
		DMA_SetCurrDataCounter(Serial6_DMA_RX_Stream, PCK_MAX);
		DMA_Cmd(Serial6_DMA_RX_Stream, ENABLE);
	}
}
