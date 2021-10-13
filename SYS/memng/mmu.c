#include "mmu.h"
#include <string.h>
#include <stdio.h>

static Mem_Opr_State_List Mem_OprState = Mem_Init;

static Mem_Monitor_TypeDef Mem_Monitor = {
    .remain_size = PHY_MEM_SIZE,
    .total_size = PHY_MEM_SIZE,
    .used_size = 0,
};

static uint8_t MMU_Buff[PHY_MEM_SIZE] __attribute__((__align(32)));

static void MMU_Trim(void)
{
}

/* memory block initial */
static bool MMU_InitBlock(void)
{

    return true;
}

static uint8_t MMU_ErrorProc(Mem_Opr_State_List *state)
{
    uint8_t state = 0;

    switch (*(uint8_t *)state)
    {

    case Mem_Init_Error:
        break;

    case Men_Molloc_Failed:
        break;

    case Mem_Free_Failed:
        break;

    default:
        break;
    }

    return state;
}

/* memory manager unit malloc */
void *MMU_Malloc(uint32_t size)
{
    void *mem_addr;

    while (true)
    {
        switch ((uint8_t)Mem_OprState)
        {
        case Mem_Init:
            if (MMU_InitBlock())
            {
                Mem_OprState = Men_Molloc;
            }
            else
                Mem_OprState = Mem_Init_Error;
            break;

        case Men_Molloc:
            __asm("cpsid i");

            __asm("cpsie i");
            return mem_addr;

        default:
            /* TO DO Error Process */
            MMU_ErrorProc(&Mem_OprState);
            return 0;
        }
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
