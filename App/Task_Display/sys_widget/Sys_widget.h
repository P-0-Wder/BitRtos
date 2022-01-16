#ifndef __SYS_WIDGET_H
#define __SYS_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "widget_mng.h"

typedef enum
{
    SysDspStage_WidgetInit = 0,
    SysDspStage_Update,
    SysDspStage_ShowVersion,
    SysDspStage_ShowTaskInfo,
    SysDspStage_ResetCallback,
    SysDspStage_Exit,
    SysDspStage_Wait,
    SysDspStage_Error,
    SysDspStage_Sum,
} SysDsp_Stage_List;

typedef enum
{
    DspVersion_Widget = 0,
    DspSysInfo_Widget = 0,
} SysDsp_Widget_List;

SysDsp_Stage_List SysWidget_DspUpdate(Widget_Handle hdl, int8_t *encoder_in);
void SysWidget_ButtonPush_Callback(void);
void SysWidget_ButtonRelease_Callback(void);

#endif
