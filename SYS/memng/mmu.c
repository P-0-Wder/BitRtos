#include "mmu.h"
#include <string.h>
#include <stdio.h>

/* internal variable */
static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = PHY_MEM_SIZE,
    .total_size = PHY_MEM_SIZE,
    .used_size = 0,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((__align(32)));

static MMU_Trim(void)
{
}

void *MMU_Malloc(uint32_t size)
{
}

void MMU_Free(void *ptr)
{
}

Mem_Monitor_TypeDef MMU_Info(void)
{
    return Mem_Monitor;
}
