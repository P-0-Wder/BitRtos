#include "stddef.h"
#include "stdio.h"
#include "runtime.h"
#include "periph_gpio.h"
#include "drv_serial.h"
#include "task_manager.h"
#include "Task_Widget.h"
#include "Task_Input.h"
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
	/* test pin */
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
	/* test pin */

	SystemInit();

	SigInput_Tsk_Hdl = Task_Create("Input", TASK_EXEC_50HZ, Group_0, Task_Priority_2, TaskInput_Core, 256);
	Widget_Tsk_Hdl = Task_Create("Widget", TASK_EXEC_100HZ, Group_0, Task_Priority_1, TaskWidget_Core, 256);
	Shell_Tsk_Hdl = Task_Create("Shell", TASK_EXEC_100HZ, Group_0, Task_Priority_0, TaskShell_Core, 2048);

	TaskSystem_Start();

	return 0;
}
