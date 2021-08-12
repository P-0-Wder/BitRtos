#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "stddef.h"
#include "runtime.h"
#include "task_manager_cfg.h"
#include "binary_tree.h"
#include "linked_list.h"

#define TASK_REGISTED 1
#define TASK_BAD_MEMSPC_REQ 0
#define TASK_SUCCESS_MEMSPC_REQ 1

#define ADDR_OFFSET(i) i << 2

#define TASK_NAME_MAXLEN 32
#define TASK_TIME_OUT
#define TASK_EXEC_10KHZ 10000
#define TASK_EXEC_8KHZ 8000
#define TASK_EXEC_5KHZ 5000
#define TASK_EXEC_4KHZ 4000
#define TASK_EXEC_2KHZ 2000
#define TASK_EXEC_1KHZ 1000
#define TASK_EXEC_500HZ 500
#define TASK_EXEC_250HZ 250
#define TASK_EXEC_200HZ 200
#define TASK_EXEC_150HZ 150
#define TASK_EXEC_100HZ 100
#define TASK_EXEC_50HZ 50

#define NOERROR 0
#define ERROR_NULLTASK 1

#define TASK_IDLE_GROUP 8
#define TASK_IDLE_PRIORITY 8

typedef uint32_t Task_Handler;
typedef void (*Task_Func)(Task_Handler hdl);

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
typedef uint32_t *Task_STK_Ptr;

typedef enum
{
    Us_Unit,
    Ms_Unit,
} delay_unit_list;

typedef struct
{
    bool on_delay;
    Task_Handler tsk_hdl;
    uint32_t time_unit;
    SYSTEM_RunTime resume_Rt;
} delay_reg;

typedef enum
{
    naturely,
    forcedly,
} exec_status_list;

typedef struct
{
    volatile Task_STK_Ptr Top_Stk_Ptr;
    Task_STK_Ptr Stack;
} TaskStack_ControlBlock;

typedef union
{
    struct
    {
        uint8_t Caller_Statistics_REG : 4;
        uint8_t TskFun_Statistics_REG : 4;
    } Reg;

    uint8_t Val;
} TaskOpr_statistics;

typedef enum
{
    stop_statistics,
    start_statistics,
} task_statistics_status_list;

typedef enum
{
    Stack_Reg_xPSR = 1,
    Stack_Reg_PC,
    Stack_Reg_LR,
    Stack_Reg_R12,
    Stack_Reg_R3,
    Stack_Reg_R2,
    Stack_Reg_R1,
    Stack_Reg_R0,
    Stack_Reg_R11,
    Stack_Reg_R10,
    Stack_Reg_R9,
    Stack_Reg_R8,
    Stack_Reg_R7,
    Stack_Reg_R6,
    Stack_Reg_R5,
    Stack_Reg_R4,
    Stack_Reg_Sum,
} Stack_Reg_List_e;
#endif

typedef union
{
    uint8_t Priority;

    struct
    {
        uint8_t Task_Level : 3;
        uint8_t Group_Level : 3;
    } Suction;
} Priority_Str;

typedef union
{
    uint8_t Flg;

    struct
    {
        uint8_t bit_0 : 1;
        uint8_t bit_1 : 1;
        uint8_t bit_2 : 1;
        uint8_t bit_3 : 1;
        uint8_t bit_4 : 1;
        uint8_t bit_5 : 1;
        uint8_t bit_6 : 1;
        uint8_t bit_7 : 1;
    } Bit;
} Flg_template;

typedef enum
{
    TaskSys_Initial = 0,
    TaskSys_Prepare,
    TaskSys_Start,
} TaskSys_State;

typedef enum
{
    Task_Ready,
    Task_Running,
    Task_Stop,
    Task_Block,
    Task_Pending,
    Task_Done,
} TASK_STATE;

typedef enum
{
    Task_Priority_0 = 0,
    Task_Priority_1,
    Task_Priority_2,
    Task_Priority_3,
    Task_Priority_4,
    Task_Priority_5,
    Task_Priority_6,
    Task_Priority_7,
    Task_Priority_Sum,
} TASK_Priority;

