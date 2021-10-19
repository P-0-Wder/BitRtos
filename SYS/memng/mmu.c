#include "mmu.h"
#include <string.h>
#include <stdio.h>

static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = PHY_MEM_SIZE,
    .total_size = PHY_MEM_SIZE,
    .used_size = 0,
    .init = false,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((aligned(BLOCK_ALIGMENT_SIZE))) __attribute__((at(0x10000000)));
;

static MemBlock_TypeDef MMU_Start;
static MemBlock_TypeDef MMU_End;

/* memory block initial */
static void MMU_Init(void)
{
    uint32_t index = 0;

    /* init memory state table */
    for (index = 0; index < PHY_MEM_SIZE; index++)
    {
        MMU_Buff[index] = 0;
    }

    MMU_Start.nxt = NULL;
    MMU_Start.len = PHY_MEM_SIZE - BLOCK_BORDER_SIZE * 2;

    MMU_End.nxt = NULL;
    MMU_End.len = 0;
}

static void MMU_UpdateFreeBlock(MemBlock_TypeDef *block)
{
    if (MMU_Start.nxt == NULL)
    {
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
            mem_addr = NULL;
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

        //mem_addr = ;
    }

    return mem_addr;
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
