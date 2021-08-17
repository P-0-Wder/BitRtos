#ifndef __TASK_WIDGET_H
#define __TASK_WIDGET_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    TaskWdiget_State_Init,
    TaskWidget_State_CreatedWidget,
    TaskWidget_State_Run,
    TaskWidget_State_Fresh,
} TaskWidget_State_List;

void TaskWidget_Core(Task_Handler self);

#endif
