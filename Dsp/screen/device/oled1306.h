#ifndef __OLED1306_H
#define __OLED1306_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pixel.h"

#define COLUMN_SIZE 128
#define ROW_SIZE 8

#define OLED_MAX_HEIGHT 64
#define OLED_MAX_WIDTH 128

#define OLED_COLUMN_BLOCK_NUM 8

typedef enum
{
    Oled_SpiBus = 0,
    Oled_I2CBus,
} Oled_BusInterface_TypeList;

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

#pragma pack(1)
typedef struct
{
    Oled_BusInterface_TypeList Bus;
    void *BusObj;
    uint8_t BusID;

    void (*rs_init)(void);
    void (*dc_init)(void);

    void (*rs_ctl)(Oled_RS_State_List state);
    void (*dc_ctl)(Oled_DC_State_List state);

    bool (*bus_init)(uint8_t ID);
    bool (*bus_transmit)(uint8_t tx_data);
} Oled_Obj_TypeDef;
#pragma pack()

typedef struct
{
    bool (*init)(Oled_Obj_TypeDef *Obj);
    bool (*enable_set)(Oled_Obj_TypeDef *Obj, Oled_Enable_State_List state);
    bool (*fresh)(Oled_Obj_TypeDef *Obj, uint8_t **map);
    bool (*clear)(Oled_Obj_TypeDef *Obj);
    uint8_t (*get_max_width)(void);
    uint8_t (*get_max_height)(void);
} Oled_GenProcFunc_TypeDef;

extern Oled_GenProcFunc_TypeDef DrvOled;

#endif
