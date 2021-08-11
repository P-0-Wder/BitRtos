#ifndef __GENDSP_H
#define __GENDSP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "oledfont.h"

typedef Font_List GenFont_List;

typedef struct
{
    void (*set_font)(GenFont_List index);
    void (*draw_point)(uint8_t x, uint8_t y);
    void (*draw_char)(char c, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_str)(char *str, uint8_t x, uint8_t y, bool col_inv);
    void (*draw_num)(uint32_t num, uint8_t x, uint8_t y, bool col_inv);
} GeneralDispalyProc_TypeDef;

#endif
