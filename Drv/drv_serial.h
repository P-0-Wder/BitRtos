#ifndef DRV_SERIAL_H
#define DRV_SERIAL_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "periph_serial.h"

#define SERIAL_MAX_RECLEN 1024

typedef enum
{
    DrvSerial_1 = Serial_1,
    DrvSerial_2 = Serial_2,
    DrvSerial_3 = Serial_3,
    DrvSerial_6 = Serial_6,
    DrvSerial_Vcp,
    DrvSerial_Sum,
} DrvSerial_Port_List;

typedef enum
{
    DrvSerial_MODE_Normal = 0,
    DrvSerial_MODE_DMA_Rx,
    DrvSerial_MODE_DMA_TxRx,
} DrvSerial_PortMode_List;

typedef enum
{
    DrvSerial_Open = 0,
    DrvSerial_Close,
    DrvSerial_Set_Baudrate,
    DrvSerial_Set_IRQCallback,
} DrvSerial_CMD_List;

typedef struct
{
    bool inuse;
    uint32_t baudrate;
    DrvSerial_Port_List port;
    uint8_t PreemptionPriority;
    uint8_t SubPriority;
    DrvSerial_PortMode_List mode;

    Serial_IRQ_Callback Irq_Callback;
    Serial_DMA_IRQ_Callback DmaIrq_Callback;
} DrvSerial_Config_Typedef;

typedef struct
{
    bool (*ctl)(DrvSerial_Port_List portx, DrvSerial_CMD_List cmd, uint32_t data, uint8_t len);
    bool (*read)(uint8_t *data, uint16_t len);
    bool (*write)(uint8_t *data, uint16_t len);
} DrvSerial_GenProcFunc_TypeDef;

extern DrvSerial_GenProcFunc_TypeDef DrvSerial;

#endif
