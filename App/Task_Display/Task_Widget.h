#ifndef __TASK_WIDGET_H
#define __TASK_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include "task_manager.h"

typedef enum
{
    WidGUI_Boot = 0,
    WidGUI_Version,
    WidGUI_OS_Info,
    WidGUI_BaseSetting,
    WidGUI_FileDisk,
    WidGUI_ModelControl,
    WidGUI_RemoteStatus,
} TaskWidget_GUIFunc_List;

typedef enum
{
    TaskWdiget_State_Init = 0,
    TaskWidget_State_CheckFresh,
    TaskWidget_State_Fresh,
} TaskWidget_State_List;

void TaskWidget_Core(Task_Handler self);

extern Task_Handler Widget_Tsk_Hdl;

#endif
