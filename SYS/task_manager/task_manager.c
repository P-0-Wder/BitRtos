#include "task_manager.h"
#include "runtime.h"
#include "string.h"
#include "stddef.h"
#include "drv_timer.h"
#include <stdio.h>
#include "task_manager_cfg.h"
#include "binary_tree.h"
#include "linked_list.h"
#include "mmu.h"

//coder: 8_B!T0
//bref:
//estabishment a task running system with priority calling functional
//alway calling the highest priority function in all function under the ready table
//reference to ucos (version earler than v8.6)

/*
*******************************   TASK Handle    *******************************

             task0   task1   task2   task3   task4   task5   task6   task7
group0     |_______|_______|_______|_______|_______|_______|_______|_______|
group1     |_______|_______|_______|_______|_______|_______|_______|_______|
group2     |_______|_______|_______|_______|_______|_______|_______|_______|
group3     |_______|_______|_______|_______|_______|_______|_______|_______|
group4     |_______|_______|_______|_______|_______|_______|_______|_______|
group5     |_______|_______|_______|_______|_______|_______|_______|_______|
group6     |_______|_______|_______|_______|_______|_______|_______|_______|
group7     |_______|_______|_______|_______|_______|_______|_______|_______|

*/

#define GET_TASKGROUP_PRIORITY(x) x >> 3
#define GET_TASKINGROUP_PRIORITY(y) y & 0X07

static const uint8_t Tsk_Handle[256] = {0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x00 ~ 0x0F
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x10 ~ 0x1F
                                        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x20 ~ 0x2F
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x30 ~ 0x3F
                                        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x40 ~ 0x4F
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x50 ~ 0x5F
                                        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x60 ~ 0x6F
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x70 ~ 0x7F
                                        7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x80 ~ 0x8F
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0x90 ~ 0x9F
                                        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0xA0 ~ 0xAF
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0xB0 ~ 0xBF
                                        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0xC0 ~ 0xCF
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0xD0 ~ 0xDF
                                        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,  //0xE0 ~ 0xEF
                                        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0}; //0xF0 ~ 0xFF

Task *Task_Ptr[Group_Sum][Task_Priority_Sum];
volatile Task *CurRunTsk_Ptr = NULL;
volatile Task *NxtRunTsk_Ptr = NULL;
static bool traverse_start = false;

static volatile TskMap_State TskHdl_RdyMap = {.Grp = 0, .TskInGrp[0] = 0, .TskInGrp[1] = 0, .TskInGrp[2] = 0, .TskInGrp[3] = 0, .TskInGrp[4] = 0, .TskInGrp[5] = 0, .TskInGrp[6] = 0, .TskInGrp[7] = 0};
static volatile TskMap_State TskHdl_PndMap = {.Grp = 0, .TskInGrp[0] = 0, .TskInGrp[1] = 0, .TskInGrp[2] = 0, .TskInGrp[3] = 0, .TskInGrp[4] = 0, .TskInGrp[5] = 0, .TskInGrp[6] = 0, .TskInGrp[7] = 0};
static volatile TskMap_State TskHdl_BlkMap = {.Grp = 0, .TskInGrp[0] = 0, .TskInGrp[1] = 0, .TskInGrp[2] = 0, .TskInGrp[3] = 0, .TskInGrp[4] = 0, .TskInGrp[5] = 0, .TskInGrp[6] = 0, .TskInGrp[7] = 0};

static Task_Create_RegList_s TskCrt_RegList = {.num = 0,
                                               .list = {.prv = NULL, .nxt = NULL, .data = NULL}};
static volatile TaskSys_State TskSys_state = TaskSys_Initial;
static uint32_t TaskSys_Idle_US = 0;
static float TaskSys_Idle_Ocupy = 0.0f;
static SYSTEM_RunTime TaskSys_StartTime;

static void Task_SetReady(Task *tsk);
static void Task_ClearReady(Task *tsk);

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
volatile Task *PndHstTsk_Ptr = NULL;

volatile TaskStack_ControlBlock CurTsk_TCB;
volatile TaskStack_ControlBlock NxtTsk_TCB;

static uint32_t Task_OS_StkMem[MSP_MEM_SPACE_SIZE];
static DrvTimer_Obj_TypeDef SysTimerObj;
uint32_t *Task_OS_ExpStkBase;

static void Task_SetStkPtr_Val(Task *tsk);
static void Task_ClearPending(Task *tsk);
static void Task_ClearBlock(Task *tsk);
static void Task_SetBASEPRI(uint32_t ulBASEPRI);

void Task_SetPending(Task *tsk);
#endif

static void Task_Statistic_Cast(uint8_t *time_base, uint16_t unuse);
static void Task_Exec(Task *tsk_ptr);
static void Task_SetPendSVPro(void);
void Task_TriggerPendSV(void);

