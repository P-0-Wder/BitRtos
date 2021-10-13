#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_LENGTH
#define BLOCK_SIZE

#define PHY_MEM_SIZE 64 * 1024

typedef uint32_t MemSize_t;

typedef struct
{
    MemSize_t used_size;
    MemSize_t remain_size;
    MemSize_t total_size;
} Mem_Monitor_TypeDef;

typedef enum
{
    Mem_Init = 0,
    Mem_Init_Error,
    Mem_None_Opr,
    Men_Molloc,
    Mem_Molloc_Successd,
    Men_Molloc_Failed,
    Mem_Free,
    Mem_Free_Successd,
    Mem_Free_Failed,
} Mem_Opr_State_List;

typedef struct
{
    uint32_t page;
    uint32_t offset;
} VirtualMem_TypeDef;

#endif
