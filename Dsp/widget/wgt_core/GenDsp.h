#ifndef __GENDSP_H
#define __GENDSP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "oledfont.h"

#define MAX_DSP_NUM_LEN 10

#define DRAW_UPPER_RIGHT 0x01
#define DRAW_UPPER_LEFT 0x02
#define DRAW_LOWER_LEFT 0x04
#define DRAW_LOWER_RIGHT 0x08
#define DRAW_ALL (DRAW_UPPER_RIGHT | DRAW_UPPER_LEFT | DRAW_LOWER_RIGHT | DRAW_LOWER_LEFT)

typedef Font_List GenFont_List;

typedef enum
{
    Ver_Dir = 0,
    Hor_Dir,
} RollDirection_List;

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t width;
    uint8_t heigh;
} GenDsp_DspArea_Limit_Range;

typedef struct
{
    void (*set_range)(int16_t x, int16_t y, uint8_t width, uint8_t height);
    void (*draw_point)(uint8_t **map, int16_t x, int16_t y, bool set);
    void (*draw_circle)(uint8_t **map, int16_t x, int16_t y, uint8_t radius, uint8_t option, bool col_inv);
    bool (*draw_char)(GenFont_List font, uint8_t **map, char c, int16_t x, int16_t y, bool col_inv);
    bool (*draw_str)(GenFont_List font, uint8_t **map, char *str, int16_t x, int16_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint8_t **map, uint32_t num, int16_t x, int16_t y, bool col_inv);
    void (*draw_rectangle)(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_inv);
    void (*draw_radius_rectangle)(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width, bool col_inv);
    void (*draw_line)(uint8_t **map, int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_width, bool col_inv);
    void (*draw_vertical_line)(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
    void (*draw_horizon_line)(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
    void (*draw_img)(uint8_t **map, bool col_inv);

    void (*fill_circle)(uint8_t **map, int16_t x, int16_t y, uint8_t radius, uint8_t option, bool col_inv);
    void (*fill_rectangle)(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv);
    void (*fill_radius_rectangle)(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv);

    uint8_t (*str_len)(GenFont_List font, char *str);
} GeneralDispalyProc_TypeDef;

extern GeneralDispalyProc_TypeDef GenDsp_Interface;

#endif
