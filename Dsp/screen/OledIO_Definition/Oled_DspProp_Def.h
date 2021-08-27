#ifndef __OLED_DSPPROP_DEF_H
#define __OLED_DSPPROP_DEF_H

typedef enum
{
    Oled_RoutateLeft_90deg = 0,
    Oled_RoutateLeft_180deg,

    Oled_RoutateRight_90deg,
    Oled_RoutateRight_180deg,
} Oled_Routate_Direction_Def;

typedef enum
{
    Oled_MirrorX,
    Oled_MirrorY,
} Oled_Mirror_Direction_Def;

typedef enum
{
    Oled_BrightLevel_1,
    Oled_BrightLevel_2,
    Oled_BrightLevel_3,
} Oled_Bright_Level;

#endif
