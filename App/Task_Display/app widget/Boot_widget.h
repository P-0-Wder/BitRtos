#ifndef __BOOT_WIDGET_H
#define __BOOT_WIDGET_H

#include <stdbool.h>

typedef enum
{
    Boot_Page_8Bit = 0,
    Boot_Page_BitRtos,
    Boot_Page_ELRS,
    Boot_Page_Sum,
} Boot_Page_List;

typedef enum
{
    Boot_State_Init = 0,
    Boot_State_Dsp,
} BootDsp_State_List;

bool BootDsp_Ctl(void);

#endif