static void ReSet_Task_Data(Task *task)
{
    task->priority.Suction.Group_Level = 0;
    task->priority.Suction.Task_Level = 0;
    task->Task_name = NULL;
    task->exec_frq = 0;
    task->exec_interval_us = 0;
    task->Exec_Func = NULL;

    task->Exec_status.detect_exec_frq = 0;
    task->Exec_status.detect_exec_time_arv = 0;
    task->Exec_status.detect_exec_time_max = 0;
    task->Exec_status.Exec_Times = 0;
    task->Exec_status.cpu_opy = 0;
    task->Exec_status.totlal_running_time = 0;

    task->delay_info.on_delay = false;
    task->delay_info.tsk_hdl = 0;
    task->delay_info.time_unit = 0;

    List_ItemInit(&task->delay_item, &task->delay_info);

    RuntimeObj_Reset(&(task->Exec_status.Exec_Time));
    RuntimeObj_Reset(&(task->Exec_status.Init_Time));
    RuntimeObj_Reset(&(task->Exec_status.Start_Time));

    task->Exec_status.State = Task_Done;
}

void Task_ReSet_BaseInfo(Task_Base_Info *info)
{
    info->tsk_hdl = 0;
    memset(info->name, NULL, TASK_NAME_MAXLEN);
    info->group = 0;
    info->priority = 0;
    info->set_frq = 0;
    info->exec_frq = 0;
    info->cpu_opy = 0.0f;
    info->stk_depth = 0;
    info->remain_stk_size = 0;
}

static bool TaskSys_Init(void)
{
#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    //init OS Stack Memory Space
    memset(Task_OS_StkMem, NULL, MSP_MEM_SPACE_SIZE);

    Task_OS_ExpStkBase = Task_OS_StkMem + MSP_MEM_SPACE_SIZE - 1;
#endif

    DrvTimer.obj_clear(&SysTimerObj);

    SysTimerObj.timerx = Timer_4;
    SysTimerObj.period = TimerCounter_1us_Period;
    SysTimerObj.Prescaler = TimerCounter_1M_Prescaler;
    SysTimerObj.PreemptionPriority = 0;
    SysTimerObj.SubPriority = 1;

    DrvTimer.ctl(DrvTimer_Counter_Mode, (uint32_t)&SysTimerObj, 0);
    DrvTimer.ctl(DrvTimer_Counter_SetIRQCallback, (uint32_t)&SysTimerObj, Task_Statistic_Cast);
    // periph_Timer_CounterMode_Init(Timer_4, TimerCounter_1M_Prescaler, TimerCounter_1us_Period, 0, 1);
    // periph_Timer_Counter_SetEnable(Timer_4, DISABLE);

    uint8_t index = 0;

    for (uint8_t group_index = Group_0; group_index < Group_Sum; group_index++)
    {
        for (uint8_t task_priority = Task_Priority_0; task_priority < Task_Priority_Sum; task_priority++)
        {
            ReSet_Task_Data(Task_Ptr[group_index][task_priority]);

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
            Task_Ptr[group_index][task_priority]->Stack_Depth = 0;
            Task_Ptr[group_index][task_priority]->TCB.Stack = NULL;
            Task_Ptr[group_index][task_priority]->TCB.Top_Stk_Ptr = NULL;
#endif
            Task_Ptr[group_index][task_priority] = NULL;

            index++;
        }
    }

    TskCrt_RegList.num = 0;
    TskCrt_RegList.list.data = NULL;
    TskCrt_RegList.list.nxt = NULL;
    TskCrt_RegList.list.prv = NULL;

    ReSet_Task_Data(CurRunTsk_Ptr);
    TskSys_state = TaskSys_Prepare;

    NVIC_SetPriority(PendSV_IRQn, 15);
}

static void Task_Exit(void)
{
    //task caller will not exit
    while (true)
    {
    }
}

static void Task_Idle(void)
{
    SYSTEM_RunTime cur_time;
    SYSTEM_RunTime time_diff;

    RuntimeObj_Reset(&time_diff);
    RuntimeObj_Reset(&cur_time);

    cur_time = Get_CurrentRunningUs();
    time_diff = Get_TimeDifference_Between(TaskSys_StartTime, cur_time);

    TaskSys_Idle_US++;
    TaskSys_Idle_Ocupy = (TaskSys_Idle_US / (float)time_diff);
}

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)

void Task_Resume_FromBlock(Task *tsk)
{
    Task_ClearBlock(tsk);
    Task_SetReady(tsk);

    tsk->Exec_status.State = Task_Running;
    NxtTsk_TCB.Top_Stk_Ptr = &tsk->TCB.Top_Stk_Ptr;
    NxtTsk_TCB.Stack = tsk->TCB.Stack;
    CurRunTsk_Ptr = tsk;
}

void Task_SetBlock(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    //set current group block
    TskHdl_BlkMap.Grp.Flg |= 1 << grp_id;
    //set current task under this group flag to block
    TskHdl_BlkMap.TskInGrp[grp_id].Flg |= 1 << tsk_id;

    //reset task in ready reg group
    TskHdl_RdyMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
    //if no task ready then clear group reg
    if (TskHdl_RdyMap.TskInGrp[grp_id].Flg == 0)
    {
        TskHdl_RdyMap.Grp.Flg &= ~(1 << grp_id);
    }

    //set task state
    tsk->Exec_status.State = Task_Block;

    CurRunTsk_Ptr = NULL;

    Task_SetBASEPRI(0);
}

