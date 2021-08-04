#ifndef __PERIPH_GPIO_H
#define __PERIPH_GPIO_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

//Serial IO definition
#define USART1_IO_PORT GPIOA
#define USART2_IO_PORT GPIOA
#define USART3_IO_PORT GPIOB
#define USART6_IO_PORT GPIOC

#define USART1_IO_CLK RCC_AHB1Periph_GPIOA
#define USART2_IO_CLK RCC_AHB1Periph_GPIOA
#define USART3_IO_CLK RCC_AHB1Periph_GPIOB
#define USART6_IO_CLK RCC_AHB1Periph_GPIOC

#define USART1_TX_PIN GPIO_Pin_9
#define USART1_RX_PIN GPIO_Pin_10
#define USART2_TX_PIN GPIO_Pin_2
#define USART2_RX_PIN GPIO_Pin_3
#define USART3_TX_PIN GPIO_Pin_10
#define USART3_RX_PIN GPIO_Pin_11
#define USART6_TX_PIN GPIO_Pin_6
#define USART6_RX_PIN GPIO_Pin_7

#define USART1_TX_PinSource GPIO_PinSource9
#define USART1_RX_PinSource GPIO_PinSource10
#define USART2_TX_PinSource GPIO_PinSource2
#define USART2_RX_PinSource GPIO_PinSource3
#define USART3_TX_PinSource GPIO_PinSource10
#define USART3_RX_PinSource GPIO_PinSource11
#define USART6_TX_PinSource GPIO_PinSource6
#define USART6_RX_PinSource GPIO_PinSource7
//Timer PWM Output IO definition
#define TIM2_PWMCH1_IO_PORT GPIOA
#define TIM2_PWMCH2_IO_PORT GPIOA
#define TIM2_PWMCH3_IO_PORT GPIOA
#define TIM2_PWMCH4_IO_PORT GPIOA

#define TIM3_PWMCH1_IO_PORT GPIOA
#define TIM3_PWMCH2_IO_PORT GPIOA
#define TIM3_PWMCH3_IO_PORT GPIOB
#define TIM3_PWMCH4_IO_PORT GPIOB

#define TIM4_PWMCH1_IO_PORT GPIOB
#define TIM4_PWMCH2_IO_PORT GPIOB
#define TIM4_PWMCH3_IO_PORT GPIOB
#define TIM4_PWMCH4_IO_PORT GPIOB

#define TIM2_PWMCH1_IO_CLK RCC_AHB1Periph_GPIOA
#define TIM2_PWMCH2_IO_CLK RCC_AHB1Periph_GPIOA
#define TIM2_PWMCH3_IO_CLK RCC_AHB1Periph_GPIOA
#define TIM2_PWMCH4_IO_CLK RCC_AHB1Periph_GPIOA

#define TIM3_PWMCH1_IO_CLK RCC_AHB1Periph_GPIOA
#define TIM3_PWMCH2_IO_CLK RCC_AHB1Periph_GPIOA
#define TIM3_PWMCH3_IO_CLK RCC_AHB1Periph_GPIOB
#define TIM3_PWMCH4_IO_CLK RCC_AHB1Periph_GPIOB

#define TIM4_PWMCH1_IO_CLK RCC_AHB1Periph_GPIOB
#define TIM4_PWMCH2_IO_CLK RCC_AHB1Periph_GPIOB
#define TIM4_PWMCH3_IO_CLK RCC_AHB1Periph_GPIOB
#define TIM4_PWMCH4_IO_CLK RCC_AHB1Periph_GPIOB

#define TIM2_PWMCH1_PIN GPIO_Pin_0
#define TIM2_PWMCH2_PIN GPIO_Pin_1
#define TIM2_PWMCH3_PIN GPIO_Pin_2
#define TIM2_PWMCH4_PIN GPIO_Pin_3

#define TIM3_PWMCH1_PIN GPIO_Pin_6
#define TIM3_PWMCH2_PIN GPIO_Pin_7
#define TIM3_PWMCH3_PIN GPIO_Pin_0
#define TIM3_PWMCH4_PIN GPIO_Pin_1

#define TIM4_PWMCH1_PIN GPIO_Pin_6
#define TIM4_PWMCH2_PIN GPIO_Pin_7
#define TIM4_PWMCH3_PIN GPIO_Pin_8
#define TIM4_PWMCH4_PIN GPIO_Pin_9

#define TIM2_PWMCH1_PinSource GPIO_PinSource0
#define TIM2_PWMCH2_PinSource GPIO_PinSource1
#define TIM2_PWMCH3_PinSource GPIO_PinSource2
#define TIM2_PWMCH4_PinSource GPIO_PinSource3

#define TIM3_PWMCH1_PinSource GPIO_PinSource6
#define TIM3_PWMCH2_PinSource GPIO_PinSource7
#define TIM3_PWMCH3_PinSource GPIO_PinSource0
#define TIM3_PWMCH4_PinSource GPIO_PinSource1

