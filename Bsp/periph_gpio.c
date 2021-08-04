#include "periph_gpio.h"
#include "stm32f4xx_gpio.h"

void GPIO_USART1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(USART1_IO_CLK, ENABLE);

	GPIO_PinAFConfig(USART1_IO_PORT, USART1_TX_PinSource, GPIO_AF_USART1);
	GPIO_PinAFConfig(USART1_IO_PORT, USART1_RX_PinSource, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN | USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(USART1_IO_PORT, &GPIO_InitStructure);
}

void GPIO_USART2_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(USART2_IO_CLK, ENABLE);

	GPIO_PinAFConfig(USART2_IO_PORT, USART2_TX_PinSource, GPIO_AF_USART2);
	GPIO_PinAFConfig(USART2_IO_PORT, USART2_RX_PinSource, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN | USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(USART2_IO_PORT, &GPIO_InitStructure);
}

void GPIO_USART3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(USART3_IO_CLK, ENABLE);

	GPIO_PinAFConfig(USART3_IO_PORT, USART3_TX_PinSource, GPIO_AF_USART3);
	GPIO_PinAFConfig(USART3_IO_PORT, USART3_RX_PinSource, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN | USART3_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(USART3_IO_PORT, &GPIO_InitStructure);
}

void GPIO_USART6_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(USART6_IO_CLK, ENABLE);

	GPIO_PinAFConfig(USART6_IO_PORT, USART6_TX_PinSource, GPIO_AF_USART6);
	GPIO_PinAFConfig(USART6_IO_PORT, USART6_RX_PinSource, GPIO_AF_USART6);

	GPIO_InitStructure.GPIO_Pin = USART6_TX_PIN | USART6_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(USART6_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM2_PWMCH1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM2_PWMCH1_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM2_PWMCH1_IO_PORT, TIM2_PWMCH1_PinSource, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = TIM2_PWMCH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM2_PWMCH1_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM2_PWMCH2_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM2_PWMCH2_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM2_PWMCH2_IO_PORT, TIM2_PWMCH2_PinSource, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = TIM2_PWMCH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM2_PWMCH2_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM2_PWMCH3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM2_PWMCH3_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM2_PWMCH3_IO_PORT, TIM2_PWMCH3_PinSource, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = TIM2_PWMCH3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM2_PWMCH3_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM2_PWMCH4_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM2_PWMCH4_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM2_PWMCH4_IO_PORT, TIM2_PWMCH4_PinSource, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = TIM2_PWMCH4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM2_PWMCH4_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM3_PWMCH1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM3_PWMCH1_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM3_PWMCH1_IO_PORT, TIM3_PWMCH1_PinSource, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = TIM3_PWMCH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM3_PWMCH1_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM3_PWMCH2_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM3_PWMCH2_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM3_PWMCH2_IO_PORT, TIM3_PWMCH2_PinSource, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = TIM3_PWMCH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM3_PWMCH2_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM3_PWMCH3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM3_PWMCH3_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM3_PWMCH3_IO_PORT, TIM3_PWMCH3_PinSource, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = TIM3_PWMCH3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM3_PWMCH3_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM3_PWMCH4_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM3_PWMCH4_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM3_PWMCH4_IO_PORT, TIM3_PWMCH4_PinSource, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = TIM3_PWMCH4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM3_PWMCH4_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM4_PWMCH1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM4_PWMCH1_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM4_PWMCH1_IO_PORT, TIM4_PWMCH1_PinSource, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = TIM4_PWMCH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM4_PWMCH1_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM4_PWMCH2_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM4_PWMCH2_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM4_PWMCH2_IO_PORT, TIM4_PWMCH2_PinSource, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = TIM4_PWMCH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM4_PWMCH2_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM4_PWMCH3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM4_PWMCH3_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM4_PWMCH3_IO_PORT, TIM4_PWMCH3_PinSource, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = TIM4_PWMCH3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM4_PWMCH3_IO_PORT, &GPIO_InitStructure);
}

void GPIO_TIM4_PWMCH4_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM4_PWMCH4_IO_CLK, ENABLE);

	GPIO_PinAFConfig(TIM4_PWMCH4_IO_PORT, TIM4_PWMCH4_PinSource, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = TIM4_PWMCH4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(TIM4_PWMCH4_IO_PORT, &GPIO_InitStructure);
}

void GPIO_SPI1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SPI1_MISO_IO_CLK | SPI1_MOSI_IO_CLK | SPI1_CLK_IO_CLK, ENABLE);

	GPIO_PinAFConfig(SPI1_MISO_IO_PORT, SPI1_MISO_PinSource, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI1_MOSI_IO_PORT, SPI1_MOSI_PinSource, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI1_CLK_IO_PORT, SPI1_CLK_PinSource, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_IO_Pin | SPI1_MISO_IO_Pin | SPI1_CLK_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SPI1_CLK_IO_PORT, &GPIO_InitStructure);
}

void GPIO_SPI2_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SPI2_MISO_IO_CLK | SPI2_MOSI_IO_CLK | SPI2_CLK_IO_CLK, ENABLE);

	GPIO_PinAFConfig(SPI2_MISO_IO_PORT, SPI2_MISO_PinSource, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI2_MOSI_IO_PORT, SPI2_MOSI_PinSource, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI2_CLK_IO_PORT, SPI2_CLK_PinSource, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_IO_Pin | SPI2_MISO_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SPI2_MOSI_IO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_CLK_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SPI2_CLK_IO_PORT, &GPIO_InitStructure);
}

void GPIO_SPI3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SPI3_MISO_IO_CLK | SPI3_MOSI_IO_CLK | SPI3_CLK_IO_CLK, ENABLE);

	GPIO_PinAFConfig(SPI3_MISO_IO_PORT, SPI3_MISO_PinSource, GPIO_AF_SPI3);
	GPIO_PinAFConfig(SPI3_MOSI_IO_PORT, SPI3_MOSI_PinSource, GPIO_AF_SPI3);
	GPIO_PinAFConfig(SPI3_CLK_IO_PORT, SPI3_CLK_PinSource, GPIO_AF_SPI3);

	GPIO_InitStructure.GPIO_Pin = SPI3_MOSI_IO_Pin | SPI3_MISO_IO_Pin | SPI3_CLK_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(SPI3_MOSI_IO_PORT, &GPIO_InitStructure);
}

void GPIO_IIC1_IO_Init(void)
{
}

void GPIO_IIC2_IO_Init(void)
{
}

void GPIO_IO_Output_Init(uint32_t RCC_AHBCLK, uint16_t pin, GPIO_TypeDef *GPIO_Port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHBCLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = pin;

	GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

void GPIO_IO_Input_Init(uint32_t RCC_AHBCLK, uint16_t pin, GPIO_TypeDef *GPIO_Port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHBCLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = pin;

	GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

uint8_t GPIO_Get_IO_LEVEL(GPIO_TypeDef *GPIO_Port, uint16_t pin)
{
	uint8_t level = 0;

	level = GPIO_ReadInputDataBit(GPIO_Port, pin);

	return level;
}

void GPIO_Set_IO_LEVEL(GPIO_TypeDef *GPIO_Port, uint16_t pin, IO_LEVEL level)
{
	switch ((uint8_t)level)
	{
	case HI:
		GPIO_ResetBits(GPIO_Port, pin);
		break;
	case LO:
		GPIO_SetBits(GPIO_Port, pin);
		break;
	default:
		break;
	}
}
