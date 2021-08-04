#ifndef __RUNTIME_H
#define __RUNTIME_H

#include <stdbool.h>
#include <stdint.h>

#define REAL_MS 1000 /* 1ms == 1000us */
#define REAL_S REAL_MS * 1000

#define RUNTIEM_MAX_TICK_FRQ RUNTIME_TICK_FRQ_1M

#define RUNTIME_TICK_FRQ_1M 1e6    /* 1us base time */
#define RUNTIME_TICK_FRQ_500K 5e5  /* 2us base time */
#define RUNTIMT_TICK_FRQ_250K 25e4 /* 4us bse time */
#define RUNTIME_TICK_FRQ_200K 20e4 /* 5us base time */
#define RUNTIME_TICK_FRQ_100K 10e4 /* 10us base time */
#define RUNTIME_TICK_FRQ_50K 5e4   /* 20us base time */
#define RUNTIME_TICK_FRQ_25K 25e3  /* 40us base time */
#define RUNTIME_TICK_FRQ_20K 2e4   /* 50us base time */
#define RUNTIME_TICK_FRQ_10K 1e4   /* 100us base time */
#define RUNTIME_TICK_FRQ_5K 5e3    /* 200us base time */
#define RUNTIME_TICK_FRQ_2K 2e3    /* 500us base time */
#define RUNTIME_TICK_FRQ_1K 1e3    /* 1ms base time */

typedef bool (*runtime_stop_p)(void);
typedef bool (*runtime_timer_init_p)(uint16_t tick_frq);
typedef uint32_t (*runtime_start_callback_p)(void);
typedef uint32_t (*runtime_stop_callback_p)(void);

typedef enum
{
    Runtime_Module_Init,
    Runtime_Module_Start,
    Runtime_Module_Stop,
} Runtime_ModuleState_List;

typedef enum
{
    Runtime_Run_Tick,
    Runtime_Run_Wait,
} Runtime_RunState_List;

/* runtime data block object definition*/
typedef struct
{
    uint64_t Use_Us;

    uint32_t time_base;
    uint32_t tick_frq;

    runtime_start_callback_p start_callback;
    runtime_stop_callback_p stop_callback;
    runtime_timer_init_p timer_init;

    Runtime_ModuleState_List module_state;
    Runtime_RunState_List tick_state;
} Runtime_DataObj_TypeDef;

#endif
