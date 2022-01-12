#include "Task_Widget.h"
#include "Task_Input.h"

Task_Handle Widget_Tsk_Hdl = 0;

static Widget_Handle MainWidget_Hdl = 0;
static Widget_Handle BootWidget_Hdl = 0;
static Widget_Handle SysWidget_Hdl = 0;
static Widget_Handle TFCardWidget_Hdl = 0;
static Widget_Handle AppWidget_Hdl = 0;
static uint8_t Main_Selector = 0;
static Encoder_Data_TypeDef Encoder;

static TaskWidget_Stage_TypeList stage = Widget_Stage_Init;

static void TaskWidget_FreshInputVal(void)
{
    Encoder_Data_TypeDef CurEncoder_tmp = TaskInput_GetData()->Enc_Val;

    Main_Selector = CurEncoder_tmp.val - Encoder.val;
    Encoder = CurEncoder_tmp;
}

static bool TaskWidget_Init(void)
{
    MainWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Main", false);
    MainWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Main", false);
    MainWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Main", false);
    MainWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Main", false);
    MainWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Main", false);

    Encoder = TaskInput_GetData()->Enc_Val;
}

void TaskWidget_Core(Task_Handle self)
{
    switch (stage)
    {
    case Widget_Stage_Init:
        break;

    case Widget_Stage_Run:
        TaskWidget_FreshInputVal();
        break;

    default:
        break;
    }
}
