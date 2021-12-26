#ifndef __VERSION_INTERFACE_H
#define __VERSION_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#pragma pack(1)
typedef struct
{
    uint8_t ID_1;
    uint8_t ID_2;
    uint8_t ID_3;
} VersionFrame_TypeDef;

typedef struct
{
    VersionFrame_TypeDef BootVer;
    VersionFrame_TypeDef OsVer;
    VersionFrame_TypeDef AppVer;
    uint64_t sn;
    char bref[128];
} Version_Info_TypeDef;
#pragma pack()

#endif
