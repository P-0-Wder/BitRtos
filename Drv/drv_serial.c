#include "drv_serial.h"
#include "vcp.h"

#define Serial_Default_Baudrate Serial_115200

/* internal virable */
static uint8_t Serial_TX_Buff[DrvSerial_Sum][SERIAL_MAX_RECLEN];
static uint8_t Serial_RX_Buff[DrvSerial_Sum][SERIAL_MAX_RECLEN];

/* internal function */

/* external function */
static bool DrvSerial_Ctl(DrvSerial_Port_List portx, DrvSerial_CMD_List cmd, uint32_t data, uint8_t len);
static bool DrvSerial_Write(uint8_t *data, uint16_t len);

/* external virable */
DrvSerial_GenProcFunc_TypeDef DrvSerial = {
    .ctl = DrvSerial_Ctl,
    .read = NULL,
    .write = DrvSerial_Write,
};

static bool DrvSerial_Ctl(DrvSerial_Port_List portx, DrvSerial_CMD_List cmd, uint32_t data, uint8_t len)
{
    DrvSerial_Config_Typedef *data_tmp = NULL;

    switch ((uint8_t)cmd)
    {
    case DrvSerial_Open:
        if (len != sizeof(DrvSerial_Config_Typedef))
            return;

        data_tmp = (DrvSerial_Config_Typedef *)data;

        if (portx != DrvSerial_Vcp)
        {
            switch ((uint8_t)(data_tmp->mode))
            {
            case DrvSerial_MODE_Normal:
                Serial_IRQ_RX_Init(portx, data_tmp->baudrate,
                                   data_tmp->PreemptionPriority,
                                   data_tmp->SubPriority, Serial_Normal);
                break;

            case DrvSerial_MODE_DMA_Rx:
                Serial_DMA_RX_Init(portx, data_tmp->baudrate,
                                   data_tmp->PreemptionPriority,
                                   data_tmp->SubPriority,
                                   (uint32_t)&Serial_RX_Buff[portx],
                                   SERIAL_MAX_RECLEN, Serial_Normal);
                break;

            case DrvSerial_MODE_DMA_TxRx:
                Serial_DMA_RXTX_Init(portx, data_tmp->baudrate,
                                     data_tmp->PreemptionPriority,
                                     data_tmp->SubPriority,
                                     (uint32_t)&Serial_RX_Buff[portx],
                                     (uint32_t)&Serial_TX_Buff[portx],
                                     SERIAL_MAX_RECLEN, Serial_Normal);
                break;

            default:
                return false;
            }
        }
        else
        {
            VCP_Init();
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
