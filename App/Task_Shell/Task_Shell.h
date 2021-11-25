#ifndef __TASK_SHELL_H
#define __TASK_SHELL_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "task_manager.h"

typedef enum
{
    Shell_State_Init = 0,
    Shell_State_Polling,
} TaskShell_State_List;

bool TaskShell_Init(void);
void TaskShell_Core(Task_Handler self);

extern Task_Handler Shell_Tsk_Hdl;

#endif