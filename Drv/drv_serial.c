#include "drv_serial.h"
#include "vcp.h"

#define Serial_Default_Baudrate Serial_115200

/* internal virable */
static uint8_t Serial_TX_Buff[DrvSerial_Sum][SERIAL_MAX_RECLEN];
static uint8_t Serial_RX_Buff[DrvSerial_Sum][SERIAL_MAX_RECLEN];

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
                Serial_IRQ_RX_Init(portx, Serial_Default_Baudrate);
                break;

            case DrvSerial_MODE_DMA_Rx:
                Serial_DMA_RX_Init(portx, Serial_Default_Baudrate);
                break;

            case DrvSerial_MODE_DMA_TxRx:
                Serial_DMA_RXTX_Init(portx, Serial_Default_Baudrate);
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
