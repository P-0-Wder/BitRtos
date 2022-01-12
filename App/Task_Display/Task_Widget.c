#include "Task_Widget.h"
#include "Task_Input.h"

Task_Handle Widget_Tsk_Hdl = 0;

static Widget_Handle BootWidget_Hdl = 0;
static Widget_Handle SysWidget_Hdl = 0;
static Widget_Handle TFCardWidget_Hdl = 0;
static Widget_Handle AppWidget_Hdl = 0;

static Encoder_Data_TypeDef Encoder;

static TaskWidget_Stage_TypeList stage = Widget_Stage_Init;

static bool TaskWifget_Create(void)
{
}

static void TaskWidget_InputValInit(void)
{
    Encoder = TaskInput_GetData()->Enc_Val;
}

static void TaskWidget_FreshInputVal(void)
{
}

static bool TaskWidget_Init(void)
{
    TaskWifget_Create();
}

void TaskWidget_Core(Task_Handle self)
{
    switch (stage)
    {
    case Widget_Stage_Init:
        break;

    case Widget_Stage_Run:
        break;

    default:
        break;
    }
}
