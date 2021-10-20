#include "mmu.h"

uint8_t Mem_Buff[PHY_MEM_SIZE] __attribute__((align(BLOCK_ALIGMENT_SIZE), at(0x10000000)));

Mem_Monitor_TypeDef Mem_Monitor;
MemBlock_TypeDef MemStart;
MemBlock_TypeDef *MemEnd;

void MMU_Init(void)
{
    Mem_Monitor.phy_size = PHY_MEM_SIZE;
    Mem_Monitor.used_size = 0;

    for (uint16_t i = 0; i < PHY_MEM_SIZE; i++)
    {
        Mem_Buff[i] = 0;
    }

    Mem_Monitor.init = true;
}

void MMU_InsertFreeBlock(MemBlock_TypeDef *blk)
{
}

void *MMU_Molloc(uint16_t size)
{
    void *mem_addr;

    if (!Mem_Monitor.init)
    {
        MMU_Init();
    }

    __asm("cpsid i");

    if (size <= Mem_Monitor.remain_size)
    {
    }

    __asm("cpsie i");

    return mem_addr;
}
