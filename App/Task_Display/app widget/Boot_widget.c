#include "Boot_widget.h"
#include "widget_mng.h"

static BootDsp_State_List Boot_Stage = Boot_State_Init;
static Boot_Page_List Cur_DspPage = Boot_Page_8Bit;
static Widget_Handle Dsp_Handle;

static bool Boot_Init_Page(void)
{
}

static Boot_Page_List Boot_Show_8Bit_Logo(void)
{
    return Boot_Page_8Bit + 1;
}

static Boot_Page_List Boot_Show_BitRTOS_Logo(void)
{
    return Boot_Page_BitRtos + 1;
}

static Boot_Page_List Boot_Show_ELRS_Logo(void)
{

    return Boot_Page_ELRS + 1;
}

bool BootDsp_Ctl(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    switch (Boot_Stage)
    {
    case Boot_State_Init:
        if (Boot_Init_Page())
            Boot_Stage = Boot_State_Dsp;
        break;

    case Boot_State_Dsp:
        switch (Cur_DspPage)
        {
        case Boot_Page_8Bit:
            Cur_DspPage = Boot_Show_8Bit_Logo();
            break;

        case Boot_Page_BitRtos:
            Cur_DspPage = Boot_Show_BitRTOS_Logo();
            break;

        case Boot_Page_ELRS:
            Cur_DspPage = Boot_Show_ELSR_Logo();
            break;

        case Boot_Page_Sum:
            return true;

        default:
            return false;
        }
        break;

    default:
        return false;
    }

    return true;
}
