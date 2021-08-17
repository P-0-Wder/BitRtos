#include "Task_Widget.h"
#include "task_manager.h"
#include "runtime.h"
#include "widget_mng.h"

/* internal variable */
static TaskWidget_State_List TaskWidget_State = TaskWdiget_State_Init;

/* external function definition */

/* internal function definition */

static void TaskWidget_Init(void)
{
}

void TaskWidget_Core(Task_Handler self)
{
    switch ((uint8_t)TaskWidget_State)
    {
    case TaskWdiget_State_Init:
        break;

    case TaskWidget_State_Fresh:
        break;
    }
}
