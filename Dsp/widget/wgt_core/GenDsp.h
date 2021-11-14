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
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t heigh;
} GenDsp_DspArea_Limit_Range;

typedef struct
{
    void (*set_range)(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*draw_point)(uint8_t **map, uint8_t x, uint8_t y, bool set);
    void (*draw_circle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t radius, uint8_t line_size);
    bool (*draw_char)(GenFont_List font, uint8_t **map, char c, uint8_t x, uint8_t y, bool col_inv);
    bool (*draw_str)(GenFont_List font, uint8_t **map, char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(GenFont_List font, uint8_t **map, uint32_t num, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t line_size);
    void (*draw_radius_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width);
    void (*draw_line)(uint8_t **map, uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, uint8_t line_width);
    void (*draw_img)(uint8_t **map);

    void (*fill_circle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t radius);
    void (*fill_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void (*fill_radius_rectangle)(uint8_t **map, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius);

    uint8_t (*str_len)(GenFont_List font, char *str);
} GeneralDispalyProc_TypeDef;

extern GeneralDispalyProc_TypeDef GenDsp_Interface;

#endif