void Task_SetPending(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    //set current group pending
    TskHdl_PndMap.Grp.Flg |= 1 << grp_id;
    //set current task under this group flag to ready
    TskHdl_PndMap.TskInGrp[grp_id].Flg |= 1 << tsk_id;

    //set task state
    tsk->Exec_status.State = Task_Pending;
}

void Task_SwitchStack(void)
{
    CurTsk_TCB = NxtTsk_TCB;
}

Task *Task_Get_HighestRank_PndTask(void)
{
    uint8_t grp_id;
    uint8_t tsk_id;

    if (TskHdl_PndMap.Grp.Flg)
    {
        //find group
        grp_id = Tsk_Handle[TskHdl_PndMap.Grp.Flg];
        //find task in group
        tsk_id = Tsk_Handle[TskHdl_PndMap.TskInGrp[grp_id].Flg];
    }
    else
        return NULL;

    if (Task_Ptr[grp_id][tsk_id] != NULL)
    {
        return Task_Ptr[grp_id][tsk_id];
    }
    else
    {
        TskHdl_PndMap.Grp.Flg &= ~(1 << grp_id);
        TskHdl_PndMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
        return NULL;
    }
}

static uint32_t Task_EnterCritical(void)
{
    /* Set BASEPRI to the max syscall priority to effect a critical
	section. */
    uint32_t ulOriginalBASEPRI, ulNewBASEPRI;

    __asm volatile(
        "	mrs %0, basepri											\n"
        "	mov %1, %2												\n"
        "	msr basepri, %1											\n"
        "	isb														\n"
        "	dsb														\n"
        : "=r"(ulOriginalBASEPRI), "=r"(ulNewBASEPRI)
        : "i"(80)
        : "memory");

    /* This return will not be reached but is necessary to prevent compiler
	warnings. */
    return ulOriginalBASEPRI;
}

__attribute__((nake)) static void Task_SetBASEPRI(uint32_t ulBASEPRI)
{
    __ASM("	msr basepri, %0	" ::"r"(ulBASEPRI)
          : "memory");
}

__attribute__((naked)) static void Task_ExitCritical(void)
{
    /* Barrier instructions are not used as this function is only used to
	lower the BASEPRI value. */
    __ASM("	msr basepri, %0	" ::"r"(0)
          : "memory");
}

__attribute__((naked)) static void Task_Recover(void)
{
    __ASM("LDR	  R3, CurrentTCBConst_Tmp2");
    __ASM("LDR    R1, [R3]");
    __ASM("LDR    R0, [R1]");

    __ASM("LDMIA  R0!, {R4-R11, R14}");

    __ASM("MSR    PSP, R0");
    __ASM("ISB");
    __ASM("BX     R14");
    __ASM(".ALIGN 4");
    __ASM("CurrentTCBConst_Tmp2: .word CurTsk_TCB");
}

__attribute__((naked)) void Task_SaveCurProc(void)
{
    __ASM("MRS    R0, PSP");
    __ASM("LDR	  R3, CurrentTCBConst_Tmp3");
    __ASM("LDR    R2, [R3]");

    __ASM("ISB");

    __ASM("STMDB    R0!, {R4-R11, R14}");
    __ASM("STR      R0, [R2]");

    /******************************  FPU SECTION  *********************************/
    __ASM("TST      R14, #0x10");
    __ASM("IT       EQ");
    __ASM("VSTMDBEQ R0!, {s16-s31}");
    /******************************  FPU SECTION  *********************************/

    __ASM("ISB");
    __ASM("BX       R14");

    __ASM("CurrentTCBConst_Tmp3: .word CurTsk_TCB");
}

__attribute__((naked)) void Task_Load(void)
{
    __ASM("LDR	  R3, =CurTsk_TCB");
    __ASM("LDR    R1, [R3]");
    __ASM("LDR    R0, [R1]");

    __ASM("LDMIA  R0!, {R4-R11, R14}");

    /******************************  FPU SECTION  *********************************/
    __ASM("TST      R14, #0x10");
    __ASM("IT       EQ");
    __ASM("VLDMIAEQ R0!, {s16-s31}");
    /******************************  FPU SECTION  *********************************/

    __ASM("MSR    PSP, R0");
    __ASM("ISB");
    //__ASM("MOV    R0, #240");
    //__ASM("MSR	  BASEPRI, R0");
    __ASM("BX     R14");
    __ASM(".ALIGN 4");
}

void Load_FirstTask(void)
{
    Task_Load();
}

