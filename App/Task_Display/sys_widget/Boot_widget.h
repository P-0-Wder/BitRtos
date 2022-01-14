#ifndef __BOOT_WIDGET_H
#define __BOOT_WIDGET_H

#include <stdbool.h>
#include "widget_mng.h"

typedef enum
{
    Boot_Page_8Bit = 0,
    Boot_Page_BitRtos,
    Boot_Page_Sum,
} Boot_Page_List;

typedef enum
{
    Boot_State_Init = 0,
    Boot_State_Dsp,
    Boot_Stage_DspDone,
    Boot_State_Error,
} BootDsp_State_List;

BootDsp_State_List BootDsp_Ctl(Widget_Handle hdl);

#endif
