#ifndef __OLEDFONT_H
#define __OLEDFONT_H

#include <stdint.h>

#define FONT_WIDTH 8

#define FONT_8_WIDTH 8
#define FONT_12_WIDTH 8
#define FONT_16_WIDTH 8

typedef enum
{
    Font_8 = 8,
    Font_12 = 12,
    Font_16 = 16,
} Font_List;

extern const uint8_t oled_asc2_0806[95][7];
extern const uint8_t oled_asc2_1206[95][12];
extern const uint8_t oled_asc2_1608[95][16];

#endif
