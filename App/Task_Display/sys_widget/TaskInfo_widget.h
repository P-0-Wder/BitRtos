#ifndef __TASKINFO_WIDGET_H
#define __TASKINFO_WIDGET_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "widget_mng.h"

typedef enum
{
    InfoDspStage_CreateWidget = 0,
    InfoDspStage_GetTaskInfo,
    InfoDspStage_DspExit,
    InfoDspStage_DspTaskName,
    InfoDspStage_DspTaskInfo,
    InfoDspStage_DspError,
    InfoDspStage_Unknow,
    InfoDspStage_Sum,
} TaskInfo_DspStage_List;

void TaskInfo_DspRefresh(int32_t val);
TaskInfo_DspStage_List TaskInfo_DspUpdate(Widget_Handle hdl, int8_t *encoder_in);
#endif
