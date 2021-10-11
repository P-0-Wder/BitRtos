#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PHY_MEM_SIZE 64 * 1024

typedef struct
{
    uint32_t used_size;
    uint32_t remain_size;
    uint32_t total_size;
} Mem_Monitor_TypeDef;

#endif
