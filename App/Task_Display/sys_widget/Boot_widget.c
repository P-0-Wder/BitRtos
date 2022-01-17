#include "Boot_widget.h"
#include "widget_mng.h"
#include "runtime.h"

#define CODER_DSP_DURATION 200
#define RTOS_DSP_DURATION 200

static BootDsp_State_List Boot_Stage = Boot_State_Init;
static Boot_Page_List Cur_DspPage = Boot_Page_8Bit;

static Widget_Handle BootLogo_Widget_Hdl = 0;
static Widget_Handle BitRtos_Widget_Hdl = 0;

static bool Boot_Init_Page(Widget_Handle hdl)
{
    BootLogo_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "Boot Logo", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (BootLogo_Widget_Hdl == WIDGET_CREATE_ERROR)
        return false;

    BitRtos_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "BitRtos Logo", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (BitRtos_Widget_Hdl == WIDGET_CREATE_ERROR)
        return false;

    return true;
}

static Boot_Page_List Boot_Show_8Bit_Logo(void)
{
    Boot_Page_List stage;
    static SYSTEM_RunTime Rt = 0;
    const uint16_t duration = CODER_DSP_DURATION;

    if (Rt == 0)
    {
        Rt = Get_CurrentRunningMs();
    }

    Widget_Mng.Control(BootLogo_Widget_Hdl)->Clear();
    Widget_Mng.Control(BootLogo_Widget_Hdl)->Draw()->draw_str(Font_8, "8Bit", (128 - strlen("8Bit") * STR_DIS) / 2, (64 - Font_8) / 2, true);
    Widget_Mng.Control(BootLogo_Widget_Hdl)->Show();

    if (Get_CurrentRunningMs() - Rt >= duration)
    {
        stage = Boot_Page_8Bit + 1;
    }
    else
        stage = Boot_Page_8Bit;

    return stage;
}

static Boot_Page_List Boot_Show_BitRTOS_Logo(void)
{
    Boot_Page_List stage;
    static SYSTEM_RunTime Rt = 0;
    const uint16_t duration = RTOS_DSP_DURATION;

    if (Rt == 0)
    {
        Rt = Get_CurrentRunningMs();
    }

    Widget_Mng.Control(BitRtos_Widget_Hdl)->Clear();
    Widget_Mng.Control(BitRtos_Widget_Hdl)->Draw()->draw_str(Font_8, "BitRtos", (128 - strlen("BitRtos") * STR_DIS) / 2, (64 - Font_8) / 2, true);
    Widget_Mng.Control(BitRtos_Widget_Hdl)->Show();

    if (Get_CurrentRunningMs() - Rt >= duration)
    {
        stage = Boot_Page_BitRtos + 1;
    }
    else
        stage = Boot_Page_BitRtos;

    return stage;
}

BootDsp_State_List BootDsp_Ctl(Widget_Handle hdl)
{
    switch (Boot_Stage)
    {
    case Boot_State_Init:
        if (hdl == 0)
            return Boot_State_Error;

        if (Boot_Init_Page(hdl))
        {
            Boot_Stage = Boot_State_Dsp;
            return Boot_State_Dsp;
        }
        else
        {
            Boot_Stage = Boot_State_Error;
            return Boot_State_Error;
        }

    case Boot_State_Dsp:
        if (hdl == 0)
            return Boot_State_Error;

        switch (Cur_DspPage)
        {
        case Boot_Page_8Bit:
            Cur_DspPage = Boot_Show_8Bit_Logo();
            return Boot_State_Dsp;

        case Boot_Page_BitRtos:
            Cur_DspPage = Boot_Show_BitRTOS_Logo();
            return Boot_State_Dsp;

        case Boot_Page_Sum:
            // Widget_Mng.Control(BootLogo_Widget_Hdl)->Clear();
            // Widget_Mng.Control(BitRtos_Widget_Hdl)->Clear();

            /* delete all widget relative to boot */
            Widget_Mng.DeleteSub(&BootLogo_Widget_Hdl);
            Widget_Mng.DeleteSub(&BitRtos_Widget_Hdl);

            Boot_Stage = Boot_Stage_DspDone;
            return Boot_Stage_DspDone;

        default:
            return Boot_State_Error;
        }
        break;

    case Boot_Stage_DspDone:
        return Boot_Stage_DspDone;

    default:
        return Boot_State_Error;
    }
}
