#ifndef __OLED_H
#define __OLED_H

#include <stdbool.h>
#include <stdint.h>

//-----------------OLED端口定义----------------
#define OLED_RS_Clr() OLED_DC_L //DC
#define OLED_RS_Set() OLED_DC_H //DC

#define OLED_RST_Clr() OLED_RST_L //RST
#define OLED_RST_Set() OLED_RST_H //RST

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据
//OLED控制用函数
void OLED_WR_Byte_(u8 dat, u8 cmd);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);

typedef enum
{
    Oled_SpiBus = 0,
    Oled_I2CBus,
} Oled_BusInterface_TypeList;

typedef enum
{
    Oled_CS_Enable,
    Oled_CS_Disable,
} Oled_CS_State_List;

typedef enum
{
    Oled_RS_Enable,
    Oled_RS_Disable,
} Oled_RS_State_List;

typedef enum
{
    Oled_DC_Enable,
    Oled_DC_Disable,
} Oled_DC_State_List;

typedef enum
{
    Oled_Write_CMD = 0,
    Oled_Write_Data,
} Oled_Write_Type;

typedef enum
{
    Oled_Display_Enable = 0,
    Oled_Display_Disable,
} Oled_Enable_State_List;

typedef struct
{
    uint8_t Oled_Width;
    uint8_t Oled_Height;

    Oled_BusInterface_TypeList Bus;
    uint8_t BusID;

    void (*cs_init)(void);
    void (*rs_init)(void);
    void (*dc_init)(void);

    void (*cs_ctl)(Oled_CS_State_List state);
    void (*rs_ctl)(Oled_RS_State_List state);
    void (*dc_ctl)(Oled_DC_State_List state);

    bool (*bus_init)(void);
    bool (*bus_transmit)(uint8_t tx_data);
} Oled_Obj_TypeDef;

typedef struct
{
    bool (*init)(Oled_Obj_TypeDef *Obj, uint8_t width, uint8_t height);
    bool (*enable_set)(Oled_Obj_TypeDef *Obj, Oled_Enable_State_List state);
    bool (*fresh)(Oled_Obj_TypeDef *Obj);
} Oled_GenProcFunc_TypeDef;

#endif
