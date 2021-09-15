#ifndef __PERIPH_SERIAL_H
#define __PERIPH_SERIAL_H

#include "stm32f4xx.h"

#define Serial_Port_Sum 4

#define SBUS_BOUND 100000
#define Serial_921600 921600
#define Serial_460800 460800
#define Serial_115200 115200
#define Serial_57600 57600
#define Serial_38400 38400
#define Serial_9600 9600

typedef enum
{
	Serial_Normal = 0,
	Serial_Sbus,
	Serial_Func_Sum,
} Serial_Func_Type;

#ifdef STM32F40XX
typedef enum
{
	Serial_1 = 0,
	Serial_2 = 1,
	Serial_3 = 2,
	Serial_6 = 3,
} Serial_List;
#endif

typedef enum
{
	IRQ,
	DMA,
} Serial_Mode;

void Serial_Deinit(Serial_List Serial);

void Serial_IRQ_RX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, Serial_Func_Type type);
void Serial_DMA_RX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t RX_Buff, uint16_t Buff_Size, Serial_Func_Type type);
void Serial_DMA_TX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t TX_Buff, uint16_t Buff_Size, Serial_Func_Type type);
void Serial_DMA_RXTX_Init(Serial_List Serial, uint32_t bound, uint8_t PreemptionPriority, uint8_t SubPriority, uint32_t RX_Buff, uint32_t TX_Buff, uint16_t Buff_Size, Serial_Func_Type type);

void Serial_SendStr(USART_TypeDef *Serial_port, const char *Str_Output);
void Serial_SendBuff(USART_TypeDef *Serial_port, char *Buff, uint16_t Len);

void Serial_DMA_TX_IRQSetting(Serial_List serial_id);
void Serial_DMA_SendBuff(Serial_List serial_id, uint16_t len);

#endif
