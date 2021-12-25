#include "Boot_widget.h"
#include "widget_mng.h"

BootDsp_State_List Boot_Stage = Boot_State_Init;
Boot_Page_List Cur_DspPage = Boot_Page_8Bit;

static void Boot_Init_Page(void)
{
}

static Boot_Page_List Boot_Show_8Bit_Logo(void)
{
}

static Boot_Page_List Boot_Show_BitRTOS_Logo(void)
{
}

static Boot_Page_List Boot_Show_ELRS_Logo(void)
{
}

bool BootDsp_Ctl(void)
{
    switch (Boot_Stage)
    {
    case Boot_State_Init:
        Boot_Init_Page();
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
