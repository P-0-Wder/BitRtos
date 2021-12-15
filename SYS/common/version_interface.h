#ifndef __VERSION_INTERFACE_H
#define __VERSION_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define VERSION_MEM_SPACE 256

#pragma pack(1)
typedef struct
{
    uint8_t base_id;
    uint8_t mid_id;
    uint8_t cur_id;
} VerBlock_TypeDef;

typedef struct
{
    VerBlock_TypeDef BootVer;
    VerBlock_TypeDef OsVer;
    VerBlock_TypeDef AppVer;
    uint64_t sn;
    char bref[128];
} Version_Info_TypeDef;
#pragma pack()

#endif
