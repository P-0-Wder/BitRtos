#ifndef __FC_BOARD_H
#define __FC_BOARD_H

#include "stm32f4xx_spi.h"
#include "debug.h"

#define CRC_CODE_LEN 2

#define MATEKF411 0
#define OMINBUSF4OSD 1
#define FURYF4 0
#define CUSM_BOARD 0
#define MATEKF405 0

#define RADIO 0
#define SDCARD 0

#if SDCARD
#define BLACKBOX 1
#else
#define BLACKBOX 0
#endif

#define DEBUG 0
#define FLASH_CHIIP 0

#if OMINBUSF4OSD
#define MPU6000 1
#define SINGLE_SD 0
#define SBUS_OUT 0
#define IIC_ENABLE 0

#define SBUS_OUT 0
#define SBUS_IN_PORT Serial_6

#if (SBUS_OUT == 1)
#define SBUS_OUT_PORT
#endif

#if RADIO
#define RADIO_PORT Serial_3
#endif

#if SDCARD
#define SDCARD_USE_SPI 1
#define SDCARD_USE_SDIO 0
#define BLACKBOX 1

#define SDCARD_SPI SPI_2
#define SDCARD_CPOL SPI_CPOL_High
#define SDCARD_CPHA SPI_CPHA_2Edge
#define SDCARD_INIT_SPEED SPI_CLOCK_INITIALIZATON
#define SDCARD_MAX_SPEED SPI_CLOCK_FAST

#define SDCARD_CS_IO_PORT GPIOB
#define SDCARD_CS_IO_CLK RCC_AHB1Periph_GPIOB
#define SDCARD_CS_IO_Pin GPIO_Pin_12

#define SDCARD_DETECT_IO_PORT GPIOB
#define SDCARD_DETECT_IO_CLK RCC_AHB1Periph_GPIOB
#define SDCARD_DETECT_IO_Pin GPIO_Pin_7
#endif

#define LED_IO_PORT GPIOB
#define LED_IO_CLK RCC_AHB1Periph_GPIOB
#define LED_IO_Pin GPIO_Pin_5

#if OSD
#define OSD_SPI SPI_3
#define OSD_SPI_CPOL SPI_CPOL_Low
#define OSD_SPI_CPHA SPI_CPHA_1Edge
#define OSD_SPI_SPEED SPI_CLOCK_STANDARD

#define OSD_CS_IO_PORT GPIOA
#define OSD_CS_IO_CLK RCC_AHB1Periph_GPIOA
#define OSD_CS_IO_Pin GPIO_Pin_15
#endif

#define MPU6000_SPI SPI_1
#define MPU6000_SPI_CPOL SPI_CPOL_High
#define MPU6000_SPI_CPHA SPI_CPHA_2Edge
#define MPU6000_SPI_SPEED SPI_CLOCK_ULTRAFAST

#define MPU6000_CS_IO_PORT GPIOA
#define MPU6000_CS_IO_CLK RCC_AHB1Periph_GPIOA
#define MPU6000_CS_IO_Pin GPIO_Pin_4

#define SBus_Inverter1_PORT GPIOC
#define SBus_Inverter1_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter1_Pin GPIO_Pin_0

#define SBus_Inverter6_PORT GPIOC
#define SBus_Inverter6_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter6_Pin GPIO_Pin_8

#define SBus_Inverter3_PORT GPIOC
#define SBus_Inverter3_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter3_Pin GPIO_Pin_9

#define IIC_SCL_CLK RCC_AHB1Periph_GPIOB
#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN GPIO_Pin_7

#define IIC_SDA_CLK RCC_AHB1Periph_GPIOB
#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN GPIO_Pin_6
#endif

#if MATEKF411
#define LED_IO_PORT GPIOC
#define LED_IO_CLK RCC_AHB1Periph_GPIOC
#define LED_IO_Pin GPIO_Pin_13

#define IIC_SCL_CLK RCC_AHB1Periph_GPIOE
#define IIC_SCL_PORT GPIOE
#define IIC_SCL_PIN GPIO_Pin_4

#define IIC_SDA_CLK RCC_AHB1Periph_GPIOE
#define IIC_SDA_PORT GPIOE
#define IIC_SDA_PIN GPIO_Pin_5
#endif

#if MATEKF405
#define MPU6000 1
#define SINGLE_SD 0
#define SBUS_OUT 0

#define MPU6000_SPI SPI_1
#define MPU6000_SPI_CPOL SPI_CPOL_High
#define MPU6000_SPI_CPHA SPI_CPHA_2Edge
#define MPU6000_SPI_SPEED SPI_CLOCK_ULTRAFAST

#define OSD_SPI SPI_3
#define OSD_SPI_CPOL SPI_CPOL_Low
#define OSD_SPI_CPHA SPI_CPHA_1Edge
#define OSD_SPI_SPEED SPI_CLOCK_STANDARD

#define SBUS_PORT Serial_6
#define RADIO_PORT Serial_3

#define LED_IO_PORT GPIOB
#define LED_IO_CLK RCC_AHB1Periph_GPIOB
#define LED_IO_Pin GPIO_Pin_5

#define OSD_CS_IO_PORT GPIOA
#define OSD_CS_IO_CLK RCC_AHB1Periph_GPIOA
#define OSD_CS_IO_Pin GPIO_Pin_15

#define MPU6000_CS_IO_PORT GPIOA
#define MPU6000_CS_IO_CLK RCC_AHB1Periph_GPIOA
#define MPU6000_CS_IO_Pin GPIO_Pin_4

#define SBus_Inverter1_PORT GPIOC
#define SBus_Inverter1_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter1_Pin GPIO_Pin_0

#define SBus_Inverter6_PORT GPIOC
#define SBus_Inverter6_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter6_Pin GPIO_Pin_8

#define SBus_Inverter3_PORT GPIOC
#define SBus_Inverter3_CLK RCC_AHB1Periph_GPIOC
#define SBus_Inverter3_Pin GPIO_Pin_9

#define IIC_SCL_CLK RCC_AHB1Periph_GPIOB
#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN GPIO_Pin_7

#define IIC_SDA_CLK RCC_AHB1Periph_GPIOB
#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN GPIO_Pin_6

#endif

#define M_PIf 3.1415926

typedef enum
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,

	AXIS_SUM,
} AXIS;

typedef enum
{
	Spi,
	Usart,
	I2c,
} Interface_Port;

typedef enum
{
	Acc_2G = 2,
	Acc_4G = 4,
	Acc_8G = 8,
	Acc_16G = 16,
} Acc_Range_List;

typedef enum
{
	Gyro_250Dp = 250,
	Gyro_500Dp = 500,
	Gyro_1000Dp = 1000,
	Gyro_2000Dp = 2000,
} Gyro_Range_List;

#endif
