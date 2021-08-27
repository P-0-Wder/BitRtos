#include "oled1306.h"
#include "delay.h"

/* const static variable */
const uint8_t OledEnable_CMD[2] = {0X14, 0XAF};
const uint8_t OledDisable_CMD[2] = {0X10, 0XAE};

/* external funtion for app or midware */
static void Oled_EnableControl(Oled_Obj_TypeDef *Oled_Obj, Oled_Enable_State_List state);
static bool Oled_Init(Oled_Obj_TypeDef *Oled_Obj);
static void Oled_Clear(Oled_Obj_TypeDef *Oled_Obj);
static bool Oled_MapUpdate(Oled_Obj_TypeDef *Oled_Obj, uint8_t **map);
static uint8_t Oled_GetMax_Width(void);
static uint8_t Oled_GetMax_Height(void);
static bool Oled_SetBright(Oled_Bright_Level lvl);

/* internal function */
static void Oled_TransmitByte(Oled_Obj_TypeDef *Oled_Obj, uint8_t data, Oled_Write_Type type);

Oled_GenProcFunc_TypeDef DrvOled = {
	.init = Oled_Init,
	.enable_set = Oled_EnableControl,
	.fresh = Oled_MapUpdate,
	.clear = Oled_Clear,
	.bright = Oled_SetBright,
	.get_max_height = Oled_GetMax_Height,
	.get_max_width = Oled_GetMax_Width,
};

/* device Bsp IO SPI_Bus correspond control Function */
static void Oled_TransmitByte(Oled_Obj_TypeDef *Oled_Obj, uint8_t data, Oled_Write_Type type)
{
	if (type == Oled_Write_CMD)
	{
		Oled_Obj->dc_ctl(Oled_RS_Disable);
	}
	else
		Oled_Obj->dc_ctl(Oled_RS_Enable);

	Oled_Obj->bus_transmit(data);

	Oled_Obj->dc_ctl(Oled_RS_Enable);
}

static void Oled_EnableControl(Oled_Obj_TypeDef *Oled_Obj, Oled_Enable_State_List state)
{
	uint8_t *Enb_CMD_Buff;
	Oled_TransmitByte(Oled_Obj, 0X8D, Oled_Write_CMD);

	if (state == Oled_Display_Enable)
	{
		Enb_CMD_Buff = (uint8_t *)OledEnable_CMD;
	}
	else
		Enb_CMD_Buff = (uint8_t *)OledDisable_CMD;

	for (uint8_t i = 0; i < sizeof(OledEnable_CMD); i++)
	{
		Oled_TransmitByte(Oled_Obj, Enb_CMD_Buff[i], Oled_Write_CMD);
	}
}

static bool Oled_Init(Oled_Obj_TypeDef *Oled_Obj)
{
	Oled_Obj->bus_init(Oled_Obj->BusID);
	Oled_Obj->dc_init();
	Oled_Obj->rs_init();

	Oled_Obj->rs_ctl(Oled_RS_Disable);
	delay_ms(100);
	Oled_Obj->rs_ctl(Oled_RS_Enable);

	Oled_TransmitByte(Oled_Obj, 0xAE, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xD5, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 80, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xA8, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0X3F, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xD3, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0X00, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0x40, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0x8D, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x14, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x14, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x20, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x02, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0xA0, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xC8, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0xDA, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x12, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0x81, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xEF, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xD9, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xf1, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xDB, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0x30, Oled_Write_CMD);

	Oled_TransmitByte(Oled_Obj, 0xA4, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xA6, Oled_Write_CMD);
	Oled_TransmitByte(Oled_Obj, 0xAF, Oled_Write_CMD);

	Oled_Clear(Oled_Obj);

	return true;
}

static void Oled_Clear(Oled_Obj_TypeDef *Oled_Obj)
{
	for (uint8_t c = 0; c < OLED_COLUMN_BLOCK_NUM; c++)
	{
		Oled_TransmitByte(Oled_Obj, (0xb0 + c), Oled_Write_CMD);
		Oled_TransmitByte(Oled_Obj, 0x00, Oled_Write_CMD);
		Oled_TransmitByte(Oled_Obj, 0x10, Oled_Write_CMD);

		for (uint8_t r = 0; r < OLED_MAX_WIDTH; r++)
		{
			Oled_TransmitByte(Oled_Obj, 0x00, Oled_Write_Data);
		}
	}
}

static uint8_t Oled_GetMax_Width(void)
{
	return OLED_MAX_WIDTH;
}

static uint8_t Oled_GetMax_Height(void)
{
	return OLED_MAX_HEIGHT;
}

static bool Oled_MapUpdate(Oled_Obj_TypeDef *Oled_Obj, uint8_t **map)
{
	uint8_t i = 0;

	if ((Oled_Obj == NULL) || (map == NULL))
		return false;

	for (uint8_t row_index = 0; row_index < ROW_SIZE; row_index++)
	{
		Oled_TransmitByte(Oled_Obj, (0xb0 + row_index), Oled_Write_CMD);
		Oled_TransmitByte(Oled_Obj, 0x00, Oled_Write_CMD);
		Oled_TransmitByte(Oled_Obj, 0x10, Oled_Write_CMD);

		for (uint8_t column_index = 0; column_index < OLED_MAX_WIDTH; column_index++)
		{
			uint8_t tmp = 0;

			for (i = 0; i < ROW_SIZE; i++)
			{
				tmp |= map[row_index * ROW_SIZE + i][column_index] << i;
			}

			Oled_TransmitByte(Oled_Obj, tmp, Oled_Write_Data);
		}
	}
	return true;
}

static bool Oled_SetBright(Oled_Bright_Level lvl)
{
	return true;
}
