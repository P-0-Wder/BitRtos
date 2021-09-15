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
    switch ((uint8_t)cmd)
    {
    case DrvSerial_Open:
        if (len != sizeof(DrvSerial_Config_Typedef))
            return;

        ((DrvSerial_Config_Typedef *)data)->port = portx;

        if (portx != DrvSerial_Vcp)
        {
            switch ((uint8_t)(((DrvSerial_Config_Typedef *)data)->mode))
            {
            case DrvSerial_MODE_Normal:
                Serial_IRQ_RX_Init(portx, ((DrvSerial_Config_Typedef *)data)->baudrate,
                                   ((DrvSerial_Config_Typedef *)data)->PreemptionPriority,
                                   ((DrvSerial_Config_Typedef *)data)->SubPriority, Serial_Normal);

                Serial_Set_IRQ_Callback(portx, ((DrvSerial_Config_Typedef *)data)->Irq_Callback);

                ((DrvSerial_Config_Typedef *)data)->inuse = true;
                break;

            case DrvSerial_MODE_DMA_Rx:
                Serial_DMA_RX_Init(portx, ((DrvSerial_Config_Typedef *)data)->baudrate,
                                   ((DrvSerial_Config_Typedef *)data)->PreemptionPriority,
                                   ((DrvSerial_Config_Typedef *)data)->SubPriority,
                                   (uint32_t)&Serial_RX_Buff[portx],
                                   SERIAL_MAX_RECLEN, Serial_Normal);

                Serial_Set_IRQ_Callback(portx, ((DrvSerial_Config_Typedef *)data)->Irq_Callback);

                ((DrvSerial_Config_Typedef *)data)->inuse = true;
                break;

            case DrvSerial_MODE_DMA_TxRx:
                Serial_DMA_RXTX_Init(portx, ((DrvSerial_Config_Typedef *)data)->baudrate,
                                     ((DrvSerial_Config_Typedef *)data)->PreemptionPriority,
                                     ((DrvSerial_Config_Typedef *)data)->SubPriority,
                                     (uint32_t)&Serial_RX_Buff[portx],
                                     (uint32_t)&Serial_TX_Buff[portx],
                                     SERIAL_MAX_RECLEN, Serial_Normal);

                Serial_Set_IRQ_Callback(portx, ((DrvSerial_Config_Typedef *)data)->Irq_Callback);
                Serial_Set_DMAIRQ_Callback(portx, ((DrvSerial_Config_Typedef *)data)->DmaIrq_Callback);

                ((DrvSerial_Config_Typedef *)data)->inuse = true;
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
        Serial_Deinit(portx);
        ((DrvSerial_Config_Typedef *)data)->port = DrvSerial_None;
        ((DrvSerial_Config_Typedef *)data)->mode = DrvSerial_MODE_None;
        ((DrvSerial_Config_Typedef *)data)->PreemptionPriority = 0;
        ((DrvSerial_Config_Typedef *)data)->SubPriority = 0;
        ((DrvSerial_Config_Typedef *)data)->baudrate = 0;
        ((DrvSerial_Config_Typedef *)data)->DmaIrq_Callback = NULL;
        ((DrvSerial_Config_Typedef *)data)->inuse = false;
        ((DrvSerial_Config_Typedef *)data)->Irq_Callback = NULL;
        break;

    case DrvSerial_Set_TxIRQCallback:
        break;

    case DrvSerial_Set_RxIRQCallback:
        break;

    default:
        return false;
    }

    return true;
}

static bool DrvSerial_Write(uint8_t *data, uint16_t len)
{
}
