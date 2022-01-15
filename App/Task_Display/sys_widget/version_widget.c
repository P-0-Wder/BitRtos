#include "version_interface.h"
#include "version_widget.h"

VersionWidget_DspStage_List stage = VersionDspStage_Init;
static Widget_Handle VersionWidget_Handle = 0;

static bool VersionWidget_Init(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    VersionWidget_Handle = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "version widget", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if (VersionWidget_Handle == WIDGET_CREATE_ERROR)
        return false;

    return true;
}

static bool VersionWidget_CreateUICtl(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    Widget_Mng.Control(hdl)->UI()->;

    return true;
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