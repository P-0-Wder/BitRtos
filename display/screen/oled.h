#ifndef __OLED_H
#define __OLED_H
//#include "sysconfig.h"
#include "stm32f10x.h"
#include "show.h"

//////////////////////////////////////////////
#define OLED_GPIO_SPI GPIOA
#define OLED_SPI_Pin_SCK GPIO_Pin_5
#define OLED_SPI_Pin_MOSI GPIO_Pin_7
#define OLED_RCC_GPIO_SPI RCC_APB2Periph_GPIOA
//////////////////////////////////////////////

#define OLED_DC_GPIO GPIOB
#define OLED_DC_Pin GPIO_Pin_7

#define OLED_RST_GPIO GPIOB
#define OLED_RST_Pin GPIO_Pin_6

#define OLED_DC_H OLED_DC_GPIO->BSRR = OLED_DC_Pin //高电平
#define OLED_DC_L OLED_DC_GPIO->BRR = OLED_DC_Pin  //低电平

#define OLED_RST_H OLED_RST_GPIO->BSRR = OLED_RST_Pin //高电平
#define OLED_RST_L OLED_RST_GPIO->BRR = OLED_RST_Pin  //低电平

//-----------------OLED端口定义----------------
#define OLED_RS_Clr() OLED_DC_L //DC
#define OLED_RS_Set() OLED_DC_H //DC

#define OLED_RST_Clr() OLED_RST_L //RST
#define OLED_RST_Set() OLED_RST_H //RST

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据
//OLED控制用函数
void OLED_WR_Byte_(u8 dat, u8 cmd);
void OLED_WR_Byte(u8 dat, u8 cmd);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowString(u8 x, u8 y, const u8 *p, u8 size, u8 mode);
void OLED_ShowCH(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_Show_CH(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_Show_CH_String(u8 x, u8 y, const u8 *p, u8 size, u8 mode);

typedef enum
{
    Oled_CS_Enable,
    Oled_CS_Disable,
} Oled_CS_State_List;

typedef struct
{
    void (*cs_init)();
    void (*cs_ctl)(Oled_CS_State_List state);

    bool (*bus_init)();
    bool (*bus_transfer)(uint8_t *Tx, uint8_t *Rx, uint16_t len);
} Oled_Obj_TypeDef;

typedef enum
{
    Oled_Display_Enable = 0,
    Oled_Display_Disable,
} Oled_Enable_State_List;

typedef struct
{
    bool (*init)(Oled_Obj_TypeDef *Obj);
    bool (*state_set)(Oled_Obj_TypeDef *Obj, Oled_Enable_State_List state);
    bool (*clear)(Oled_Obj_TypeDef *Obj);
    bool (*fresh)(Oled_Obj_TypeDef *Obj);
} Oled_GenProcFunc_TypeDef;

#endif
