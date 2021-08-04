#ifndef __CRC16_H
#define __CRC16_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    uint16_t calc_crc16(const uint8_t *buf, const uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
