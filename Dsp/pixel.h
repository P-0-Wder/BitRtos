#ifndef __PIXEL_H
#define __PIXEL_H

#include <stdint.h>

typedef union
{
    uint8_t val;

    struct
    {
        uint8_t column_7 : 1;
        uint8_t column_6 : 1;
        uint8_t column_5 : 1;
        uint8_t column_4 : 1;
        uint8_t column_3 : 1;
        uint8_t column_2 : 1;
        uint8_t column_1 : 1;
        uint8_t column_0 : 1;
    } Column_section;
} Pixel_Block_TypeDef;

typedef Pixel_Block_TypeDef Widget_Pixel;
typedef Pixel_Block_TypeDef Oled_Pixel_Block_TypeDef;

#endif
