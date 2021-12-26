#include "storage.h"
#include "crc16.h"

typedef int8_t Stor_error;

static bool Storage_Init(void)
{
}

static Stor_error Stroage_Get(uint8_t sector, uint8_t ID, uint8_t *data, uint16_t len)
{
    return true;
}

static Stor_error Storage_Set(uint8_t sector, uint8_t ID, uint8_t *data, uint16_t len)
{
    return true;
}

static Stor_error Storage_GetUserParam()
{
    return true;
}

static Stor_error Storage_SetUserParam()
{
    return true;
}

static Stor_error Storage_GetSystemParam()
{
    return true;
}

static Stor_error Storage_SetSysetmParam()
{
    return true;
}