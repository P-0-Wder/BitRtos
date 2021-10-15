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

static void MMU_Trim(void)
{
}

/* memory block initial */
static bool MMU_InitBlock(void)
{

    return true;
}

/* memory manager unit malloc */
void *MMU_Malloc(uint32_t size)
{
    void *mem_addr;

    if (!Mem_Monitor.init)
    {
        MMU_InitBlock();

        Mem_Monitor.init = true;
    }

    if (Mem_Monitor.init)
    {
        __asm("cpsid i");

        if (size > Mem_Monitor.remain_size)
        {
            return 0;
        }
        else
        {
            Mem_Monitor.remain_size -= size;
            Mem_Monitor.used_size += size;
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
