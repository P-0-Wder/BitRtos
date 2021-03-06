#include "Sys_widget.h"
#include "TaskInfo_widget.h"
#include "version_widget.h"
#include "widget_mng.h"
#include "Task_Input.h"
#include "runtime.h"

static SysDsp_Stage_List stage = SysDspStage_WidgetInit;
static Widget_Handle SysWidget_Handle = 0;
static UI_TriggerLabel_Handle VersionLabel_Handle = 0;
static UI_TriggerLabel_Handle TaskInfoLabel_Handle = 0;
static UI_TriggerLabel_Handle BackLabel_Handle = 0;

static void SysWidget_Fresh(int8_t *encoder_in);

static void VersionLabel_Trigger_Callback(void)
{
    Widget_Mng.Control(SysWidget_Handle)->Hide();

    stage = SysDspStage_ShowVersion;
}

static void TaskInfoLabel_Trigger_Callback(void)
{
    Widget_Mng.Control(SysWidget_Handle)->Hide();

    stage = SysDspStage_ShowTaskInfo;
}

static void BackLabel_Trigger_Callback(void)
{
    Widget_Mng.Control(SysWidget_Handle)->Hide();

    stage = SysDspStage_Exit;
}

static bool SysWidget_Init(Widget_Handle hdl, int8_t *encoder_in)
{
    int16_t y_offset = 0;

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

    SysWidget_Fresh(encoder_in);

    return true;
}

static void SysWidget_Fresh(int8_t *encoder_in)
{
    Widget_Mng.Control(SysWidget_Handle)->Clear();
    Widget_Mng.Control(SysWidget_Handle)->UI()->Show_Selector(encoder_in);
    Widget_Mng.Control(SysWidget_Handle)->UI()->Fresh();
    Widget_Mng.Control(SysWidget_Handle)->Show();
}

SysDsp_Stage_List SysWidget_DspUpdate(Widget_Handle hdl, int8_t *encoder_in, bool *btn)
{
    VersionWidget_DspStage_List VersionWidget_state;
    TaskInfo_DspStage_List TaskInfoWidget_state;

    switch (stage)
    {
    case SysDspStage_WidgetInit:
        if (SysWidget_Init(hdl, encoder_in))
            stage = SysDspStage_Update;
        else
            stage = SysDspStage_Error;
        return SysDspStage_WidgetInit;

    case SysDspStage_Update:
        if (*btn)
        {
            Widget_Mng.Control(SysWidget_Handle)->UI()->TriggerLabel()->trigger(Widget_Mng.Control(SysWidget_Handle)->UI()->Get_CurSelected_UI());
            *btn = false;
        }

        SysWidget_Fresh(encoder_in);
        return SysDspStage_Update;

    case SysDspStage_ShowTaskInfo:
        TaskInfoWidget_state = TaskInfo_DspUpdate(SysWidget_Handle, encoder_in, btn);

        if (TaskInfoWidget_state == InfoDspStage_DspExit)
        {
            stage = SysDspStage_Update;
            return SysDspStage_Update;
        }
        else if (TaskInfoWidget_state == InfoDspStage_DspError)
        {
            stage = SysDspStage_Error;
            return SysDspStage_Error;
        }
        else if (TaskInfoWidget_state == InfoDspStage_DspExit)
        {
            stage = SysDspStage_Update;
            Widget_Mng.Control(SysWidget_Handle)->UI()->Reset_SelectUICtl();
            return SysDspStage_Update;
        }

        return SysDspStage_ShowTaskInfo;

    case SysDspStage_ShowVersion:
        VersionWidget_state = VersionWidget_Update(SysWidget_Handle, encoder_in, btn);

        if (VersionWidget_state == VersionDspStage_Exit)
        {
            stage = SysDspStage_Update;
            return SysDspStage_Update;
        }
        else if (VersionWidget_state == VersionDspStage_Error)
        {
            stage = SysDspStage_Update;
            return SysDspStage_Error;
        }
        else if (VersionWidget_state == VersionDspStage_Exit)
        {
            stage = SysDspStage_Update;
            Widget_Mng.Control(SysWidget_Handle)->UI()->Reset_SelectUICtl();
            return SysDspStage_Update;
        }
        return SysDspStage_ShowVersion;

    case SysDspStage_Exit:
        *btn = false;
        Widget_Mng.Control(SysWidget_Handle)->Clear();
        Widget_Mng.Control(SysWidget_Handle)->UI()->Reset_SelectUICtl();
        stage = SysDspStage_Update;
        return SysDspStage_Exit;

    default:
        return SysDspStage_Error;
    }
}
