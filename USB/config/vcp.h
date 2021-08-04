#ifndef __VCP_H
#define __VCP_H

#include <stdint.h>
#include "tm_stm32f4_usb_vcp.h"

TM_USB_VCP_Result VCP_Init(void);
TM_USB_VCP_Result VCP_Send(uint8_t *DataArray, uint32_t Length);
TM_USB_VCP_Result VCP_GetConnectState(void);

#endif
