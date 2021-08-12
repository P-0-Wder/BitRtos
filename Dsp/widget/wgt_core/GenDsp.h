#ifndef __GENDSP_H
#define __GENDSP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "oledfont.h"

typedef Font_List GenFont_List;

typedef enum
{
    Ver_Dir = 0,
    Hor_Dir,
} RollDirection_List;

typedef enum
{
    Vertical_Line = 0,
    Horizon_Line,
} LineDirection_Type;

typedef struct
{
    bool frame;
    uint8_t width;
    uint8_t height;

    uint8_t x;
    uint8_t y;

    uint8_t range;

    RollDirection_List Dir;
} RollBar_TypeDef;

typedef struct
{
    bool (*init_rollbar)(RollBar_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t range, RollDirection_List dir, bool frame);

    void (*set_font)(GenFont_List index);
    void (*draw_char)(char c, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_str)(char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(uint32_t num, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_process_bar)(uint8_t pct, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*draw_point)(uint8_t x, uint8_t y);
    void (*draw_line)(uint8_t x, uint8_t y, uint8_t len, uint8_t len_size, LineDirection_Type type);
    void (*draw_Roller_bar)(RollBar_TypeDef *bar, int8_t move_dis);
    void (*draw_rectangle)();
    void (*draw_circle)();
    void (*draw_img)();
} GeneralDispalyProc_TypeDef;

#endif
