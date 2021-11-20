#ifndef __DSPUTILBOX_H
#define __DSPUTILBOX_H

#include <stdbool.h>
#include <stdint.h>

#define DEFAULT_MASSAGEBOX_X
#define DEFAULT_MASSAGEBOX_Y
#define DEFAULT_MASSAGEBOX_WIDTH
#define DEFAULT_MASSAGEBOX_HEIGHT

#define YES_BUTTON_X
#define YES_BUTTON_Y

#define NO_BOTTON_X
#define NO_BUTTON_Y

typedef enum
{
    Dialog_Input_Floating = 0,
    Dialog_Input_No,
    Dialog_Input_Yes,
    Dialog_Input_Next,
    Dialob_Input_Back,
    Dialog_Input_Comfirm,
    Dialog_Input_Cancle,
} Dialog_Opr_State_List;

Dialog_Opr_State_List MassageBox(char *str);
Dialog_Opr_State_List WizerdBox(char *str);

#endif