__attribute__((naked)) void Task_SwitchContext(void)
{
    __ASM("MRS      R0, PSP");
    __ASM("ISB");

    __ASM("LDR      R3, CurrentTCBConst_Tmp");
    __ASM("LDR      R2, [R3]");

    /******************************  FPU SECTION  *********************************/
    __ASM("TST      R14, #0x10");
    __ASM("IT       EQ");
    __ASM("VSTMDBEQ R0!, {s16-s31}");
    /******************************  FPU SECTION  *********************************/

    __ASM("STMDB    R0!, {R4-R11, R14}");
    __ASM("STR      R0, [R2]");

    __ASM("STMDB    SP!, {R0, R3}");
    __ASM("MOV      R0, %0" ::"i"(0x50));
    __ASM("MSR      BASEPRI, R0");

    __ASM("DSB");
    __ASM("ISB");

    __ASM("BL       Task_SwitchStack");

    __ASM("MOV      R0, #0");
    __ASM("MSR      BASEPRI, R0");
    __ASM("LDMIA    SP!, {R0, R3}");

    __ASM("LDR      R1, [R3]");
    __ASM("LDR      R0, [R1]");

    __ASM("LDMIA    R0!, {R4-R11, R14}");

    /******************************  FPU SECTION  *********************************/
    __ASM("TST      R14, #0x10");
    __ASM("IT       EQ");
    __ASM("VLDMIAEQ R0!, {s16-s31}");
    /******************************  FPU SECTION  *********************************/

    __ASM("MSR      PSP,R0");
    __ASM("ISB");
    //__ASM("MOV      R0, #240");
    //__ASM("MSR	    BASEPRI, R0");
    __ASM("BX       R14");

    __ASM("CurrentTCBConst_Tmp: .word CurTsk_TCB");
    __ASM(".ALIGN 4");
}

static void Task_SetStkPtr_Val(Task *tsk)
{
    uint32_t *Tsk_Ptr_tmp = NULL;

    memset(tsk->TCB.Stack, TASK_STACK_DEFAULT, tsk->Stack_Depth * sizeof(uint32_t));

    Tsk_Ptr_tmp = &tsk->TCB.Stack + (tsk->Stack_Depth - (uint32_t)1);
    Tsk_Ptr_tmp = (uint32_t *)((uint32_t)(Tsk_Ptr_tmp)&0XFFFFFFF8ul);

    Tsk_Ptr_tmp--;
    *Tsk_Ptr_tmp = 0x01000000uL; /* xPSR */

    Tsk_Ptr_tmp--;
    *Tsk_Ptr_tmp = ((uint32_t)Task_Caller) & 0xfffffffeUL; /* PC */

    Tsk_Ptr_tmp--;
    *Tsk_Ptr_tmp = (uint32_t)Task_Exit; /* LR */

    /* Save code space by skipping register initialisation. */
    Tsk_Ptr_tmp -= 5;              /* R12, R3, R2 and R1. */
    *Tsk_Ptr_tmp = (uint32_t)NULL; /* R0 */

    /* A save method is being used that requires each task to maintain its
	    own exec return value. */
    Tsk_Ptr_tmp--;
    *Tsk_Ptr_tmp = 0xfffffffd;

    Tsk_Ptr_tmp -= 8; /* R11, R10, R9, R8, R7, R6, R5 and R4. */

    //set task stack top pointer
    tsk->TCB.Top_Stk_Ptr = Tsk_Ptr_tmp; //&Tsk_Ptr_tmp
}

#endif

static void Task_SetReady(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    //set current group flag to ready
    TskHdl_RdyMap.Grp.Flg |= 1 << grp_id;
    //set current task under this group flag to ready
    TskHdl_RdyMap.TskInGrp[grp_id].Flg |= 1 << tsk_id;

    tsk->Exec_status.State = Task_Ready;
}

static void Task_ClearBlock(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    TskHdl_BlkMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
    if (TskHdl_BlkMap.TskInGrp[grp_id].Flg == 0)
    {
        TskHdl_BlkMap.Grp.Flg &= ~(1 << grp_id);
    }
}

static void Task_ClearPending(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    TskHdl_PndMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
    if (TskHdl_PndMap.TskInGrp[grp_id].Flg == 0)
    {
        TskHdl_PndMap.Grp.Flg &= ~(1 << grp_id);
    }
}

static void Task_ClearReady(Task *tsk)
{
    uint8_t grp_id = GET_TASKGROUP_PRIORITY(tsk->priority.Priority);
    uint8_t tsk_id = GET_TASKINGROUP_PRIORITY(tsk->priority.Priority);

    TskHdl_RdyMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
    if (TskHdl_RdyMap.TskInGrp[grp_id].Flg == 0)
    {
        TskHdl_RdyMap.Grp.Flg &= ~(1 << grp_id);
    }
}

Task *Task_Get_HighestRank_RdyTask(void)
{
    uint8_t grp_id = 0;
    uint8_t tsk_id = 0;

    if (TskHdl_RdyMap.Grp.Flg)
    {
        //find group
        grp_id = Tsk_Handle[TskHdl_RdyMap.Grp.Flg];
        //find task in group
        tsk_id = Tsk_Handle[TskHdl_RdyMap.TskInGrp[grp_id].Flg];
    }
    else
        return NULL;

    if (Task_Ptr[grp_id][tsk_id] != NULL)
    {
        return Task_Ptr[grp_id][tsk_id];
    }
    else
    {
        TskHdl_RdyMap.Grp.Flg &= ~(1 << grp_id);
        TskHdl_RdyMap.TskInGrp[grp_id].Flg &= ~(1 << tsk_id);
        return NULL;
    }
}

