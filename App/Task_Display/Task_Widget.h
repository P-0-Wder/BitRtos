#ifndef __TASK_WIDGET_H
#define __TASK_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include "task_manager.h"

typedef enum
{
    Boot_Page_8Bit = 0,
    Boot_Page_BitRtos,
    Boot_Page_ELRS,
    Boot_Page_Sum,
} WidgetBoot_Page_List;

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
    TaskWdiget_State_Init,
    TaskWidget_State_CheckFresh,
    TaskWidget_State_Fresh,
} TaskWidget_State_List;

void TaskWidget_Core(Task_Handler self);

extern Task_Handler Widget_Tsk_Hdl;

#endif
