#ifndef __SRVOLED_H
#define __SRVOLED_H

#include <stdint.h>

typedef struct
{
    uint8_t width;
    uint8_t height;
} SrvOled_DspRange;

typedef struct
{
    bool (*init)(void);
    bool (*fresh)(uint8_t **bit_map);
    SrvOled_DspRange (*get_range)(void);
} SrvOled_TypeDef;

extern SrvOled_TypeDef SrvOled;
#endif
