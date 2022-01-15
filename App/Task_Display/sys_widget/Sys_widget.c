#include "Sys_widget.h"
#include "TaskInfo_widget.h"
#include "version_widget.h"

static SysDsp_Stage_List stage = SysDsp_Stage_WidgetInit;

static bool SysWidget_Init(Widget_Handle hdl)
{
    if (!TaskInfo_DspUpdate(hdl) || !VersionWidget_Update(hdl))
        return false;

    return true;
}

SysDsp_Stage_List SysWidget_DspUpdate(Widget_Handle hdl)
{
    switch (stage)
    {
    case SysDsp_Stage_WidgetInit:
        if (SysWidget_Init(hdl))
        {
            stage = SysDsp_Stage_Update;
        }
        else
        {
            stage = SysDsp_Stage_Error;
        }
        return stage;

    case SysDsp_Stage_Update:
        break;

    default:
        return SysDsp_Stage_Error;
    }
}
