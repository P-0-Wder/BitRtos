#include "Task_TestWidget.h"
#include "task_manager.h"
#include "Boot_widget.h"
#include "OS_widget.h"
#include "version_widget.h"
#include "widget_mng.h"
#include "Task_Input.h"

/* this task only for test */

/* internal variable */
static TaskWidget_State_List TaskWidget_State = TaskWdiget_State_Init;
static Widget_Handle test1;
static Widget_Handle test2;
static Widget_Handle test3;
static Widget_Handle test4;

/* external function definition */
// Task_Handler Widget_Tsk_Hdl = 0;

static UI_Button_Handle test_btn_1 = 0;
static UI_Button_Handle test_btn_2 = 0;
static UI_CheckBox_Handle test_checkbox_1 = 0;
static UI_SlideBar_Handle test_slidebar_1 = 0;
static UI_ProcessBar_Handle test_processbar_1 = 0;
static UI_Drop_Handle test_drop = 0;
static UI_DigInput_Handle test_DigInput = 0;
static UI_StrInput_Handle test_StrInput = 0;
static UI_TriggerLabel_Handle test_TriggerLabel = 0;

/* internal function definition */

static void TaskWidget_Init(void)
{
    Widget_Mng.config_all->mirror(Oled_MirrorX);

    test1 = Widget_Mng.Create(20, 2, 25, 20, "test1", true);
    // test2 = Widget_Mng.Create(50, 5, 50, 20, "test2", true);
    // test3 = Widget_Mng.Create(10, 15, 70, 40, "test3", true);
    test4 = Widget_Mng.Create(0, 0, 128, 64, "test4", false);

    // test_btn_1 = Widget_Mng.Control(test4)->UI()->Button()->create("test_btn_1", 90, 5, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, Reset_Btn, DEFAULT_BUTTON_STATE);
    // test_btn_2 = Widget_Mng.Control(test4)->UI()->Button()->create("test_btn_2", 90, 10 + DEFAULT_BUTTON_HEIGHT, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, Lock_Btn, DEFAULT_BUTTON_STATE);

    test_checkbox_1 = Widget_Mng.Control(test4)->UI()->CheckBox()->create("checkbox", 0, 10, true);
    test_processbar_1 = Widget_Mng.Control(test4)->UI()->ProcessBar()->create("frame type bar", UI_ProcBar_DspType_DotBar, 0, 32, 50, 0, 100);
    Widget_Mng.Control(test4)->UI()->ProcessBar()->Set_DspDir(test_processbar_1, UI_ProcBar_GrothFrom_Left);
    test_slidebar_1 = Widget_Mng.Control(test4)->UI()->SlideBar()->create("Slidebar", 0, 0, SliderBar_Horizon_Mode, 10, -10, 0, 50);
    test_drop = Widget_Mng.Control(test4)->UI()->Drop()->create("drop 1: ", 0, 36);
    Widget_Mng.Control(test4)->UI()->Drop()->add_item(test_drop, "item_1", NULL, 0, NULL);
    Widget_Mng.Control(test4)->UI()->Drop()->add_item(test_drop, "item_2", NULL, 0, NULL);

    test_DigInput = Widget_Mng.Control(test4)->UI()->DigInput()->create("Int Input:", 0, 47, UI_IntDig_Input);
    Widget_Mng.Control(test4)->UI()->DigInput()->set_IntInput_Range(test_DigInput, 3, -200, 200, -173);

    test_StrInput = Widget_Mng.Control(test4)->UI()->StrInput()->create("Str Input:", 0, 56);
    test_TriggerLabel = Widget_Mng.Control(test4)->UI()->TriggerLabel()->create("TriggerLabel", 0, 65);
}

