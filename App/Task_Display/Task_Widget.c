#include "Task_Widget.h"
#include "Boot_widget.h"
#include "Task_Input.h"

Task_Handle Widget_Tsk_Hdl = 0;

static Widget_Handle BootWidget_Hdl = 0;
static Widget_Handle SysWidget_Hdl = 0;
static Widget_Handle TFCardWidget_Hdl = 0;
static Widget_Handle AppWidget_Hdl = 0;
static uint8_t Main_Selector = 0;
static Encoder_Data_TypeDef Encoder;

static TaskWidget_Stage_TypeList stage = Widget_Stage_Init;
static TaskWiget_Error_List Task_Error = Create_Widget_NoEror;

static void TaskWidget_FreshInputVal(void)
{
    Encoder_Data_TypeDef CurEncoder_tmp = TaskInput_GetData()->Enc_Val;

    Main_Selector = CurEncoder_tmp.val - Encoder.val;
    Encoder = CurEncoder_tmp;
}

static TaskWiget_Error_List TaskWidget_Init(void)
{
    BootWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Boot", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (BootWidget_Hdl == 0)
        return Create_BootWidget_Error;

    SysWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "SysInfo", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if (SysWidget_Hdl == 0)
        return Create_SysWidget_Error;

    TFCardWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "TFCard", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (TFCardWidget_Hdl == 0)
        return Create_TFCardWidget_Error;

    AppWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "App", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (AppWidget_Hdl == 0)
        return Create_AppWidget_Error;

    Encoder = TaskInput_GetData()->Enc_Val;

    return Create_Widget_NoEror;
}

void TaskWidget_Core(Task_Handle self)
{
    switch (stage)
    {
    case Widget_Stage_Init:
        Task_Error = TaskWidget_Init();

        if (Task_Error == Create_Widget_NoEror)
        {
            stage = Widget_Stage_BootDsp;
        }
        else
            stage = Widget_Stage_Error;
        break;

    case Widget_Stage_BootDsp:
        break;

    case Widget_Stage_Run:
        TaskWidget_FreshInputVal();
        break;

    case Widget_Stage_Error:
        break;

    default:
        break;
    }
}
