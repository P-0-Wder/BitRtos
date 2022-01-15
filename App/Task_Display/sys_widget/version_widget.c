#include "version_widget.h"

VersionWidget_DspStage_List stage = VersionDspStage_Init;
static Widget_Handle VersionWidget_Handle = 0;
static UI_TriggerLabel_Handle ver_boot_LabelHandle = 0;
static UI_TriggerLabel_Handle ver_RTOS_LabelHandle = 0;
static UI_TriggerLabel_Handle ver_App_LabelHandle = 0;
static UI_TriggerLabel_Handle ver_Back_LabelHandle = 0;

static void VersionWidget_BackLabel_Callback(void)
{
}

static bool VersionWidget_CreateUICtl(Widget_Handle hdl)
{
    uint8_t y_offset = 0;
    if (hdl == 0)
        return false;

    ver_boot_LabelHandle = Widget_Mng.Control(hdl)->UI()->TriggerLabel()->create("Boot    Ver: 0.0.0", 0, y_offset);
    if (ver_boot_LabelHandle == 0)
        return false;

    y_offset += UICTL_TRIGGERLABEL_HEIGHT;
    ver_RTOS_LabelHandle = Widget_Mng.Control(hdl)->UI()->TriggerLabel()->create("BitRTOS Ver: 0.0.1", 0, y_offset);
    if (ver_RTOS_LabelHandle == 0)
        return false;

    y_offset += UICTL_TRIGGERLABEL_HEIGHT;
    ver_App_LabelHandle = Widget_Mng.Control(hdl)->UI()->TriggerLabel()->create("App     Ver: 0.0.1", 0, y_offset);
    if (ver_App_LabelHandle == 0)
        return false;

    y_offset += UICTL_TRIGGERLABEL_HEIGHT;
    ver_Back_LabelHandle = Widget_Mng.Control(hdl)->UI()->TriggerLabel()->create("Back", 0, y_offset);
    if (ver_Back_LabelHandle == 0)
        return false;

    Widget_Mng.Control(hdl)->UI()->TriggerLabel()->set_callback(ver_Back_LabelHandle, VersionWidget_BackLabel_Callback);

    return true;
}

static bool VersionWidget_Init(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    VersionWidget_Handle = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "version", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if ((VersionWidget_Handle == WIDGET_CREATE_ERROR) || !VersionWidget_CreateUICtl(VersionWidget_Handle))
        return false;

    return true;
}

static void VersionWidget_Fresh(Widget_Handle hdl)
{
    Widget_Mng.Control(hdl)->Clear();

    Widget_Mng.Control(hdl)->Show();
}

VersionWidget_DspStage_List VersionWidget_Update(Widget_Handle hdl)
{
    switch (stage)
    {
    case VersionDspStage_Init:
        if (VersionWidget_init(hdl))
            stage = VersionDspStage_Update;
        else
            stage = VersionDspStage_Error;

    case VersionDspStage_Update:
        return VersionDspStage_Update;

    case VersionDspStage_Error:
        return VersionDspStage_Error;

    default:
        stage = VersionDspStage_Error;
        return VersionDspStage_Error;
    }
}