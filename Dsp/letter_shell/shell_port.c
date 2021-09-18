#include "shell.h"
#include "stm32f4xx.h"
#include "periph_serial.h"
#include "shell_port.h"
#include "stm32f4xx_usart.h"
#include "delay.h"
#include "periph_dma.h"

Shell shell;
char shell_buff[512];
uint16_t dma_shell_buf[1] = {0};

void user_shell_write(const char ch)
{	
		dma_shell_buf[0] = ch;
		Serial_DMA_SendBuff(Serial_1,1);
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
}

void Shell_Init(void)
{
	shell.write = user_shell_write;
	
	shellInit(&shell,shell_buff,512);
}



