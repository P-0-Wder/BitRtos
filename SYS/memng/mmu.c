#include "mmu.h"

uint8_t Mem_Buff[PHY_MEM_SIZE] __attribute__((align(BLOCK_ALIGMENT_SIZE), at(0x10000000)));

Mem_Monitor_TypeDef Mem_Monitor;

MemBlock_TypeDef MemStart;
MemBlock_TypeDef *MemEnd;

void MMU_Init(void)
{
    MemBlock_TypeDef *FstFreeBlock_tmp = NULL;

    Mem_Monitor.phy_size = PHY_MEM_SIZE;
    Mem_Monitor.used_size = 0;

    for (uint32_t i = 0; i < PHY_MEM_SIZE; i++)
    {
        Mem_Buff[i] = 0;
    }

    MemStart.nxtFree = (void *)Mem_Buff;
    MemStart.size = 0;

    MemEnd = (void *)(&Mem_Buff[PHY_MEM_SIZE] - sizeof(MemBlock_TypeDef));
    MemEnd->nxtFree = NULL;
    MemEnd->size = 0;

    Mem_Monitor.total_size = (MemBlock_Addr)MemEnd - (MemBlock_Addr)MemStart.nxtFree;

    FstFreeBlock_tmp = MemStart.nxtFree;
    FstFreeBlock_tmp->nxtFree = MemEnd;

    Mem_Monitor.total_size -= sizeof(MemBlock_TypeDef);
    FstFreeBlock_tmp->size = Mem_Monitor.total_size;

    Mem_Monitor.remain_size = Mem_Monitor.total_size;

    Mem_Monitor.init = true;
}

void MMU_InsertFreeBlock(MemBlock_TypeDef *blk)
{
}

void *MMU_Molloc(uint16_t size)
{
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
        size += (size % BLOCK_ALIGMENT_SIZE);

        PrvFreeBlock = &MemStart;
        Block_Tmp = MemStart.nxtFree;
        while ((Block_Tmp->size < size) && (Block_Tmp->nxtFree != NULL))
        {
            PrvFreeBlock = Block_Tmp;
            Block_Tmp = Block_Tmp->nxtFree;
        }

        if ((Block_Tmp != MemEnd) && (Block_Tmp != NULL))
        {
            mem_addr = (void *)Block_Tmp->nxtFree;
        }
    }

    __asm("cpsie i");

    return mem_addr;
}
