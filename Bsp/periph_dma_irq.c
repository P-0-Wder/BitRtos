#include "periph_dma_irq.h"
#include "periph_serial.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "periph_gpio.h"

void DMA1_Stream6_IRQHandler(void)
{
    Serial_DMA_TX_IRQSetting(Serial_2);
}

void DMA1_Stream3_IRQHandler(void)
{

    Serial_DMA_TX_IRQSetting(Serial_3);
}

void DMA2_Stream7_IRQHandler(void)
{
    Serial_DMA_TX_IRQSetting(Serial_1);
   
}