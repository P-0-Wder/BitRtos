#ifndef __TASKINFO_WIDGET_H
#define __TASKINFO_WIDGET_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "widget_mng.h"

typedef enum
{
    Stage_CreateWidget = 0,
    Stage_GetTaskInfo,
    Stage_DspTaskName,
    Stage_DspTaskInfo,
    Stage_DspBack,
    Stage_DspError,
    Stage_Unknow,
    Stage_Sum,
} TaskInfo_DspStage_List;

bool TaskInfo_SetStage(int8_t *offset);
TaskInfo_DspStage_List TaskInfo_DspUpdate(Widget_Handle hdl);
#endif
