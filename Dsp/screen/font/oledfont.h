#ifndef __OLEDFONT_H
#define __OLEDFONT_H

#include <stdint.h>

#define FONT_WIDTH 8

typedef enum
{
    Font_12 = 12,
    Font_16 = 16,
} Font_List;

extern const uint8_t oled_asc2_1206[95][12];
extern const uint8_t oled_asc2_1608[95][16];

#endif
