#include "delay.h"
#include "runtime.h"
#include "stm32f4xx_it.h"
#include "misc.h"

#define configTICK_RATE_HZ 167

static volatile uint32_t us_time;

static u8 fac_us = 0;
static u16 fac_ms = 0;

void delay_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	fac_us = configTICK_RATE_HZ + 1;
	reload = configTICK_RATE_HZ + 1;
	reload *= 1000000 / configTICK_RATE_HZ;
	fac_ms = 1000 / configTICK_RATE_HZ;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD = reload;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void delay_us(u32 nus)
{
	nus *= 2;
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;		//需要的节拍数
	told = SysTick->VAL;		//刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
}

void delay_ms(uint32_t MS)
{
	while (MS--)
	{
		delay_us(REAL_MS);
	}
}