void Task_SetNextTask_Ptr(const Task *nxt)
{
    if (nxt != NULL)
    {
        NxtRunTsk_Ptr = nxt;
        NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
        NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;
    }
}

//first need to know is linux support AT&T formate ASM code
__attribute__((naked)) static void Task_SetPendSVPro(void)
{
    //set pendsv interrupt
    __ASM(".equ NVIC_SYSPRI14, 0xE000ED22");
    __ASM(".equ NVIC_PENDSV_PRI, 0xFF");

    __ASM("LDR      R0, =NVIC_SYSPRI14");
    __ASM("LDR      R1, =NVIC_PENDSV_PRI");
    __ASM("STRB     R1, [R0]");

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    //set PSP to 0 to initial context switch call
    __ASM("MOVS     R0, #0");
    __ASM("MSR      PSP, R0");

    //initial MSP to Task_OS_ExpStkBase
    __ASM("LDR      R0, =Task_OS_ExpStkBase");
    __ASM("LDR      R1, [R0]");
    __ASM("MSR      MSP, R1");
#endif

    __ASM("BX       LR");
}

__attribute__((naked)) void Task_TriggerPendSV(void)
{
    __ASM(".equ NVIC_INT_CTRL, 0xE000ED04");
    __ASM(".equ NVIC_PENDSVSET, 0x10000000");

    __ASM("LDR      R0, =NVIC_INT_CTRL");
    __ASM("LDR      R1, =NVIC_PENDSVSET");
    __ASM("STR      R1, [R0]");
    __ASM("BX       LR");
}

void Task_Force_TriggerPendSV(void)
{
    /* Barriers are normally not required but do ensure the code is completely \
        within the specified behaviour for the architecture. */
    __ASM volatile("dsb" ::
                       : "memory");
    __ASM volatile("isb");
    Task_TriggerPendSV();
}

uint8_t Task_Get_TaskNum(void)
{
    return TskCrt_RegList.num;
}

//return high priority task pointer
Task *Task_PriorityCompare(const Task *tsk_l, const Task *tsk_r)
{
    if ((tsk_l == NULL) && (tsk_r == NULL))
    {
        return NULL;
    }

    if ((tsk_l == NULL) && (tsk_r != NULL))
    {
        return tsk_r;
    }

    if ((tsk_l != NULL) && (tsk_r == NULL))
    {
        return tsk_l;
    }

    uint8_t L_Grp_level = GET_TASKGROUP_PRIORITY(tsk_l->priority.Suction.Group_Level);
    uint8_t R_Grp_level = GET_TASKGROUP_PRIORITY(tsk_r->priority.Suction.Group_Level);
    uint8_t L_TskPri_level = GET_TASKINGROUP_PRIORITY(tsk_l->priority.Suction.Task_Level);
    uint8_t R_TskPri_level = GET_TASKINGROUP_PRIORITY(tsk_r->priority.Suction.Task_Level);

    if (L_Grp_level < R_Grp_level)
    {
        return tsk_l;
    }
    else if (L_Grp_level > R_Grp_level)
    {
        return tsk_r;
    }
    else if (L_Grp_level == R_Grp_level)
    {
        if (L_TskPri_level < R_TskPri_level)
        {
            return tsk_l;
        }
        else if (L_TskPri_level > R_TskPri_level)
        {
            return tsk_r;
        }
    }
}

