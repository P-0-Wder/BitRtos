/*
* copy from freeRTOS
* coder : 8_B!T0
*/
#include "mmu.h"

#pragma pack(BLOCK_ALIGMENT_SIZE)
uint8_t Mem_Buff[PHY_MEM_SIZE] __attribute__((section(".ccmram")));
#pragma pack()

Mem_Monitor_TypeDef Mem_Monitor;
MemBlock_TypeDef MemStart;
MemBlock_TypeDef *MemEnd;
uint8_t trace;

static void MMU_InsertFreeBlock(MemBlock_TypeDef *pxBlockToInsert);
static void MMU_Init(void);

Mem_Monitor_TypeDef MMU_Get_Monitor(void)
{
    MemBlock_TypeDef *pxIterator;

    /* comput freeblock number */
    for (pxIterator = &MemStart; pxIterator->nxtFree != MemEnd; pxIterator = pxIterator->nxtFree)
    {
        Mem_Monitor.FreeBlock_Num++;
    }

    return Mem_Monitor;
}

static void MMU_Init(void)
{
    MemBlock_TypeDef *FstFreeBlock_tmp = NULL;

    Mem_Monitor.phy_size = PHY_MEM_SIZE;
    Mem_Monitor.used_size = 0;
    Mem_Monitor.FreeBlock_Num = 1;

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

void *MMU_Malloc(uint16_t size)
{
    MemBlock_TypeDef *PrvFreeBlock = NULL;
    MemBlock_TypeDef *NxtFreeBlock = NULL;
    MemBlock_TypeDef *Block_Tmp = NULL;
    void *mem_addr = NULL;

    trace = 1;

    __asm("cpsid i");

    if (!Mem_Monitor.init)
    {
        MMU_Init();
    }

    size += sizeof(MemBlock_TypeDef);

    if (size < Mem_Monitor.remain_size)
    {
        /* aligment request byte number */
        size += (size % BLOCK_ALIGMENT_SIZE);

        PrvFreeBlock = &MemStart;
        Block_Tmp = MemStart.nxtFree;

        while ((Block_Tmp->size < size) && (Block_Tmp->nxtFree != NULL))
        {
            PrvFreeBlock = Block_Tmp;

            if (((uint32_t)Block_Tmp->nxtFree & 0xF0000000) == (uint32_t)Mem_Buff)
            {
                Block_Tmp = Block_Tmp->nxtFree;
            }
        }

        if ((((uint32_t)Block_Tmp & 0xF0000000) == (uint32_t)Mem_Buff) && (Block_Tmp != MemEnd))
        {
            Mem_Monitor.req_t++;

            mem_addr = (void *)(((uint8_t *)Block_Tmp) + sizeof(MemBlock_TypeDef));

            PrvFreeBlock->nxtFree = Block_Tmp->nxtFree;

            if ((Block_Tmp->size - size) > MINIMUM_BLOCK_SIZE)
            {
                NxtFreeBlock = (void *)(((uint8_t *)Block_Tmp) + size);
                NxtFreeBlock->size = Block_Tmp->size - size - sizeof(MemBlock_TypeDef);
                Block_Tmp->size = size;
            }

            Mem_Monitor.remain_size -= size;
            Mem_Monitor.used_size += size;

            /* bug  here? */
            MMU_InsertFreeBlock(NxtFreeBlock);

            /* maybe create bug here */
            Block_Tmp->nxtFree = NULL;
        }
    }

    __asm("cpsie i");

    Mem_Monitor.FreeBlock = &MemStart;

    return mem_addr;
}

void MMU_Free(void *ptr)
{
    uint8_t *puc = (uint8_t *)ptr;
    MemBlock_TypeDef *pxLink;

    trace = 2;

    if (((uint32_t)ptr & 0xF0000000) != (uint32_t)Mem_Buff)
    {
        while (1)
            ;
    }
    __asm("cpsid i");
    if (ptr != NULL)
    {
        Mem_Monitor.fre_t++;

        /* The memory being freed will have an BlockLink_t structure immediately
		before it. */
        puc -= sizeof(MemBlock_TypeDef);

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = (void *)puc;

        if (pxLink->nxtFree == NULL)
        {
            //pxLink->size &= ~xBlockAllocatedBit;

            /* Add this block to the list of free blocks. */
            Mem_Monitor.used_size -= pxLink->size;
            Mem_Monitor.remain_size += pxLink->size;

            if (Mem_Monitor.remain_size > Mem_Monitor.total_size)
            {
                while (1)
                    ;
            }

            //traceFREE(pv, pxLink->size);
            MMU_InsertFreeBlock(((MemBlock_TypeDef *)pxLink));
        }
    }
    __asm("cpsie i");

    ptr = NULL;
}

static void MMU_InsertFreeBlock(MemBlock_TypeDef *pxBlockToInsert)
{
    MemBlock_TypeDef *pxIterator;
    uint8_t *puc;

    /* Iterate through the list until a block is found that has a higher address
     * than the block being inserted. */
    for (pxIterator = &MemStart; pxIterator->nxtFree < pxBlockToInsert; pxIterator = pxIterator->nxtFree)
    {
        /* Nothing to do here, just iterate to the right position. */
    }

    if (pxIterator == NULL)
        return;

    puc = (uint8_t *)pxIterator;

    if ((puc != (uint8_t *)&MemStart) && ((uint32_t)puc & 0xF0000000) != (uint32_t)Mem_Buff)
    {
        while (1)
            ;
    }

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
            pxBlockToInsert->size += pxIterator->nxtFree->size;
            pxBlockToInsert->nxtFree = pxIterator->nxtFree->nxtFree;
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
