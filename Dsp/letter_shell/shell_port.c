#include "shell.h"
#include <stdio.h>
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

/*can print the characters to the terminal with this function */
void user_printf_shell(const int *ch,...)
{
	shellWriteString(&shell,ch);
}

void bt_printf(const char *fmt, ...)
{
	va_list args;
	static char bt_log_buf[128];
	int i = 0;
	int x = 0;
	va_start(args,fmt);


	vsnprintf(bt_log_buf,sizeof(bt_log_buf)-1,fmt,args);
	user_printf_shell(bt_log_buf);
	va_end(args);
}

void Shell_Init(void)
{
	shell.write = user_shell_write;
	
	shellInit(&shell,shell_buff,512);
}