Task_Handle Task_Create(const char *name, uint32_t frq, Priority_Group group, TASK_Priority priority, Task_Func func, uint32_t StackDepth)
{
    Task_Handle handle;
    uint16_t task_name_len = strlen(name);
    uint32_t *Tsk_Ptr_tmp = NULL;
    static bool taskOs_InitState = false;

    if (!taskOs_InitState)
    {
        TaskSys_Init();
        taskOs_InitState = true;
    }

    //already have task in current group and priority in task pointer matrix
    if (Task_Ptr[group][priority] != NULL)
    {
        return TASK_REGISTED;
    }

    //request a memory space for Task_Ptr contain
    Task_Ptr[group][priority] = (Task *)MMU_Malloc(sizeof(Task));

    //record Task_Ptr poiner`s address
    handle = *&Task_Ptr[group][priority];

    Task_Ptr[group][priority]->Task_name = name;

    Task_Ptr[group][priority]->exec_frq = frq;
    Task_Ptr[group][priority]->exec_interval_us = RUNTIEM_MAX_TICK_FRQ / frq;
    Task_Ptr[group][priority]->Exec_Func = func;

    Task_Ptr[group][priority]->priority.Priority = (group << 3) | priority;

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    //init delay tag
    Task_Ptr[group][priority]->delay_info.on_delay = false;
    Task_Ptr[group][priority]->delay_info.tsk_hdl = handle;
    Task_Ptr[group][priority]->delay_info.time_unit = 0;

    //request memory space for task stack
    Task_Ptr[group][priority]->Stack_Depth = StackDepth;
    Task_Ptr[group][priority]->TCB.Stack = (uint32_t *)MMU_Malloc(StackDepth * sizeof(uint32_t));

    if (Task_Ptr[group][priority]->TCB.Stack != NULL)
    {
        Task_SetStkPtr_Val(Task_Ptr[group][priority]);
    }
    else
    {
        return TASK_BAD_MEMSPC_REQ;
    }
#endif

    //reset single loop running us
    Task_Ptr[group][priority]->TskFuncUing_US = 0;

    //reset task cpu occupy data
    Task_Ptr[group][priority]->Exec_status.cpu_opy = 0;
    Task_Ptr[group][priority]->Exec_status.totlal_running_time = 0;

    //set current group flag to ready
    TskHdl_RdyMap.Grp.Flg |= 1 << GET_TASKGROUP_PRIORITY(Task_Ptr[group][priority]->priority.Priority);
    //set current task under this group flag to ready
    TskHdl_RdyMap.TskInGrp[GET_TASKGROUP_PRIORITY(Task_Ptr[group][priority]->priority.Priority)].Flg |= 1 << GET_TASKINGROUP_PRIORITY(Task_Ptr[group][priority]->priority.Priority);

    Task_Ptr[group][priority]->Exec_status.detect_exec_frq = 0;
    Task_Ptr[group][priority]->Exec_status.detect_exec_time_arv = 0;
    Task_Ptr[group][priority]->Exec_status.detect_exec_time_max = 0;

    RuntimeObj_Reset(&(Task_Ptr[group][priority]->Exec_status.Exec_Time));
    RuntimeObj_Reset(&(Task_Ptr[group][priority]->Exec_status.Init_Time));
    RuntimeObj_Reset(&(Task_Ptr[group][priority]->Exec_status.Start_Time));

    Task_Ptr[group][priority]->Exec_status.Init_Time = Get_CurrentRunningUs();

    Task_Ptr[group][priority]->Exec_status.Exec_Times = 0;
    Task_Ptr[group][priority]->Exec_status.error_code = NOERROR;

    Task_SetReady(Task_Ptr[group][priority]);

    Task_Ptr[group][priority]->item_ptr = (item_obj *)MMU_Malloc(sizeof(item_obj));
    if (Task_Ptr[group][priority]->item_ptr == NULL)
    {
        return TASK_BAD_MEMSPC_REQ;
    }

    List_ItemInit(Task_Ptr[group][priority]->item_ptr, Task_Ptr[group][priority]);
    if (TskCrt_RegList.num == 0)
    {
        List_Init(&TskCrt_RegList.list, Task_Ptr[group][priority]->item_ptr, by_condition, Task_PriorityCompare);
    }
    else
    {
        List_Insert_Item(&TskCrt_RegList.list, Task_Ptr[group][priority]->item_ptr);
    }

    List_ItemInit(&Task_Ptr[group][priority]->delay_item, &Task_Ptr[group][priority]->delay_info);

    TskCrt_RegList.num++;

    return handle;
}

//Remove func untest
void Task_Remove(Task_Handle Tsk_Hdl)
{
    //convert Tsk_Hdl from uint32_t var to Task Address which we need to delete
    //MMU_Free that memory space
    //erase all data in specificly memory space
    //set Task pointer to Null which we wanted to be delete

    uint8_t remove_group = GET_TASKGROUP_PRIORITY(((Task *)Tsk_Hdl)->priority.Priority);
    uint8_t remove_task = GET_TASKINGROUP_PRIORITY(((Task *)Tsk_Hdl)->priority.Priority);

    ReSet_Task_Data((Task *)Tsk_Hdl);

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    MMU_Free((uint32_t *)((Task *)Tsk_Hdl)->TCB.Stack);
    MMU_Free((uint32_t *)((Task *)Tsk_Hdl)->TCB.Top_Stk_Ptr);
#endif

    MMU_Free((Task *)Tsk_Hdl);
    Task_Ptr[remove_group][remove_task] = NULL;
}

void TaskSystem_Start(void)
{
    Runtime_Config(RUNTIME_TICK_FRQ_40K);
    Runtime_Set_start_Callback(NULL);
    Runtime_Set_stop_Callback(NULL);
    Runtime_Set_tick_Callback(Task_Scheduler); /* 5us cast by calling the scheduler */
    Runtime_Start();

    RuntimeObj_Reset(&TaskSys_StartTime);

    TaskSys_StartTime = Get_CurrentRunningUs();

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)
    NxtRunTsk_Ptr = Task_Get_HighestRank_RdyTask();

    if (NxtRunTsk_Ptr != NULL)
    {
        NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
        NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;

        CurTsk_TCB = NxtTsk_TCB;
    }
#endif

    DrvTimer.ctl(DrvTimer_Counter_SetState, (uint32_t)&SysTimerObj, ENABLE);
    Task_SetPendSVPro();
    Task_TriggerPendSV();

    return true;
}

void Task_SetRunState(Task_Handle Tsk_Handle, TASK_STATE state)
{
    ((Task *)Tsk_Handle)->Exec_status.State = state;
}

static Task *Get_TaskInstance(uint8_t group, uint8_t priority)
{
    return Task_Ptr[group][priority];
}

