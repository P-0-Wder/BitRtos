/*
* Coder : 8_B!T0
*
* BitRTOS Core Ticker Code Module
*
*/
#include "runtime.h"

/* internal variable */
static Runtime_DataObj_TypeDef RunTime;
static runtime_stop_p Runtime_Stop_FuncPtr = NULL;

void Runtime_PreInit(runtime_start_callback_p start_cb, runtime_stop_callback_p stop_cb, runtime_timer_init_p timer_init_cb)
{
    RunTime.start_callback = start_cb;
    RunTime.stop_callback = stop_cb;
    RunTime.timer_init = timer_init_cb;
}

Runtime_ModuleState_List Get_RuntimeState(void)
{
    return RunTime.module_state;
}

bool Runtime_Config(uint32_t tick_frq)
{
    RunTime.module_state = Runtime_Module_Init;
    RunTime.time_base = RUNTIEM_MAX_TICK_FRQ / tick_frq;
    RunTime.tick_frq = tick_frq;

    if(RunTime.timer_init != NULL)
    {
        return RunTime.timer_init(RunTime.tick_frq);
    }

    return false;
}

void Runtime_Start(void)
{
    RunTime.module_state = Runtime_Module_Start;

    if(RunTime.start_callback != NULL)
    {
        RunTime.start_callback();
    }
}

bool Runtime_Stop(void)
{
    if(Runtime.tick_state != Runtime_Run_Tick)
    {
        RunTime.module_state = Runtime_Module_Stop;

        if(RunTime.stop_callback != NULL)
        {
            RunTime.stop_callback();
        }

        return true;
    }

    Runtime_Stop_FuncPtr = Runtme_Stop;
    return false;
}

bool Runtime_Tick(void)
{
    if(Runtime.module_state != Runtime_Module_Start)
    {
        Runtime.tick_state = Runtime_Run_Tick;

        Runtime.Use_Us += Runtime.time_base;

        Runtime.tick_state = Runtime_Run_Wait;

        if(Runtime_Stop_FuncPtr != NULL)
        {
            Runtime_Stop_FuncPtr();
            Runtime_Stop_FuncPtr = NULL;
        }

        return true;
    }
    
    return false;
}

uint64_t Get_CurrentRunningUs(void)
{
    return Runtime.Use_Us;
}

uint64_t Get_CurrentRunningMs(void)
{
    return (Runtime.Use_Us / REAL_MS);
}

uint64_t Get_CurrentRunningS(void)
{
    return (Get_CurrentRunningMs() / REAL_MS);
}

uint64_t Get_TimeDifference(uint64_t time_in)
{
    return (Runtime.Use_Us - time_in);
}

uint64_t Get_TargetRunTime(uint16_t duration)
{
    return (Runtime.Use_Us + duration);
}

/* return the object addr for the longer one */
/* if EQ_L equal to EQ_R return null pointer */
uint32_t RuntimeObj_Compare(const uint64_t *EQ_L, const uint64_t *EQ_R)
{
    if(*EQ_L > *EQ_R)
    {
        return EQ_L;
    }
    else if(*EQ_L < *EQ_R)
    {
        return EQ_R;
    }

    return NULL;
}

/* input time object compare with current runtime */
/* if input time object >= current runtime return true */
bool RuntimeObj_CompareWithCurrent(const uint64_t time_in)
{
    if(time_in >= runtime.Use_Us)
    {
        return true;
    }

    return false;
}

