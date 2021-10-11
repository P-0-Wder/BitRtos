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

/* virtual memory manager unit malloc */
void *VMMU_Malloc(uint32_t size)
{
}

/* virtual memory manager unit free */
void VMMU_Free(void *ptr)
{
}

Mem_Monitor_TypeDef VMMU_Info(void)
{
    return Mem_Monitor;
}
