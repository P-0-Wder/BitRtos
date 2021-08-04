#include "sys.h"

void WFI_SET(void)
{
	__ASM(
		"WFI");
}

void INTX_DISABLE(void)
{
	__ASM("CPSID I");
	__ASM("BX LR");
}

void INTX_ENABLE(void)
{
	__ASM("CPSIE I");
	__ASM("BX LR");
}

void MSR_MSP(u32 addr)
{
	__ASM("MSR MSP, r0");
	__ASM("BX r14");
}
