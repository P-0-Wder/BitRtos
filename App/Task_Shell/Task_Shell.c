#include "Task_Shell.h"
#include "drv_serial.h"
#include "shell.h"

static TaskShell_State_List TaskState = Shell_State_Init;

void TaskShell_Core(Task_Handler self)
{
    switch ((uint8_t)TaskState)
    {
    case Shell_State_Init:
        Shell_Init();
        break;

    case Shell_State_Polling:
        break;

    default:
        break;
    }
}