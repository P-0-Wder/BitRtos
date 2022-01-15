#include "Sys_widget.h"
#include "TaskInfo_widget.h"
#include "version_widget.h"
#include "widget_mng.h"

static SysDsp_Stage_List stage = SysDsp_Stage_WidgetInit;
static Widget_Handle SysWidget_Handle = 0;
static UI_TriggerLabel_Handle VersionLabel_Handle = 0;
static UI_TriggerLabel_Handle TaskInfoLabel_Handle = 0;
static UI_TriggerLabel_Handle BackLabel_Handle = 0;

static void VersionLabel_Trigger_Callback(void)
{
}

static void TaskInfoLabel_Trigger_Callback(void)
{
}

static void BackLabel_Trigger_Callback(void)
{
}

static bool SysWidget_Init(Widget_Handle hdl)
{
    uint8_t y_offset = 0;

    /* ctreate widget and controller */
    SysWidget_Handle = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "System", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if (SysWidget_Handle == WIDGET_CREATE_ERROR)
        return false;

    VersionLabel_Handle = Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->create("version", 0, y_offset);
    if (VersionLabel_Handle == NULL)
        return false;

    Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->set_callback(VersionLabel_Handle, VersionLabel_Trigger_Callback);

    y_offset += UICTL_TRIGGERLABEL_HEIGHT;
    TaskInfoLabel_Handle = Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->create("Task Info", 0, y_offset);
    if (TaskInfoLabel_Handle == NULL)
        return false;

    Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->set_callback(TaskInfoLabel_Handle, TaskInfoLabel_Trigger_Callback);

    y_offset += UICTL_TRIGGERLABEL_HEIGHT;
    BackLabel_Handle = Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->create("Back", 0, y_offset);
    if (BackLabel_Handle == NULL)
        return false;

    Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->set_callback(BackLabel_Handle, BackLabel_Trigger_Callback);

    return true;
}

static void SysWidget_Fresh(void)
{
}

SysDsp_Stage_List SysWidget_DspUpdate(Widget_Handle hdl, int8_t *encoder_in)
{
    switch (stage)
    {
    case SysDsp_Stage_WidgetInit:
        if (SysWidget_Init(hdl))
            stage = SysDsp_Stage_Update;
        else
            stage = SysDsp_Stage_Error;
        return stage;

    case SysDsp_Stage_Update:
        break;

    default:
        stage = SysDsp_Stage_Error;
        return SysDsp_Stage_Error;
    }
}
