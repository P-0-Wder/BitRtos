#include "Task_Widget.h"
#include "Boot_widget.h"
#include "Task_Input.h"

Task_Handle Widget_Tsk_Hdl = 0;

#define WidgetSelect_TimeDiff 500

static Widget_Handle ManuWidget_Hdl = 0;
static Widget_Handle BootWidget_Hdl = 0;
static Widget_Handle SysWidget_Hdl = 0;
static Widget_Handle TFCardWidget_Hdl = 0;
static Widget_Handle AppWidget_Hdl = 0;
static Widget_Handle Cur_Widget = 0;

static Encoder_Data_TypeDef Encoder;

static TaskWidget_Stage_TypeList stage = Widget_Stage_Init;
static TaskWiget_Error_List Task_Error = Create_Widget_NoEror;

static SYSTEM_RunTime EncoderBtnTrigger_Rt = 0;
static bool show_manu = false;
static TaskWidget_Manu_UI_TypeDef Manu_UI;

#define TaskWidget_FreshInputVal() TaskInput_GetData()->Enc_Val

static void TaskWidget_JumpTo_AppWidget(void)
{
    Cur_Widget = AppWidget_Hdl;
    show_manu = false;
}

static void TaskWidget_JumpTo_SysInfoWidget(void)
{
    Cur_Widget = SysWidget_Hdl;
    show_manu = false;
}

static void TaskWidget_JumpTo_TFCardWidget(void)
{
    Cur_Widget = TFCardWidget_Hdl;
    show_manu = false;
}

static bool TaskWidget_InitManu(void)
{
    int8_t UI_Pos = 10;

    if (ManuWidget_Hdl == WIDGET_CREATE_ERROR)
        return false;

    Manu_UI.Label_SysInfo = Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->create(HandleToWidgetObj(SysWidget_Hdl)->name, 0, UI_Pos);
    Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->set_callback(Manu_UI.Label_SysInfo, TaskWidget_JumpTo_SysInfoWidget);
    UI_Pos += UICTL_DEFAULT_HEIGHT;

    Manu_UI.Label_TFCard = Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->create(HandleToWidgetObj(TFCardWidget_Hdl)->name, 0, UI_Pos);
    Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->set_callback(Manu_UI.Label_TFCard, TaskWidget_JumpTo_TFCardWidget);
    UI_Pos += UICTL_DEFAULT_HEIGHT;

    Manu_UI.Label_App = Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->create(HandleToWidgetObj(AppWidget_Hdl)->name, 0, UI_Pos);
    Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->set_callback(Manu_UI.Label_App, TaskWidget_JumpTo_AppWidget);

    Manu_UI.selector = 0;
    return true;
}

