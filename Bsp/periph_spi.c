#include "stm32f4xx_spi.h"
#include "periph_gpio.h"
#include "periph_spi.h"

uint16_t SpiErrorCount = 0;
uint16_t SpiErrorCount2 = 0;

static void (*SPI_IO_Init[SPI_PORT_SUM])() = {GPIO_SPI1_IO_Init, GPIO_SPI2_IO_Init, GPIO_SPI3_IO_Init};

static SPI_TypeDef *SPI_PORT[SPI_PORT_SUM] = {SPI1, SPI2, SPI3};

static uint32_t SPI_CLK[SPI_PORT_SUM] = {RCC_APB2Periph_SPI1, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3};

static const uint16_t divisorMap[] = {
	SPI_BaudRatePrescaler_256, // SPI_CLOCK_INITIALIZATON      328.125 KBits/s
	SPI_BaudRatePrescaler_128, // SPI_CLOCK_SLOW               656.25 KBits/s
	SPI_BaudRatePrescaler_8,   // SPI_CLOCK_STANDARD           10.5 MBits/s
	SPI_BaudRatePrescaler_4,   // SPI_CLOCK_FAST               21.0 MBits/s
	SPI_BaudRatePrescaler_2	   // SPI_CLOCK_ULTRAFAST          42.0 MBits/s
};

void periph_SPI_DeInit(SPI_List SPIx)
{
	SPI_I2S_DeInit(SPI_PORT[SPIx]);
}

void periph_SPI_Init(SPI_List SPIx, SPIClockSpeed_e speed, uint16_t CPOL, uint16_t CPHA)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_IO_Init[SPIx]();

	if (SPIx == SPI_1)
	{
		RCC_APB2PeriphClockCmd(SPI_CLK[SPIx], ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(SPI_CLK[SPIx], ENABLE);
	}

	SPI_InitStructure.SPI_CPOL = CPOL;
	SPI_InitStructure.SPI_CPHA = CPHA;

	SPI_I2S_DeInit(SPI_PORT[SPIx]);
	SPI_Cmd(SPI_PORT[SPIx], DISABLE);

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_BaudRatePrescaler = divisorMap[(uint8_t)speed];
	SPI_Init(SPI_PORT[SPIx], &SPI_InitStructure);

	SPI_Cmd(SPI_PORT[SPIx], ENABLE);
}

void periph_SPI_SetSpeed(SPI_List SPIx, SPIClockSpeed_e speed)
{
#define BR_CLEAR_MASK 0xFFC7
	SPI_Cmd(SPI_PORT[SPIx], DISABLE);

	uint16_t tempRegister = SPI_PORT[SPIx]->CR1;
	tempRegister &= BR_CLEAR_MASK;
	tempRegister |= divisorMap[speed];
	SPI_PORT[SPIx]->CR1 = tempRegister;
	SPI_Cmd(SPI_PORT[SPIx], ENABLE);
}

bool periph_SPI_WriteByte(SPI_List SPIx, uint8_t TxData)
{
	uint16_t retry = 0;
	while (SPI_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if (retry > SPI_TIMEOUT)
			return false;
	}
	SPI_I2S_SendData(SPI_PORT[SPIx], TxData);
	return true;
}

bool periph_SPI_ReadByte(SPI_List SPIx, uint8_t *data)
{
	uint16_t retry = 0;
	while (SPI_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if (retry > SPI_TIMEOUT)
			return false;
	}
	SPI_I2S_SendData(SPI_PORT[SPIx], 0XFF);

	retry = 0;
	while (SPI_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if (retry > SPI_TIMEOUT)
			return false;
	}
	*data = SPI_I2S_ReceiveData(SPI_PORT[SPIx]);
	return true;
}

bool periph_SPI_ReadWriteByte(SPI_List SPIx, uint8_t TxData, uint8_t *RxData)
{
	uint16_t retry = 0;
	while (SPI_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if (retry > SPI_TIMEOUT)
			return false;
	}
	SPI_I2S_SendData(SPI_PORT[SPIx], TxData);

	retry = 0;
	while (SPI_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if (retry > SPI_TIMEOUT)
			return false;
	}

	*RxData = SPI_I2S_ReceiveData(SPI_PORT[SPIx]);

	return true;
}

uint16_t periph_SPI_Transfer(SPI_List SPIx, uint8_t *rx, uint8_t *tx, uint16_t len)
{
	uint16_t spitimeout = SPI_TIMEOUT;
	uint8_t b;
	SPI_PORT[SPIx]->DR;
	while (len--)
	{
		b = tx ? *(tx++) : 0xff;

		while (SPI_I2S_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_TXE) == RESET)
		{
			if ((spitimeout--) == 0)
			{
				return 0;
			}
		}
		SPI_I2S_SendData(SPI_PORT[SPIx], b);

		spitimeout = SPI_TIMEOUT;
		while (SPI_I2S_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_RXNE) == RESET)
		{
			if ((spitimeout--) == 0)
			{
				return 0;
			}
		}

		b = SPI_I2S_ReceiveData(SPI_PORT[SPIx]);
		if (rx)
		{
			*(rx++) = b;
		}
	}
	return len;
}

uint8_t periph_SPI_CheckBusy(SPI_List SPIx)
{
	return SPI_I2S_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_TXE) == RESET || SPI_I2S_GetFlagStatus(SPI_PORT[SPIx], SPI_I2S_FLAG_BSY) == SET;
}