static void Task_Exec(Task *tsk_ptr)
{
    SYSTEM_RunTime time_diff;
    RuntimeObj_Reset(&time_diff);

    tsk_ptr = NxtRunTsk_Ptr;

    while (true)
    {
        if (tsk_ptr->Exec_status.State == Task_Ready)
        {
            tsk_ptr->TskFuncUing_US = 0;

            //when task function execute finish reset ready flag of current task in group
            //code down below
            Task_ClearReady(tsk_ptr);

            //set current running task
            CurRunTsk_Ptr = tsk_ptr;

            if (tsk_ptr->Exec_status.Exec_Times == 0)
            {
                tsk_ptr->Exec_status.Start_Time = Get_CurrentRunningUs();
                tsk_ptr->Exec_status.Exec_Time = tsk_ptr->Exec_status.Start_Time;
            }

            tsk_ptr->Exec_status.State = Task_Running;

            //execute task funtion
            tsk_ptr->Exec_Func(*&tsk_ptr);

            //record task running times
            tsk_ptr->Exec_status.Exec_Times++;

            //get max task execut time
            if (tsk_ptr->TskFuncUing_US > tsk_ptr->Exec_status.detect_exec_time_max)
            {
                tsk_ptr->Exec_status.detect_exec_time_max = tsk_ptr->TskFuncUing_US;
            }

            //get task total execute time unit in us
            tsk_ptr->Exec_status.totlal_running_time += tsk_ptr->TskFuncUing_US;
            time_diff = Get_TimeDifference_Between(tsk_ptr->Exec_status.Start_Time, tsk_ptr->Exec_status.Exec_Time);

            tsk_ptr->Exec_status.cpu_opy = tsk_ptr->Exec_status.totlal_running_time / (float)time_diff;
            tsk_ptr->Exec_status.cpu_opy *= 100;

            //get average task running time
            tsk_ptr->Exec_status.detect_exec_time_arv += tsk_ptr->TskFuncUing_US;
            if (tsk_ptr->Exec_status.Exec_Times > 1)
            {
                tsk_ptr->Exec_status.detect_exec_time_arv /= 2;
            }

            tsk_ptr->Exec_status.Exec_Time = Get_TargetRunTime(tsk_ptr->exec_interval_us);

            //get task execute frequence
            if (tsk_ptr->Exec_status.Exec_Times)
            {
                tsk_ptr->Exec_status.detect_exec_frq = (uint32_t)(tsk_ptr->Exec_status.Exec_Times / (float)(tsk_ptr->Exec_status.Exec_Time / 1000000));
            }

            tsk_ptr->Exec_status.State = Task_Stop;

            //erase currnet runnint task pointer
            CurRunTsk_Ptr = NULL;
        }
    }
}

uint32_t Task_Get_IdleUS(void)
{
    return TaskSys_Idle_US;
}

float Task_Get_IdleOcupy(void)
{
    return TaskSys_Idle_Ocupy;
}

void Task_Caller(void)
{
    static uint8_t i = 0;

    //if any task in any group is under ready state
    if (NxtRunTsk_Ptr != NULL)
    {
        i++;
        if (i == TskCrt_RegList.num)
        {
            traverse_start = true;
        }

        //execute task function in function matrix
        Task_Exec(NxtRunTsk_Ptr);
    }
}

void TaskSys_Set_State(TaskSys_State state)
{
    TskSys_state = state;
}

TaskSys_State TaskSys_Get_State(void)
{
    return TskSys_state;
}

static int Task_CrtList_TraversePoll_callback(item_obj *item, void *data, void *arg)
{
    if (data != NULL)
    {
        //get current highest priority task handler AKA NxtRunTsk_Ptr

        if ((TskSys_state == TaskSys_Start) &&
            (((Task *)data)->Exec_status.State == Task_Stop) &&
            (!RuntimeObj_CompareWithCurrent(((Task *)data)->Exec_status.Exec_Time)))
        {
            Task_SetReady((Task *)data);
        }
    }

    return 0;
}