static void TestWidget_Dynamic_Dsp(void)
{
    static int8_t rad = 0;
    static bool change = false;

    static int8_t dis = 0;
    static bool move = false;
    static int8_t selector = 0;
    static int8_t selector_cnt = 0;
    static bool selector_switch = false;
    static uint32_t Lst_Encoder_Val = 0;
    uint32_t encoder_val = 0;

    encoder_val = Lst_Encoder_Val - TaskInput_GetData()->Enc_Val.val;
    Lst_Encoder_Val = TaskInput_GetData()->Enc_Val.val;

    if (!change)
    {
        rad += 2;
        // slidebar_in--;

        if (rad >= 32)
        {
            // Widget_Mng.Control(test4)->UI()->Button()->Operate(test_btn_1, UI_Btn_PushDwn);
            // Widget_Mng.Control(test4)->UI()->Button()->Operate(test_btn_2, UI_Btn_PushDwn);
            Widget_Mng.Control(test4)->UI()->CheckBox()->Trigger(test_checkbox_1);
            change = true;
        }
    }
    else
    {
        rad -= 2;
        // slidebar_in++;

        if (rad <= 2)
        {
            // Widget_Mng.Control(test4)->UI()->Button()->Operate(test_btn_1, UI_Btn_RlsUp);
            // Widget_Mng.Control(test4)->UI()->Button()->Operate(test_btn_2, UI_Btn_RlsUp);
            change = false;
        }
    }

    if (!selector_switch)
    {
        if ((selector_cnt % 10) == 0)
            selector = 1;

        if (selector_cnt >= 30)
        {
            selector_switch = !selector_switch;
        }
    }
    else
    {
        if ((selector_cnt % 10) == 0)
            selector = -1;

        if (selector_cnt <= 0)
        {
            selector_switch = !selector_switch;
        }
    }

    if (!move)
    {
        dis += 2;

        if (dis > 20)
        {
            // Widget_Mng.Control(test1)->Clear();
            // Widget_Mng.Control(test1)->Draw()->draw_char(Font_12, '1', 8, 2, true);
            // Widget_Mng.Control(test1)->Show();
            move = true;
        }
    }
    else
    {
        dis -= 2;

        if (dis <= 2)
        {
            // Widget_Mng.Control(test1)->Hide();
            move = false;
        }
    }
    selector = encoder_val;

    static int8_t prc_val;
    prc_val += encoder_val;
    if (prc_val >= 100)
        prc_val = 100;
    else if (prc_val <= 0)
        prc_val = 0;

    Widget_Mng.Control(test4)->Clear();
    Widget_Mng.Control(test4)->UI()->SlideBar()->Input(test_slidebar_1, &encoder_val);

    Widget_Mng.Control(test4)->UI()->ProcessBar()->Set_Value(test_processbar_1, prc_val);
    // Widget_Mng.Control(test4)->Draw()->draw_str(Font_8, "slidebar Timer", 2, 2, true);

    Widget_Mng.Control(test4)->UI()->Show_Selector(&selector);
    Widget_Mng.Control(test4)->UI()->Fresh();
    Widget_Mng.Control(test4)->Show();

    // Widget_Mng.Control(test3)->Clear();
    // Widget_Mng.Control(test3)->Move(15 /* + dis*/, 15);
    // Widget_Mng.Control(test3)->Draw()->fill_circle(35, 20, rad, true);
    // Widget_Mng.Control(test3)->Draw()->fill_circle(35, 20, rad - 2, false);

    // Widget_Mng.Control(test3)->Draw()->draw_str(Font_12, "8_B!T0", 15, 22, true);
    // Widget_Mng.Control(test3)->Draw()->draw_char(Font_12, '3', 8, 2, true);
    // Widget_Mng.Control(test3)->Show();

    // Widget_Mng.Control(test2)->Clear();
    // Widget_Mng.Control(test2)->Move(50, 5 + dis);
    // Widget_Mng.Control(test2)->Draw()->draw_char(Font_12, '2', 8, 2, true);
    // // Widget_Mng.Control(test2)->Draw()->fill_radius_rectangle(22, 3, 25, 14, 3);
    // Widget_Mng.Control(test2)->Draw()->draw_str(Font_12, "yes", 24, 3, false);
    // //Widget_Mng.Control(test2)->Draw()->draw_str(Font_12, "no", 46, 40, true);
    // //Widget_Mng.Control(test2)->Draw()->draw_radius_rectangle(40, 40, 25, 15, 3, 1);
    // Widget_Mng.Control(test2)->Show();
}

void TaskWidget_TestCore(Task_Handler self)
{
    switch ((uint8_t)TaskWidget_State)
    {
    case TaskWdiget_State_Init:
        TaskWidget_Init();
        TaskWidget_State = TaskWidget_State_CheckFresh;
        break;

    case TaskWidget_State_CheckFresh:
        if (Widget_Mng.trigger_fresh())
        {
            TaskWidget_State = TaskWidget_State_Fresh;
        }
        else
            TaskWidget_State = TaskWidget_State_CheckFresh;
        break;

    case TaskWidget_State_Fresh:
        TestWidget_Dynamic_Dsp();
        Widget_Mng.fresh_all();
        TaskWidget_State = TaskWidget_State_CheckFresh;
        break;
    }
}