#define TIM4_PWMCH1_PinSource GPIO_PinSource6
#define TIM4_PWMCH2_PinSource GPIO_PinSource7
#define TIM4_PWMCH3_PinSource GPIO_PinSource8
#define TIM4_PWMCH4_PinSource GPIO_PinSource9
//SPI IO definition
#define SPI1_MOSI_IO_PORT GPIOA
#define SPI1_MISO_IO_PORT GPIOA
#define SPI1_CLK_IO_PORT GPIOA

#define SPI2_MOSI_IO_PORT GPIOB
#define SPI2_MISO_IO_PORT GPIOB
#define SPI2_CLK_IO_PORT GPIOB

#define SPI3_MOSI_IO_PORT GPIOC
#define SPI3_MISO_IO_PORT GPIOC
#define SPI3_CLK_IO_PORT GPIOC

#define SPI1_MOSI_IO_CLK RCC_AHB1Periph_GPIOA
#define SPI1_MISO_IO_CLK RCC_AHB1Periph_GPIOA
#define SPI1_CLK_IO_CLK RCC_AHB1Periph_GPIOA

#define SPI2_MOSI_IO_CLK RCC_AHB1Periph_GPIOB
#define SPI2_MISO_IO_CLK RCC_AHB1Periph_GPIOB
#define SPI2_CLK_IO_CLK RCC_AHB1Periph_GPIOB

#define SPI3_MOSI_IO_CLK RCC_AHB1Periph_GPIOC
#define SPI3_MISO_IO_CLK RCC_AHB1Periph_GPIOC
#define SPI3_CLK_IO_CLK RCC_AHB1Periph_GPIOC

#define SPI1_MOSI_IO_Pin GPIO_Pin_7
#define SPI1_MISO_IO_Pin GPIO_Pin_6
#define SPI1_CLK_IO_Pin GPIO_Pin_5

#define SPI2_MOSI_IO_Pin GPIO_Pin_15
#define SPI2_MISO_IO_Pin GPIO_Pin_14
#define SPI2_CLK_IO_Pin GPIO_Pin_13

#define SPI3_MOSI_IO_Pin GPIO_Pin_12
#define SPI3_MISO_IO_Pin GPIO_Pin_11
#define SPI3_CLK_IO_Pin GPIO_Pin_10

#define SPI1_MOSI_PinSource GPIO_PinSource7
#define SPI1_MISO_PinSource GPIO_PinSource6
#define SPI1_CLK_PinSource GPIO_PinSource5

#define SPI2_MOSI_PinSource GPIO_PinSource15
#define SPI2_MISO_PinSource GPIO_PinSource14
#define SPI2_CLK_PinSource GPIO_PinSource13

#define SPI3_MOSI_PinSource GPIO_PinSource12
#define SPI3_MISO_PinSource GPIO_PinSource11
#define SPI3_CLK_PinSource GPIO_PinSource10

//IIC IO definition

typedef enum
{
	HI = 0,
	LO = 1,
} IO_LEVEL;

void GPIO_USART1_IO_Init(void);
void GPIO_USART2_IO_Init(void);
void GPIO_USART3_IO_Init(void);
void GPIO_USART6_IO_Init(void);

void GPIO_TIM2_PWMCH1_IO_Init(void);
void GPIO_TIM2_PWMCH2_IO_Init(void);
void GPIO_TIM2_PWMCH3_IO_Init(void);
void GPIO_TIM2_PWMCH4_IO_Init(void);

void GPIO_TIM3_PWMCH1_IO_Init(void);
void GPIO_TIM3_PWMCH2_IO_Init(void);
void GPIO_TIM3_PWMCH3_IO_Init(void);
void GPIO_TIM3_PWMCH4_IO_Init(void);

void GPIO_TIM4_PWMCH1_IO_Init(void);
void GPIO_TIM4_PWMCH2_IO_Init(void);
void GPIO_TIM4_PWMCH3_IO_Init(void);
void GPIO_TIM4_PWMCH4_IO_Init(void);

void GPIO_SPI1_IO_Init(void);
void GPIO_SPI2_IO_Init(void);
void GPIO_SPI3_IO_Init(void);

void GPIO_IO_Output_Init(uint32_t RCC_AHBCLK, uint16_t pin, GPIO_TypeDef *GPIO_Port);
void GPIO_IO_Input_Init(uint32_t RCC_AHBCLK, uint16_t pin, GPIO_TypeDef *GPIO_Port);
void GPIO_Set_IO_LEVEL(GPIO_TypeDef *GPIO_Port, uint16_t pin, IO_LEVEL level);
uint8_t GPIO_Get_IO_LEVEL(GPIO_TypeDef *GPIO_Port, uint16_t pin);

#endif
