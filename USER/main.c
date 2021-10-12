#include "stddef.h"
#include "stdio.h"
#include "runtime.h"
#include "periph_gpio.h"
#include "task_manager.h"
#include "Task_Widget.h"
#include "shell_port.h"
#include "periph_serial.h"
#include "stm32f4xx_usart.h"
#include "periph_dma.h"

Task_Handler test1_tsk_hdl;
Task_Handler test2_tsk_hdl;
Task_Handler test3_tsk_hdl;
Task_Handler test4_tsk_hdl;

extern uint16_t dma_shell_buf[40];
extern uint8_t shell_recv ;

int main(void)
{
	delay_init();
	
	Serial_DMA_RXTX_Init(Serial_1,115200,3,3,&shell_recv,dma_shell_buf,40,0);
	Shell_Init();
	SystemInit();

	while(1)
	{
		
	}
	TaskSystem_Start();
	
	return 0;
}

void sum(void)
{
	user_shell_write_byte('n',4);
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
sum, sum, test);

void test1(void)
{
	user_shell_write('j');
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
test1, test1, test);