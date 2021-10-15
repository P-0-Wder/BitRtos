#include "mmu.h"
#include <string.h>
#include <stdio.h>

static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = PHY_MEM_SIZE,
    .total_size = PHY_MEM_SIZE,
    .table_size = MEM_ALLOC_TABLE_SIZE,
    .used_size = 0,
    .init = false,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((aligned(BLOCK_ALIGMENT_SIZE)));
static uint8_t MMU_StateTable[MEM_ALLOC_TABLE_SIZE];

static void MMU_Trim(void)
{
}

/* memory block initial */
static void MMU_Init(void)
{
    /* init memory state table */
    for (uint32_t index = 0; index < MEM_ALLOC_TABLE_SIZE; index++)
    {
        MMU_StateTable[index] = 0;
    }
}

/* memory manager unit malloc */
void *MMU_Malloc(uint32_t size)
{
    uint32_t req_block_num = 0;
    void *mem_addr;

    if (!Mem_Monitor.init)
    {
        MMU_Init();

        Mem_Monitor.init = true;
    }

    if (Mem_Monitor.init)
    {
        __asm("cpsid i");

        if ((size > Mem_Monitor.remain_size) || (size == 0))
        {
            return 0;
        }
        else
        {
            req_block_num = size / BLOCK_ALIGMENT_SIZE;
            if (size % BLOCK_ALIGMENT_SIZE)
            {
                req_block_num++;
            }

            Mem_Monitor.remain_size -= req_block_num * BLOCK_ALIGMENT_SIZE;
            Mem_Monitor.used_size += req_block_num * BLOCK_ALIGMENT_SIZE;
        }

        __asm("cpsie i");
        return mem_addr;
    }

    return 0;
}

/* memory manager unit free */
void MMU_Free(void *ptr)
{
    __asm("cpsid i");

    __asm("cpsie i");
}

Mem_Monitor_TypeDef MMU_Info(void)
{
    return Mem_Monitor;
}