void Task_Scheduler(void)
{
    static uint8_t grp = 0;
    static uint8_t tsk = 0;

    if (TskSys_state != TaskSys_Start)
        return;

    if (traverse_start)
        List_traverse(&TskCrt_RegList.list, Task_CrtList_TraversePoll_callback, NULL, sub_callback);

    NxtRunTsk_Ptr = Task_Get_HighestRank_RdyTask();

#if (TASK_SCHEDULER_TYPE == PREEMPTIVE_SCHDULER)

    PndHstTsk_Ptr = Task_Get_HighestRank_PndTask();

    if (CurRunTsk_Ptr == NULL)
    {
        if (PndHstTsk_Ptr == NULL)
        {
            //Current Run task Pointer is Null also no task In pending mode
            //set CurRunTsk_Ptr to NxtRunTsk_Ptr
            if (NxtRunTsk_Ptr != NULL)
            {
                NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
                NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;

                Task_TriggerPendSV();
                return;
            }
            else
            {
                //doing idle function
                Task_Idle();
            }
        }
        else
        {
            if (NxtRunTsk_Ptr != NULL)
            {
                //if has NxtRunTsk_Ptr is not NULL also have task in pending state
                //then get the highest priority task in pend list
                //compare with NxtRunTsk_Ptr
                if (((NxtRunTsk_Ptr != PndHstTsk_Ptr) &&
                     (Task_PriorityCompare(NxtRunTsk_Ptr, PndHstTsk_Ptr) == PndHstTsk_Ptr)) ||
                    (NxtRunTsk_Ptr == PndHstTsk_Ptr))
                {
                    Task_ClearPending(PndHstTsk_Ptr);
                    PndHstTsk_Ptr->Exec_status.State = Task_Stop;
                    NxtRunTsk_Ptr = PndHstTsk_Ptr;
                }
            }
            else
            {
                //if NxtRunTsk_Ptr is NULL
                //then set NxtRunTsk_Ptr PndHstTsk_Ptr
                NxtRunTsk_Ptr = PndHstTsk_Ptr;
            }

            NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
            NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;

            Task_TriggerPendSV();
            return;
        }
    }
    else
    {
        if (CurRunTsk_Ptr != NxtRunTsk_Ptr)
        {
            if ((NxtRunTsk_Ptr != NULL) && (Task_PriorityCompare(CurRunTsk_Ptr, NxtRunTsk_Ptr) == NxtRunTsk_Ptr))
            {
                //if NxtRunTsk_Ptr group priority is higher then CurRunTsk_Ptr also PndHstTsk_Ptr
                //set current run task as pending state first
                Task_ClearReady(CurRunTsk_Ptr);
                Task_SetPending(CurRunTsk_Ptr);

                //get the highset priority task in pending list
                //if PndHstTsk_Ptr is not NULL
                //then compare the priority between the NxtRunTsk_Ptr and PndHstTsk_Ptr
                if (PndHstTsk_Ptr != NULL)
                {
                    if (Task_PriorityCompare(PndHstTsk_Ptr, NxtRunTsk_Ptr) == PndHstTsk_Ptr)
                    {
                        Task_ClearPending(PndHstTsk_Ptr);
                        PndHstTsk_Ptr->Exec_status.State = Task_Stop;
                        NxtRunTsk_Ptr = PndHstTsk_Ptr;
                    }
                }

                NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
                NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;

                Task_TriggerPendSV();
                return;
            }
        }
        else
        {
            //compare with the highest priority pending task
            if ((PndHstTsk_Ptr != NULL) && (Task_PriorityCompare(CurRunTsk_Ptr, PndHstTsk_Ptr) == PndHstTsk_Ptr))
            {
                //if PndHstTsk_Ptr group priority is higher then CurRunTsk_Ptr
                //set current run task as pending state first
                Task_SetPending(CurRunTsk_Ptr);
                Task_SetReady(PndHstTsk_Ptr);
                NxtRunTsk_Ptr = Task_Stop;

                NxtRunTsk_Ptr = PndHstTsk_Ptr;
            }

            NxtTsk_TCB.Top_Stk_Ptr = &NxtRunTsk_Ptr->TCB.Top_Stk_Ptr;
            NxtTsk_TCB.Stack = NxtRunTsk_Ptr->TCB.Stack;

            Task_TriggerPendSV();
            return;
        }
    }
#else
    Task_TriggerPendSV();
#endif
}

bool Task_GetInfo_ByIndex(uint8_t index, Task_Base_Info *info)
{
    item_obj *task_tmp;

    if (index > TskCrt_RegList.num)
        return false;

    task_tmp = &(TskCrt_RegList.list);

    for (uint8_t i = 0; i < index; i++)
    {
        if (task_tmp == NULL)
            return false;

        task_tmp = task_tmp->nxt;
    }

    info->name = TaskHandleToTaskObj(task_tmp->data)->Task_name;
    info->group = GET_TASKGROUP_PRIORITY(TaskHandleToTaskObj(task_tmp->data)->priority.Priority);
    info->priority = GET_TASKINGROUP_PRIORITY(TaskHandleToTaskObj(task_tmp->data)->priority.Priority);
    info->stk_depth = TaskHandleToTaskObj(task_tmp->data)->Stack_Depth;
    info->set_frq = TaskHandleToTaskObj(task_tmp->data)->exec_frq;
    info->exec_frq = TaskHandleToTaskObj(task_tmp->data)->Exec_status.detect_exec_frq;
    info->cpu_opy = TaskHandleToTaskObj(task_tmp->data)->Exec_status.cpu_opy;
    info->tsk_hdl = task_tmp->data;

    return true;
}

Task *Task_GetCurrentRunTask(void)
{
    return CurRunTsk_Ptr;
}

static void Task_Statistic_Cast(uint8_t *time_base, uint16_t unuse)
{
    if ((CurRunTsk_Ptr != NULL) && (CurRunTsk_Ptr->Exec_status.State == Task_Running))
    {
        CurRunTsk_Ptr->TskFuncUing_US += 1;
    }
}

uint32_t Task_GetStackRemain(const Task_Handle hdl)
{
    uint32_t remain_size = 0;

    __asm("cpsid i");

    while ((uint8_t)(((Task *)hdl)->TCB.Stack + remain_size) == TASK_STACK_DEFAULT)
    {
        remain_size++;
    }

    __asm("cpsie i");

    return (remain_size / 4);
}
