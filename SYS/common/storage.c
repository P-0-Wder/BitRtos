#include "storage.h"
#include "periph_flash.h"
#include "crc16.h"

typedef Storage_ErrorCode_List Stor_error;

static bool Storage_Init(void)
{
}

static bool Storage_BaseWrite(uint32_t addr, uint8_t *data, uint16_t len)
{
    return true;
}

static bool Storage_BaseRead(uint32_t addr, uint8_t *data, uint16_t len)
{
    return true;
}

static Stor_error Stroage_Get(uint8_t sector, uint8_t ID, uint8_t *data, uint16_t len)
{
    return Storage_NoError;
}

static Stor_error Storage_Set(uint8_t sector, uint8_t ID, uint8_t *data, uint16_t len)
{
    return Storage_NoError;
}

static Stor_error Storage_GetUserParam(uint8_t ID, uint8_t *data, int16_t len)
{
    return Storage_NoError;
}

static Stor_error Storage_SetUserParam(uint8_t ID, uint8_t *data, uint16_t len)
{
    return Storage_NoError;
}

static Stor_error Storage_GetSystemParam(uint8_t ID, uint8_t *data, uint16_t len)
{
    return Storage_NoError;
}

static Stor_error Storage_SetSysetmParam(uint8_t ID, uint8_t *data, uint16_t len)
{
    return Storage_NoError;
}