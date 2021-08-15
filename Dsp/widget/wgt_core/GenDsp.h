#ifndef __GENDSP_H
#define __GENDSP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "oledfont.h"

#define MAX_DSP_NUM_LEN 10

typedef Font_List GenFont_List;

typedef enum
{
    Ver_Dir = 0,
    Hor_Dir,
} RollDirection_List;

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
    void (*draw_point)(uint8_t **map, uint8_t x, uint8_t y, bool set);
    void (*draw_circle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_size);
    void (*draw_char)(GenFont_List font, uint8_t **map, char c, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_str)(GenFont_List font, uint8_t **map, char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint8_t **map, uint32_t num, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
    void (*draw_line)(uint8_t **map, uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_width);
    bool (*init_rollbar)(RollBar_TypeDef *Obj, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t range, RollDirection_List dir, bool frame);
    void (*draw_process_bar)(uint8_t **map, uint8_t percent, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*draw_Roller_bar)(uint8_t **map, RollBar_TypeDef *bar, int8_t move_dis);
    void (*draw_img)(uint8_t **map);
} GeneralDispalyProc_TypeDef;

extern GeneralDispalyProc_TypeDef GenDsp_Interface;

#endif
