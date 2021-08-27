#ifndef __SRVOLED_H
#define __SRVOLED_H

#include <stdint.h>
#include <stdbool.h>
#include "Oled_DspProp_Def.h"

typedef struct
{
    uint8_t width;
    uint8_t height;
} SrvOled_DspRange;

typedef struct
{
    bool (*init)(void);
    bool (*fresh)(uint8_t **bit_map);
    bool (*routate)(Oled_Routate_Direction_Def routate_dir);
    bool (*mirror)(Oled_Mirror_Direction_Def mirror_dir);
    bool (*bright)(Oled_Bright_Level level);
    SrvOled_DspRange (*get_range)(void);
} SrvOled_TypeDef;

extern SrvOled_TypeDef SrvOled;
#endif
