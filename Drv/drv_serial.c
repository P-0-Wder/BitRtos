#include "drv_serial.h"
#include "vcp.h"

/* internal virable */

/* internal function */

/* external function */
static bool DrvSerial_Ctl(DrvSerial_Port_List portx, DrvSerial_CMD_List cmd, uint32_t data);
static bool DrvSerial_Write(uint8_t *data, uint16_t len);

/* external virable */
DrvSerial_GenProcFunc_TypeDef DrvSerial = {
    .ctl = DrvSerial_Ctl,
    .read = NULL,
    .write = DrvSerial_Write,
};

static void DrvSerial_GetInstence(DrvSerial_Port_List portx)
{
}

static bool DrvSerial_Ctl(DrvSerial_Port_List portx, DrvSerial_CMD_List cmd, uint32_t data)
{
    switch ((uint8_t)cmd)
    {
    case DrvSerial_Open:
        if (portx != DrvSerial_Vcp)
        {
            switch ((uint8_t)data)
            {
            case DrvSerial_MODE_Normal:
                break;

            case DrvSerial_MODE_DMA_Rx:
                break;

            case DrvSerial_MODE_DMA_Tx:
                break;

            case DrvSerial_MODE_DMA_TxRx:
                break;

            default:
                return false;
            }
        }
        else
        {
        }
        break;

    case DrvSerial_Close:
        break;

    case DrvSerial_Set_Baudrate:
        break;

    case DrvSerial_Set_IRQCallback:
        break;

    default:
        return false;
    }

    return true;
}

static bool DrvSerial_Write(uint8_t *data, uint16_t len)
{
}
