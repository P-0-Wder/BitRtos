#include "periph_serial_irq.h"
#include "periph_serial.h"
#include "periph_gpio.h"
#include "periph_dma.h"
#include "string.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"

extern uint8_t Serial_TX_Buff[Serial_Port_Sum][SERIAL_MAX_RECLEN];
extern uint8_t Serial_RX_Buff[Serial_Port_Sum][SERIAL_MAX_RECLEN];

void USART1_IRQHandler(void)
{
	uint8_t rec;
	static u8 rec_len = 0;
	static u8 rec_enable = 0;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if (Serial_Get_IRQ_RxCallback(Serial_1) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_1)((uint8_t)USART_ReceiveData(USART1), 1);
		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial1_DMA_RX_Stream, DISABLE);

		rec = USART1->DR;
		rec = USART1->SR;

		rec = SERIAL_MAX_RECLEN - DMA_GetCurrDataCounter(Serial1_DMA_RX_Stream);

		if (Serial_Get_IRQ_RxCallback(Serial_1) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_1)(Serial_RX_Buff[Serial_1], rec);
		}

		memset(Serial_RX_Buff[Serial_1], NULL, SERIAL_MAX_RECLEN);

		DMA_ClearFlag(Serial1_DMA_RX_Stream, DMA_FLAG_TCIF5);
		DMA_SetCurrDataCounter(Serial1_DMA_RX_Stream, SERIAL_MAX_RECLEN);
		DMA_Cmd(Serial1_DMA_RX_Stream, ENABLE);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		if (Serial_Get_IRQ_RxCallback(Serial_2) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_2)((uint8_t)USART_ReceiveData(USART2), 1);
		}

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial2_DMA_RX_Stream, DISABLE);

		rec = USART2->DR;
		rec = USART2->SR;

		rec = SERIAL_MAX_RECLEN - DMA_GetCurrDataCounter(Serial2_DMA_RX_Stream);

		if (Serial_Get_IRQ_RxCallback(Serial_2) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_2)(Serial_RX_Buff[Serial_2], rec);
		}

		memset(Serial_RX_Buff[Serial_2], NULL, SERIAL_MAX_RECLEN);

		DMA_ClearFlag(Serial2_DMA_RX_Stream, DMA_FLAG_TCIF5);
		DMA_SetCurrDataCounter(Serial2_DMA_RX_Stream, SERIAL_MAX_RECLEN);
		DMA_Cmd(Serial2_DMA_RX_Stream, ENABLE);
	}
}

void USART3_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if (Serial_Get_IRQ_RxCallback(Serial_3) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_3)((uint8_t)USART_ReceiveData(USART3), 1);
		}

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial3_DMA_RX_Stream, DISABLE);

		rec = USART3->DR;
		rec = USART3->SR;

		rec = DMA_GetCurrDataCounter(Serial3_DMA_RX_Stream);
		DMA_ClearFlag(Serial3_DMA_RX_Stream, DMA_FLAG_TCIF1);
		DMA_SetCurrDataCounter(Serial3_DMA_RX_Stream, SERIAL_MAX_RECLEN);
		DMA_Cmd(Serial3_DMA_RX_Stream, ENABLE);
	}
}

void USART6_IRQHandler(void)
{
	uint8_t rec;

	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		if (Serial_Get_IRQ_RxCallback(Serial_6) != NULL)
		{
			Serial_Get_IRQ_RxCallback(Serial_6)((uint8_t)USART_ReceiveData(USART6), 1);
		}

		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(Serial6_DMA_RX_Stream, DISABLE);

		rec = USART6->DR;
		rec = USART6->SR;

		rec = DMA_GetCurrDataCounter(Serial6_DMA_RX_Stream);
		DMA_ClearFlag(Serial6_DMA_RX_Stream, DMA_FLAG_TCIF1);
		DMA_SetCurrDataCounter(Serial6_DMA_RX_Stream, SERIAL_MAX_RECLEN);
		DMA_Cmd(Serial6_DMA_RX_Stream, ENABLE);
	}
}
