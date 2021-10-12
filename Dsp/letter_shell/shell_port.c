#include "shell.h"
#include "stm32f4xx.h"
#include "periph_serial.h"
#include "shell_port.h"
#include "stm32f4xx_usart.h"
#include "delay.h"
#include "periph_dma.h"

Shell shell;
char shell_buff[512];
int dma_shell_buf[40] = {0};

void user_shell_write(const int ch)
{	
		dma_shell_buf[0] = ch;
		Serial_DMA_SendBuff(Serial_1,1);
		while(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7) != RESET)
		{
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
			break;
		}

}

void user_shell_write_byte(const int ch,int len)
{
	int i = 0;
	for(;i<len;i++)
	{
		user_shell_write(ch);
	}
}

void Shell_Init(void)
{
	shell.write = user_shell_write;
	
	shellInit(&shell,shell_buff,512);
}



