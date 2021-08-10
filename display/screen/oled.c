#include "oled.h"

/* varible definition */
uint8_t OLED_GRAM[128][8];

const uint8_t OledEnable_CMD[2] = {0X14, 0XAF};
const uint8_t OledDisable_CMD[2] = {0X10, 0XAE};

/* external funtion for app or midware */
static void Oled_EnableControl(Oled_Obj_TypeDef *Oled_Obj, Oled_Enable_State_List state);

/* internal function */
static void Oled_TransmitByte(Oled_Obj_TypeDef *Oled_Obj, uint8_t data, Oled_Write_Type type);

/* device Bsp IO SPI_Bus correspond control Function */
static void Oled_TransmitByte(Oled_Obj_TypeDef *Oled_Obj, uint8_t data, Oled_Write_Type type)
{
	if (type == Oled_Write_CMD)
	{
		Oled_Obj->rs_ctl(Oled_RS_Disable);
	}
	else
		Oled_Obj->rs_ctl(Oled_RS_Enable);

	Oled_Obj->bus_transmit(data);

	Oled_Obj->rs_ctl(Oled_RS_Disable);
}

static void Oled_EnableControl(Oled_Obj_TypeDef *Oled_Obj, Oled_Enable_State_List state)
{
	Oled_TransmitByte(Oled_Obj, 0X8D, Oled_Write_CMD);
}

void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

void OLED_Refresh_Gram(void)
{
	s16 i, n;

	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD);
		OLED_WR_Byte(0x00, OLED_CMD);
		OLED_WR_Byte(0x10, OLED_CMD);
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
	}
}

void OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		for (n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0X00;
		}
	}
	OLED_Refresh_Gram();
}

void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
	u8 pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return;
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if (t)
		OLED_GRAM[x][pos] |= temp;
	else
		OLED_GRAM[x][pos] &= ~temp;
}

u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}

void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	OLED_SPI_Init();

	OLED_RST_Clr();
	Delay(1000);
	OLED_RST_Set();

	OLED_WR_Byte(0xAE, OLED_CMD);
	OLED_WR_Byte(0xD5, OLED_CMD);
	OLED_WR_Byte(80, OLED_CMD);
	OLED_WR_Byte(0xA8, OLED_CMD);
	OLED_WR_Byte(0X3F, OLED_CMD);
	OLED_WR_Byte(0xD3, OLED_CMD);
	OLED_WR_Byte(0X00, OLED_CMD);

	OLED_WR_Byte(0x40, OLED_CMD);

	OLED_WR_Byte(0x8D, OLED_CMD);
	OLED_WR_Byte(0x14, OLED_CMD);
	OLED_WR_Byte(0x20, OLED_CMD);
	OLED_WR_Byte(0x02, OLED_CMD);

	OLED_WR_Byte(0xA0, OLED_CMD);
	OLED_WR_Byte(0xC8, OLED_CMD);

	OLED_WR_Byte(0xDA, OLED_CMD);
	OLED_WR_Byte(0x12, OLED_CMD);

	OLED_WR_Byte(0x81, OLED_CMD);
	OLED_WR_Byte(0xEF, OLED_CMD);
	OLED_WR_Byte(0xD9, OLED_CMD);
	OLED_WR_Byte(0xf1, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD);
	OLED_WR_Byte(0x30, OLED_CMD);

	OLED_WR_Byte(0xA4, OLED_CMD);
	OLED_WR_Byte(0xA6, OLED_CMD);
	OLED_WR_Byte(0xAF, OLED_CMD);
	OLED_Clear();
}
