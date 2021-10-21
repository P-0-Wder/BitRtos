#include "mmu.h"

uint8_t Mem_Buff[PHY_MEM_SIZE] __attribute__((aligned(BLOCK_ALIGMENT_SIZE))) __attribute__((at(0x10000000)));

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

void MMU_InsertFreeBlock(MemBlock_TypeDef *pxBlockToInsert)
{
    MemBlock_TypeDef *pxIterator;
    uint8_t *puc;

    for (pxIterator = &MemStart; pxIterator->nxtFree < (void *)pxBlockToInsert; pxIterator = pxIterator->nxtFree)
    {
        /* Nothing to do here, just iterate to the right position. */
    }

    puc = (uint8_t *)pxIterator;
    if ((puc + pxIterator->size) == (uint8_t *)pxBlockToInsert)
    {
        pxIterator->size += pxBlockToInsert->size;
        pxBlockToInsert = pxIterator;
    }

    /* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
    puc = (uint8_t *)pxBlockToInsert;
    if ((puc + pxBlockToInsert->size) == (uint8_t *)pxIterator->nxtFree)
    {
        if (pxIterator->nxtFree != MemEnd)
        {
            /* Form one big block from the two blocks. */
            pxBlockToInsert->size += ((MemBlock_TypeDef *)pxIterator->nxtFree)->size;
            pxBlockToInsert->nxtFree = ((MemBlock_TypeDef *)pxIterator->nxtFree)->nxtFree;
        }
        else
        {
            pxBlockToInsert->nxtFree = MemEnd;
        }
    }
    else
    {
        pxBlockToInsert->nxtFree = pxIterator->nxtFree;
    }

    if (pxIterator != pxBlockToInsert)
    {
        pxIterator->nxtFree = pxBlockToInsert;
    }
}

void *MMU_Molloc(uint16_t size)
{
    MemBlock_TypeDef *PrvFreeBlock = NULL;
    MemBlock_TypeDef *NxtFreeBlock = NULL;
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
            mem_addr = (void *)(((uint8_t *)Block_Tmp->nxtFree) + sizeof(MemBlock_TypeDef));
            PrvFreeBlock->nxtFree = Block_Tmp->nxtFree;

            NxtFreeBlock = (void *)(((uint8_t *)Block_Tmp) + size);
            NxtFreeBlock->size = Block_Tmp->size - size;

            MMU_InsertFreeBlock(NxtFreeBlock);

            Block_Tmp->size = size;
        }
    }

    __asm("cpsie i");

    return mem_addr;
}

void MMU_Free(void *ptr)
{
    uint8_t *puc = (uint8_t *)ptr;
    MemBlock_TypeDef *pxLink;

    if (ptr != NULL)
    {
        /* The memory being freed will have an BlockLink_t structure immediately
		before it. */
        puc -= sizeof(MemBlock_TypeDef);

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = (void *)puc;

        if (pxLink->nxtFree == NULL)
        {
            //pxLink->size &= ~xBlockAllocatedBit;

            __asm("cpsid i");
            {
                /* Add this block to the list of free blocks. */
                Mem_Monitor.remain_size += pxLink->size;

                //traceFREE(pv, pxLink->size);
                MMU_InsertFreeBlock(((MemBlock_TypeDef *)pxLink));
            }
            __asm("cpsie i");
        }
    }
}