typedef enum
{
    Group_0 = 0,
    Group_1,
    Group_2,
    Group_3,
    Group_4,
    Group_5,
    Group_6,
    Group_7,
    Group_Sum,
} Priority_Group;

typedef struct
{
    Flg_template Grp;
    Flg_template TskInGrp[Group_Sum];
} TskMap_State;

typedef struct
{
    SYSTEM_RunTime Init_Time;
    SYSTEM_RunTime Start_Time;
    SYSTEM_RunTime Exec_Time;

    uint32_t detect_exec_frq;      //detect task running frq
    uint32_t detect_exec_time_arv; //task  average running time
    uint32_t detect_exec_time_max; //task max running time
    uint32_t Exec_Times;
    uint8_t error_code;

    uint32_t totlal_running_time;
    float cpu_opy;
    TASK_STATE State;
} Task_Exec_Status;

typedef struct
{
    Priority_Str priority;
    char *Task_name;

    uint32_t exec_frq;
    uint16_t exec_interval_us;

    Task_Func Exec_Func;
    Task_Exec_Status Exec_status;

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    uint32_t Stack_Depth;
    TaskStack_ControlBlock TCB;
#endif

    TaskOpr_statistics statistics_reg;

    //private variable
    uint32_t TskFuncUing_US; //Cast US time while system Calling the Target task function
    uint32_t CallerUsing_US; //Cast US time while Task_Caller Function is in using

    delay_reg delay_info;

    item_obj *item_ptr;
    item_obj delay_item;
} Task;

typedef struct
{
    Task_Handler tsk_hdl;
    uint8_t name[TASK_NAME_MAXLEN];
    uint8_t group;
    uint8_t priority;
    uint16_t set_frq;
    uint16_t exec_frq;
    uint16_t interval_us;
    float cpu_opy;
    uint16_t avg_exec_us;
    uint16_t max_exec_us;
    uint32_t exec_times;
} Task_Base_Info;

typedef struct
{
    uint8_t num;
    list_obj list;
} Task_Create_RegList_s;

void Task_Scheduler(void);
void Task_Caller(void);
void TaskSystem_Start(void);

Task_Handler Task_Create(const char *name, uint32_t frq, Priority_Group group, TASK_Priority priority, Task_Func func, uint32_t StackDepth);
void Task_Remove(Task_Handler Tsk_Hdl);
void Task_SetRunState(Task_Handler Tsk_Handle, TASK_STATE state);
void Task_PrintState(Task_Handler Tsk_Hdl);
Task *Task_GetCurrentRunTask(void);
Task *Task_PriorityCompare(const Task *tsk_l, const Task *tsk_r);
void TaskSystem_Start(void);
uint8_t Task_Get_TaskNum(void);
Task_Base_Info Task_GetInfo_ByIndex(uint8_t index);
uint32_t Task_Get_IdleUS(void);
float Task_Get_IdleOcupy(void);
void Task_CountRunningTime(void);
void TaskSys_Set_State(TaskSys_State state);
void Task_TriggerPendSV(void);
TaskSys_State TaskSys_Get_State(void);

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
void Load_FirstTask(void);
void Task_Load(void);
void Task_SwitchContext(void);
void Task_SetBlock(Task *tsk);
void Task_SetPending(Task *tsk);
void Task_Force_TriggerPendSV(void);
void Task_SaveCurProc(void);
void Task_Resume_FromBlock(Task *tsk);
void Task_SetNextTask_Ptr(const Task *nxt);

bool Task_Pause(Task_Handler tsk_hdl);
bool Task_FrcPause(Task_Handler tsk_hdl);
bool Task_Resume(Task_Handler tsk_hdl);
bool Task_FrcExec(Task_Handler tsk_hdl);

Task *Task_Get_HighestRank_RdyTask(void);
Task *Task_Get_HighestRank_PndTask(void);
#endif

#endif
