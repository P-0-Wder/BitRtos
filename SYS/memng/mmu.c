/*
* code modify from freertos
* i can`t create my own memory manager module
* sorry guys maybe one day but not now
* sad story
*/
#include "mmu.h"
#include <string.h>
#include <stdio.h>

static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = 0,
    .total_size = PHY_MEM_SIZE,
    .used_size = 0,
    .init = false,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((aligned(BLOCK_ALIGMENT_SIZE))) __attribute__((at(0x10000000)));

static MemBlock_TypeDef MMU_Start;
static MemBlock_TypeDef *MMU_End = NULL;

/* memory block initial */
static void MMU_Init(void)
{
    MemBlock_TypeDef *FirstFreeBlock;
    uint32_t index = 0;
    MemAddr addr = 0;
    uint8_t *aliged_addr = 0;

    /* init memory state table */
    for (index = 0; index < PHY_MEM_SIZE; index++)
    {
        MMU_Buff[index] = 0;
    }

    /* aligment mmu address */
    addr = (MemAddr)MMU_Buff;

    if (addr & (BLOCK_ALIGMENT_SIZE - 1))
    {
        addr += BLOCK_ALIGMENT_SIZE - addr & (BLOCK_ALIGMENT_SIZE - 1);
        addr &= ~((size_t)(BLOCK_ALIGMENT_SIZE - 1));

        Mem_Monitor.total_size -= addr - (MemAddr)MMU_Buff;
        Mem_Monitor.remain_size = Mem_Monitor.total_size;
    }

    aliged_addr = (uint8_t *)addr;

    MMU_Start.nxt = (void *)aliged_addr;
    MMU_Start.len = 0;

    addr = ((MemAddr)aliged_addr) + Mem_Monitor.total_size;
    addr -= sizeof(MemBlock_TypeDef);
    addr &= ~((MemAddr)(BLOCK_ALIGMENT_SIZE - 1));

    MMU_End = (void *)addr;
    MMU_End->nxt = NULL;
    MMU_End->len = 0;

    FirstFreeBlock = (void *)aliged_addr;
    FirstFreeBlock->len = addr - (size_t)FirstFreeBlock;
    FirstFreeBlock->nxt = MMU_End;
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
    uint32_t req_block_size = 0;
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
            /* illegal parameter input */
            mem_addr = NULL;
        }
        else
        {
            req_block_num = size / BLOCK_ALIGMENT_SIZE;
            if (size % BLOCK_ALIGMENT_SIZE)
            {
                req_block_num++;
            }

            req_block_size = req_block_num * BLOCK_ALIGMENT_SIZE;

            /* match the size of block */

            Mem_Monitor.remain_size -= req_block_size;
            Mem_Monitor.used_size += req_block_size;

            //mem_addr = ;
        }

        __asm("cpsie i");
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
