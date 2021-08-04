
#ifndef __FAULT_CHECK_H
#define __FAULT_CHECK_H

#include "stdint.h"
#include "stm32f4xx.h"

#define DEBUGEVT_FAULT_SET (1 << 31)
#define FORCED___FAULT_SET (1 << 30)
#define VECTTBL__FAULT_SET (1 << 1)

#define UFSR
#define BFSR
#define MFSR

typedef enum
{
    vecttbl_bit = 1,
    forced_bit = 30,
    debugevt_bit = 31,
} fault_bit_list;

typedef union
{
    uint8_t value;

    struct
    {
        uint8_t debugevt : 1;
        uint8_t forced : 1;
        uint8_t vecttbl : 1;
    } reg;

} fault_item;

typedef union
{
    uint32_t value;

    struct
    {
        struct
        {
            uint8_t RESERVED : 4;

            uint8_t IACCVIOL : 1;
            uint8_t DACCVIOL : 1;
            uint8_t MUNSTKERR : 1;
            uint8_t MSTKERR : 1;
        } mfsr;

        struct
        {
            uint8_t RESERVED : 3;

            uint8_t IBUSERR : 1;
            uint8_t PRECISERR : 1;
            uint8_t IMPRECISERR : 1;
            uint8_t UNSTKERR : 1;
            uint8_t STKERR : 1;
        } bfsr;

        struct
        {
            uint16_t RESERVED : 10;

            uint8_t UNDEFINSTR : 1;
            uint8_t INVSTATE : 1;
            uint8_t INVPC : 1;
            uint8_t NOCP : 1;
            uint8_t UNALIGNED : 1;
            uint8_t DIVBYZERO : 1;
        } ubsr;

    } reg;

} cfsr_item;

fault_item Get_FaultReg_Value(void);

#endif
