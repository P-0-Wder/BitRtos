#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_ALIGMENT_SIZE 4
#define PHY_MEM_SIZE 64 * 1024

#define MINIMUM_BLOCK_SIZE BLOCK_ALIGMENT_SIZE * 2

typedef uint32_t MemSize_t;
typedef uint32_t MemBlock_Addr;

/* memory request type */
typedef enum
{
    MemReq_OS = BLOCK_ALIGMENT_SIZE - 1,
    MemReq_Task = 0,
} Mem_ReqType_List;

/* memory block structure  */
typedef struct _MemBlock_TypeDef
{
    /* pointer to next free memory block */
    struct _MemBlock_TypeDef *nxtFree;

    /* request size */
    uint16_t size;
} MemBlock_TypeDef;

typedef struct
{
    MemSize_t phy_size;
    MemSize_t used_size;
    MemSize_t remain_size;
    MemSize_t total_size;

    MemBlock_TypeDef *FreeBlock;
    uint16_t FreeBlock_Num;

    uint32_t req_t;
    uint32_t fre_t;

    bool init;
} Mem_Monitor_TypeDef;

void *MMU_Malloc(uint16_t size);
void MMU_Free(void *ptr);

#endif
