#include "tm_stm32f4_usb_vcp.h"
#include "vcp.h"

int __io_putchar(int ch)
{
    return ch;
}

int _write(int file, char *ptr, int len)
{
    //Serial_SendBuff(Serial_Port[RADIO_PORT], ptr, len);
    VCP_Send(ptr, len);
    return len;
}

TM_USB_VCP_Result VCP_Init(void)
{
    return TM_USB_VCP_Init();
}

TM_USB_VCP_Result VCP_Send(uint8_t *DataArray, uint32_t Length)
{
    TM_USB_VCP_Result state_temp;

    state_temp = TM_USB_VCP_Send(DataArray, Length);

    return state_temp;
}

TM_USB_VCP_Result VCP_GetConnectState(void)
{
    return TM_USB_VCP_GetStatus();
}