static TaskWiget_Error_List TaskWidget_Init(void)
{
    BootWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "Boot", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (BootWidget_Hdl == WIDGET_CREATE_ERROR)
        return Create_BootWidget_Error;

    SysWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "SysInfo", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if (SysWidget_Hdl == WIDGET_CREATE_ERROR)
        return Create_SysWidget_Error;

    TFCardWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "TFCard", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    if (TFCardWidget_Hdl == WIDGET_CREATE_ERROR)
        return Create_TFCardWidget_Error;

    AppWidget_Hdl = Widget_Mng.Create(0, 0, 128, 64, "App", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (AppWidget_Hdl == WIDGET_CREATE_ERROR)
        return Create_AppWidget_Error;

    ManuWidget_Hdl = Widget_Mng.Create(29, 10, 70, 42, "Manu", SHOW_WIDGET_FRAME, HIDE_WIDGET_NAME);
    if (ManuWidget_Hdl == WIDGET_CREATE_ERROR)
        return Create_ManuWidget_Error;

    if (!TaskWidget_InitManu())
        return Create_ManuWidget_Error;

    Encoder.val = TaskInput_GetData()->Enc_Val.val;
    Encoder.btn = false;

    return Create_Widget_NoEror;
}

static void EncoderPush_Callback(void)
{
    EncoderBtnTrigger_Rt = Get_CurrentRunningMs();
    Encoder.btn = true;
}

static void EncoderRelease_Callback(void)
{
    Encoder.btn = false;
}

static bool TaskWidget_ShowManu(int8_t val)
{
    if (Encoder.btn)
    {
        if (TaskInput_GetCurEncoderBtn_Level())
        {
            if (!show_manu && Get_CurrentRunningMs() - EncoderBtnTrigger_Rt >= WidgetSelect_TimeDiff)
            {
                show_manu = true;

                EncoderBtnTrigger_Rt = Get_CurrentRunningMs();
                Encoder.btn = false;
            }
        }
        else
            EncoderBtnTrigger_Rt = Get_CurrentRunningMs();
    }

    if (show_manu)
    {
        Manu_UI.selector = val;

        Widget_Mng.Control(ManuWidget_Hdl)->Clear();
        Widget_Mng.Control(ManuWidget_Hdl)->Draw()->draw_str(Font_8, HandleToWidgetObj(ManuWidget_Hdl)->name, (HandleToWidgetObj(ManuWidget_Hdl)->width - strlen(HandleToWidgetObj(ManuWidget_Hdl)->name) * STR_DIS) / 2, 0, true);

        if (Encoder.btn)
        {
            Widget_Mng.Control(ManuWidget_Hdl)->UI()->TriggerLabel()->trigger(Widget_Mng.Control(ManuWidget_Hdl)->UI()->Get_CurSelected_UI());
            Encoder.btn = false;
        }

        Widget_Mng.Control(ManuWidget_Hdl)->UI()->Show_Selector(&Manu_UI.selector);
        Widget_Mng.Control(ManuWidget_Hdl)->UI()->Fresh();
        Widget_Mng.Control(ManuWidget_Hdl)->Show();
    }

    if ((Cur_Widget != AppWidget_Hdl) || !show_manu)
        Widget_Mng.Control(ManuWidget_Hdl)->Hide();

    return show_manu;
}

static uint8_t TaskWidget_UpdateDsp(int8_t val)
{
    BootDsp_State_List BootDsp_Stage = BootDsp_Ctl(BootWidget_Hdl);

    if (BootDsp_Stage == Boot_Stage_DspDone)
    {
        if (BootWidget_Hdl)
        {
            Widget_Mng.Delete(&BootWidget_Hdl);
            Cur_Widget = AppWidget_Hdl;
        }

        if (Cur_Widget == AppWidget_Hdl)
        {
            if (!TaskWidget_ShowManu(val))
            {
                TaskInput_SetCallback(DevEncoderBtn_Push_Callback, EncoderPush_Callback);
                TaskInput_SetCallback(DevEncoderBtn_Release_Callback, EncoderRelease_Callback);
            }
        }
        else if (Cur_Widget == TFCardWidget_Hdl)
        {
            Widget_Mng.Control(TFCardWidget_Hdl)->Clear();
            Widget_Mng.Control(TFCardWidget_Hdl)->Show();
        }
        else if (Cur_Widget == SysWidget_Hdl)
        {
            Widget_Mng.Control(SysWidget_Hdl)->Clear();
            Widget_Mng.Control(SysWidget_Hdl)->Show();
        }
    }

    return 0;
}

void TaskWidget_Core(Task_Handle self)
{
    int8_t val;

    switch (stage)
    {
    case Widget_Stage_Init:
        Task_Error = TaskWidget_Init();

        if (Task_Error == Create_Widget_NoEror)
        {
            stage = Widget_Stage_Run;
        }
        else
            stage = Widget_Stage_Error;
        break;

    case Widget_Stage_Run:
        val = Encoder.val - TaskWidget_FreshInputVal().val;
        Encoder.val = TaskWidget_FreshInputVal().val;
        TaskWidget_UpdateDsp(val);

    case Widget_Stage_CheckFresh:
        if (Widget_Mng.trigger_fresh())
            stage = Widget_Stage_Fresh;
        break;

    case Widget_Stage_Fresh:
        Widget_Mng.fresh_all();
        stage = Widget_Stage_Run;
        break;

    case Widget_Stage_Error:
        break;

    default:
        break;
    }
}
