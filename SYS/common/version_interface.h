#ifndef __VERSION_INTERFACE_H
#define __VERSION_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BitRTOS_Version
#define App_Version

#pragma pack(1)
typedef struct
{
    uint8_t Ver[3];
    uint64_t sn;
    char bref[128];
} Version_Info_TypeDef;
#pragma pack()

#endif
