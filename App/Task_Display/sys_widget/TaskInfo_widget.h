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
    Stage_DspExit,
    Stage_DspTaskName,
    Stage_DspTaskInfo,
    Stage_DspError,
    Stage_Unknow,
    Stage_Sum,
} TaskInfo_DspStage_List;

void TaskInfo_DspRefresh(int32_t val);
TaskInfo_DspStage_List TaskInfo_DspUpdate(Widget_Handle hdl);
#endif
