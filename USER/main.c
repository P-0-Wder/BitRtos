#include "stddef.h"
#include "stdio.h"
#include "runtime.h"
#include "periph_gpio.h"

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

	io_level = ~io_level;
}

int main(void)
{
	GPIO_IO_Output_Init(RCC_AHB1Periph_GPIOB, GPIO_Pin_3, GPIOB);

	Runtime_Config(RUNTIME_TICK_FRQ_200K);
	Runtime_Set_start_Callback(NULL);
	Runtime_Set_stop_Callback(NULL);
	Runtime_Set_tick_Callback(Gpio_Flip);
	Runtime_Start();

	while (true)
	{
	}

	return 0;
}
