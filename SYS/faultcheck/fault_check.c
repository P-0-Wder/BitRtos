#include "fault_check.h"

uint32_t hardfault_reg_val = 0;
uint32_t cfsr_reg_val = 0;

cfsr_item Get_CfsrReg_Value(void)
{
    cfsr_item cfsr_item_tmp;

    cfsr_item_tmp.value = 0;

    __ASM(".equ CFSRFAULT_REG, 0xE000ED28");

    __ASM("LDR      R1, =CFSRFAULT_REG");
    __ASM("LDR      R3, =cfsr_reg_val");

    __ASM("LDR      R2, [R1]");
    __ASM("STR      R2, [R3]");

    cfsr_item_tmp.value = cfsr_reg_val;
}

fault_item Get_FaultReg_Value(void)
{
    uint32_t Sp_Val = 0;
    fault_item fault_item_tmp;
    volatile cfsr_item cfsr_val;

    cfsr_val.value = 0;
    fault_item_tmp.value = 0;

    __ASM(".equ HARDFAULT_REG, 0xE000ED2C");

    __ASM("LDR      R1, =HARDFAULT_REG");
    __ASM("LDR      R3, =hardfault_reg_val");

    __ASM("LDR      R2, [R1]");
    __ASM("STR      R2, [R3]");

    if (!__get_PSP())
    {
        Sp_Val = __get_PSP();
    }
    else
    {
        Sp_Val = __get_MSP();
    }

    if (hardfault_reg_val & DEBUGEVT_FAULT_SET)
    {
        fault_item_tmp.reg.debugevt = 1;
    }

    if (hardfault_reg_val & FORCED___FAULT_SET)
    {
        fault_item_tmp.reg.forced = 1;

        //check cfsr
        cfsr_val = Get_CfsrReg_Value();
    }

    if (hardfault_reg_val & VECTTBL__FAULT_SET)
    {
        fault_item_tmp.reg.forced = 1;
    }

    return fault_item_tmp;
}
