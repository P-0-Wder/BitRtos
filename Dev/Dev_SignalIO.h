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
    Sig_PPM = 1 << 0,
    Sig_Sbus = 1 << 1,
    Sig_Crsf = 1 << 2,
    Sig_Sync = 1 << 3,
} DevSignal_Type_List;

typedef enum
{
    Sig_In = 0,
    Sig_Out,
} DevSignal_Trans_Type_List;

#endif
