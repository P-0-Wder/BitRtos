#ifndef __EVENT_FUNC_H
#define __EVENT_FUNC_H

#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "runtime.h"
#include "linked_list.h"

typedef uint32_t event_item_handler;
typedef uint32_t event_block_handler;
typedef void (*Event_CallBack_Func)(void *arg);

//state poll and process enum
//used in the OSEvent_PollProcess Function
typedef enum
{
    event_halt_stb,
    event_pause_stb,
    event_resume_stb,
    event_remove_stb,
    event_regsite_stb,
    event_trigger_stb,
    event_process_stb,
} event_standby_opr_id;

typedef enum
{
    event_no_error,
    event_uninit,
    event_list_empty,
    event_opr_locked,
    event_init_error,
    error_lock_error,
    event_item_error,
    event_block_error,
    error_unlock_error,
    event_insert_error,
    event_list_obj_error,
    event_popfirst_error,
    event_proc_fstitm_error,
    evebt_item_create_error,
    event_block_create_error,
    event_no_registed_things,
} event_err_code_id;

typedef enum
{
    evt_proc_Immd,  //process event immediately
    evt_proc_InSch, //process when scheduler is calling
} event_proc_type;

typedef enum
{
    evt_id_os,
    evt_id_runtime,
} module_event_id;

typedef enum
{
    evt_os_start,
    evt_os_pause,
    evt_os_resume,
    evt_os_stop,
} os_event_id;

typedef struct
{
    SYSTEM_RunTime trg_time;
    SYSTEM_RunTime prc_time;

    event_proc_type proc_type;

    uint8_t module_id;
    uint8_t evt_id;

    char *descr;

    void *arg;
    Event_CallBack_Func callback;
    bool triggered_status;
} event_block_s;

typedef struct
{
    event_block_handler block_hdl;
    event_item_handler item_hdl;

    event_err_code_id error_id; //error output
} event_regs_TypeDef;

#pragma pack(1)
typedef struct
{
    event_standby_opr_id standby_id;
    void *arg;
} StanBy_Event_TypeDef;

bool Event_Enable(void);
event_err_code_id Event_Poll(void);

#endif