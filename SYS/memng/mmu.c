#include "mmu.h"
#include "task_manager.h"

uint8_t Mem_Buff[PHY_MEM_SIZE] __attribute__((align(BLOCK_ALIGMENT_SIZE), at(0x10000000)));

Mem_Monitor_TypeDef Mem_Monitor;

MemBlock_TypeDef MemStart;
MemBlock_TypeDef *MemEnd;

void MMU_Init(void)
{
    MemBlock_Addr mem_addr;

    Mem_Monitor.phy_size = PHY_MEM_SIZE;
    Mem_Monitor.used_size = 0;

    for (uint16_t i = 0; i < PHY_MEM_SIZE; i++)
    {
        Mem_Buff[i] = 0;
    }

    MemStart.nxtFree = (void *)Mem_Buff;
    MemStart.size = 0;

    MemEnd = (void *)(&Mem_Buff[PHY_MEM_SIZE] - sizeof(MemBlock_TypeDef));
    MemEnd->nxtFree = NULL;
    MemEnd->size = 0;

    Mem_Monitor.total_size = (MemBlock_Addr)MemEnd - (MemBlock_Addr)MemStart.nxtFree;
    Mem_Monitor.remain_size = Mem_Monitor.total_size;

    Mem_Monitor.init = true;
}

void MMU_InsertFreeBlock(MemBlock_TypeDef *blk)
{
}

void *MMU_Molloc(uint16_t size)
{
    MemBlock_TypeDef *NxtFreeBlock = NULL;
    MemBlock_TypeDef *PrvFreeBlock = NULL;
    MemBlock_TypeDef *Block_Tmp = NULL;
    void *mem_addr = NULL;

    if (!Mem_Monitor.init)
    {
        MMU_Init();
    }

    __asm("cpsid i");

    if (size <= Mem_Monitor.remain_size)
    {
        /* aligment request byte number */
        if (size % BLOCK_ALIGMENT_SIZE)
        {
            size += (size % BLOCK_ALIGMENT_SIZE);
        }

        PrvFreeBlock = &MemStart;
        Block_Tmp = MemStart.nxtFree;
        while ((size < Block_Tmp->size) && (Block_Tmp->nxtFree != NULL))
        {
        }
    }

    __asm("cpsie i");

    return mem_addr;
}
