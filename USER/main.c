#include "stddef.h"
#include "stdio.h"
#include "runtime.h"
#include "periph_gpio.h"
#include "drv_serial.h"
#include "task_manager.h"
#include "Task_Widget.h"
#include "Task_Shell.h"

void Gpio_Flip(void)
{
	static bool io_level = false;

	if (io_level)
	{
		GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_3, HI);
	}
	else
	{
		GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_3, LO);
	}

	io_level = !io_level;
}

/* the code up top is only for the test currently */
int main(void)
{
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_0, GPIOB);
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_1, GPIOB);
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_2, GPIOB);
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_3, GPIOB);
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_4, GPIOB);
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_5, GPIOB);

	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_0, LO);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_1, LO);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_2, LO);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_3, LO);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_4, LO);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_5, LO);

	SystemInit();

	TaskWidget_Hdl = Task_Create("Widget", TASK_EXEC_100HZ, Group_0, Task_Priority_1, TaskWidget_Core, 200);
	Shell_Tsk_Hdl = Task_Create("Shell", TASK_EXEC_1KHZ, Group_0, Task_Priority_4, TaskShell_Core, 1024);

	TaskSystem_Start();

	return 0;
}
