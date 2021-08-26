#ifndef __SRVOLED_H
#define __SRVOLED_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t width;
    uint8_t height;
} SrvOled_DspRange;

typedef enum
{
    Oled_RoutateLeft_90deg = 0,
    Oled_RoutateLeft_180deg,

    Oled_RoutateRight_90deg,
    Oled_RoutateRight_180deg,
} SrvOled_Routate_Direction_Def;

typedef enum
{
    Oled_MirrorX,
    Oled_MirrorY,
} SrvOled_Mirror_Direction_Def;

typedef enum
{
    Oled_BrightLevel_1,
    Oled_BrightLevel_2,
    Oled_BrightLevel_3,
} SrvOled_Bright_Level;

typedef struct
{
    bool (*init)(void);
    bool (*fresh)(uint8_t **bit_map);
    bool (*routate)(SrvOled_Routate_Direction_Def routate_dir);
    bool (*mirror)(SrvOled_Mirror_Direction_Def mirror_dir);
    bool (*bright)(SrvOled_Bright_Level level);
    SrvOled_DspRange (*get_range)(void);
} SrvOled_TypeDef;

extern SrvOled_TypeDef SrvOled;
#endif
