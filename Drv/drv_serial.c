#include "drv_serial.h"
#include "periph_serial.h"

/* internal virable */

/* internal function */

/* external function */
static bool DrvSerial_Ctl(DrvSerial_CMD_List cmd, uint32_t data);
static bool DrvSerial_Write(uint8_t *data, uint16_t len);

/* external virable */
DrvSerial_GenProcFunc_TypeDef DrvSerial = {
    .ctl = DrvSerial_Ctl,
    .read = NULL,
    .write = DrvSerial_Write,
};

static bool DrvSerial_Ctl(DrvSerial_CMD_List cmd, uint32_t data)
{
}

static bool DrvSerial_Write(uint8_t *data, uint16_t len)
{
}
