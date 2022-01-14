#include "Sys_widget.h"
#include "widget_mng.h"

static SysDsp_Stage_List stage = SysDsp_Stage_WidgetInit;

static bool SysWidget_Init(void)
{

    return true;
}

SysDsp_Stage_List SysWidget_DspUpdate(void)
{
    switch (stage)
    {
    case SysDsp_Stage_WidgetInit:
        if (SysWidget_Init())
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
