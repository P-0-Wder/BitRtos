#include "periph_dma_irq.h"
#include "periph_serial.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"

void DMA1_Stream6_IRQHandler(void)
{
    if (periph_Get_DMA_IRQCallback(DMA_1, DMA_Stream_6) != NULL)
    {
        periph_Get_DMA_IRQCallback(DMA_1, DMA_Stream_6)();
    }
}

void DMA1_Stream3_IRQHandler(void)
{
    if (periph_Get_DMA_IRQCallback(DMA_1, DMA_Stream_3) != NULL)
    {
        periph_Get_DMA_IRQCallback(DMA_1, DMA_Stream_3)();
    }
}

void DMA2_Stream7_IRQHandler(void)
{
    if (periph_Get_DMA_IRQCallback(DMA_2, DMA_Stream_7) != NULL)
    {
        periph_Get_DMA_IRQCallback(DMA_2, DMA_Stream_7)();
    }
}