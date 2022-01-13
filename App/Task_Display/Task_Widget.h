#ifndef __TASK_WIDGET_H
#define __TASK_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "widget_mng.h"
#include "task_manager.h"

typedef enum
{
    Create_Widget_NoEror = 0,
    Create_BootWidget_Error,
    Create_SysWidget_Error,
    Create_TFCardWidget_Error,
    Create_AppWidget_Error,
} TaskWiget_Error_List;

typedef enum
{
    WidgetDsp_BootLogo = 0,
    WidgetDsp_SysInfo,
    WidgetDsp_TFCardInfo,
    WidgetDsp_AppInfo,
    WidgetDsp_Sum,
} DspWidget_TypeList;

typedef enum
{
    Widget_Stage_Init = 0,
    Widget_Stage_BootDsp,
    Widget_Stage_Run,
    Widget_Stage_Error,
} TaskWidget_Stage_TypeList;

void TaskWidget_Core(Task_Handle self);

extern Task_Handle Widget_Tsk_Hdl;
#endif
