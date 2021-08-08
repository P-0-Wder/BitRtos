#include "stddef.h"
#include "stdio.h"
#include "runtime.h"
#include "periph_gpio.h"
#include "task_manager.h"

Task_Handler test1_tsk_hdl;
Task_Handler test2_tsk_hdl;
Task_Handler test3_tsk_hdl;
Task_Handler test4_tsk_hdl;

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

void Task_test_1(Task_Handler self)
{
	static uint8_t a = 0;

	a++;
}

void Task_test_2(Task_Handler self)
{
	static uint8_t a = 0;

	a++;
}

void Task_test_3(Task_Handler self)
{
	static uint8_t a = 0;

	a++;
}

void Task_test_4(Task_Handler self)
{
	static uint8_t a = 0;

	a++;
}

int main(void)
{
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_3, GPIOB);
	GPIO_Set_IO_LEVEL(GPIOB, GPIO_Pin_3, LO);
	SystemInit();

	test1_tsk_hdl = Task_Create("test 1", TASK_EXEC_4KHZ, Group_0, Task_Priority_1, Task_test_1, 200);
	test2_tsk_hdl = Task_Create("test 2", TASK_EXEC_2KHZ, Group_0, Task_Priority_0, Task_test_2, 200);
	test3_tsk_hdl = Task_Create("test 3", TASK_EXEC_1KHZ, Group_0, Task_Priority_2, Task_test_3, 200);
	test4_tsk_hdl = Task_Create("test 4", TASK_EXEC_1KHZ, Group_0, Task_Priority_5, Task_test_4, 200);

	TaskSystem_Start();

	return 0;
}
