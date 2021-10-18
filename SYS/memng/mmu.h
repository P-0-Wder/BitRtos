#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_ALIGMENT_SIZE 8
#define BLOCK_BORDER_SIZE 4
#define PHY_MEM_SIZE 64 * 1024

typedef uint32_t *MemSize_t;
typedef uint32_t *MemAddr;

typedef struct
{
    MemSize_t used_size;
    MemSize_t remain_size;
    MemSize_t total_size;

    bool init;
} Mem_Monitor_TypeDef;

/* memory block structure  */
typedef struct
{
    void *nxt;
    uint32_t len;
} MemBlock_TypeDef;

#endif
