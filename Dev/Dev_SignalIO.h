#ifndef __DEV_SIGNALIO_H
#define __DEV_SIGNALIO_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CHANNEL_NUM 16

typedef struct
{
    /* data */
} PPM_Obj_TypeDef;

typedef struct
{

} Sbus_Obj_TypeDef;

typedef struct
{
    /* data */
} Crsf_Obj_TypeDef;

typedef enum
{
    Signal_PPM = 1 << 0,
    Signal_Sbus = 2 << 0,
    Signal_Crsf = 3 << 0,
} DevSignal_Type_List;

typedef enum
{
    Signal_In = 0,
    Signal_Out,
} DevSignal_IO_Type_List;

#endif
