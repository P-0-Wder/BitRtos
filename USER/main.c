#include "stddef.h"
#include "vcp.h"
#include "stdio.h"

int main(void)
{
	RCC_ClocksTypeDef SysFrq;
	RCC_GetClocksFreq(&SysFrq);

	return 0;
}
