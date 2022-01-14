#ifndef __SYS_WIDGET_H
#define __SYS_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "widget_mng.h"

typedef enum
{
    SysDsp_Stage_WidgetInit = 0,
    SysDsp_Stage_Update,
    SysDsp_Stage_Error,
} SysDsp_Stage_List;

typedef enum
{
    DspVersion_Widget = 0,
    DspSysInfo_Widget = 0,
} SysDsp_Widget_List;

SysDsp_Stage_List SysWidget_DspUpdate(Widget_Handle hdl);

#endif
