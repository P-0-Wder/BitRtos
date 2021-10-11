#include "mmu.h"
#include <string.h>
#include <stdio.h>

static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = PHY_MEM_SIZE,
    .total_size = PHY_MEM_SIZE,
    .used_size = 0,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((__align(32)));

static void MMU_Trim(void)
{
}

/* memory manager unit malloc */
void *MMU_Malloc(uint32_t size)
{
    __asm("cpsid i");

    __asm("cpsie i");
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
