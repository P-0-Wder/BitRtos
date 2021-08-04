#include "periph_dma_irq.h"
#include "periph_serial.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "radio.h"
#include "periph_gpio.h"

void DMA1_Stream6_IRQHandler(void)
{
#ifdef RADIO_PORT
    #if(RADIO_PORT == Serial_2)
    Serial_DMA_TX_IRQSetting(RADIO_PORT);

    #endif
#endif
}

void DMA1_Stream3_IRQHandler(void)
{
#ifdef RADIO_PORT
    #if(RADIO_PORT == Serial_3)
    Serial_DMA_TX_IRQSetting(RADIO_PORT);

    #endif
#endif
}

void DMA2_Stream7_IRQHandler(void)
{
#ifdef RADIO_PORT
    #if((RADIO_PORT == Serial_1) || (RADIO_PORT == Serial_6))
    Serial_DMA_TX_IRQSetting(RADIO_PORT);

    #endif
#endif
}