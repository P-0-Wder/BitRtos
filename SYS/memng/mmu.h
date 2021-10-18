#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_ALIGMENT_SIZE 8
#define BLOCK_BORDER_SIZE 4
#define PHY_MEM_SIZE 62 * 1024

#define MEM_ALLOC_TABLE_SIZE PHY_MEM_SIZE / BLOCK_ALIGMENT_SIZE

typedef uint32_t *MemSize_t;
typedef uint32_t *MemAddr;

typedef struct
{
    MemSize_t used_size;
    MemSize_t remain_size;
    MemSize_t total_size;
    MemSize_t table_size;

    bool init;
} Mem_Monitor_TypeDef;

/* memory block structure  */
typedef struct
{
    void *bgn;
    void *end;
    void *nxt;

    uint32_t len;
} MemBlock_TypeDef;

typedef struct
{
    uint32_t page;
    uint32_t offset;
} VirtualMem_TypeDef;

#endif
