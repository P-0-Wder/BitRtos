#include "Task_Widget.h"
#include "task_manager.h"
#include "widget_mng.h"

/* this task only for test */

/* internal variable */
static TaskWidget_State_List TaskWidget_State = TaskWdiget_State_Init;
static Widget_Handle test1;
static Widget_Handle test2;
static Widget_Handle test3;
static Widget_Handle test4;

/* external function definition */
Task_Handler TaskWidget_Hdl = 0;

/* internal function definition */

static void TaskWidget_Init(void)
{
    Widget_Mng.config_all->mirror(Oled_MirrorX);

    test1 = Widget_Mng.Create(20, 2, 25, 20, "test1", true);
    //test2 = Widget_Mng.Create(50, 5, 50, 20, "test2", true);
    test3 = Widget_Mng.Create(15, 15, 70, 40, "test3", true);
    test4 = Widget_Mng.Create(0, 0, 128, 64, "test4", true);
}

static void TestWidget_Dynamic_Dsp(void)
{
    static uint8_t rad = 0;
    static bool change = false;

    static uint8_t dis = 0;
    static bool move = false;

    if (!change)
    {
        rad++;

        if (rad > 10)
            change = true;
    }
    else
    {
        rad--;

        if (rad <= 2)
            change = false;
    }

    if (!move)
    {
        dis += 2;

        if (dis > 20)
        {
            Widget_Mng.Control(test1)->Clear();
            Widget_Mng.Control(test1)->Show();
            move = true;
        }
    }
    else
    {
        dis -= 2;

        if (rad <= 2)
        {
            Widget_Mng.Control(test1)->Hide();
            move = false;
        }
    }

    Widget_Mng.Control(test4)->Clear();
    Widget_Mng.Control(test4)->Draw()->draw_char(Font_12, '4', 8, 2, true);
    Widget_Mng.Control(test4)->Show();

    Widget_Mng.Control(test1)->Draw()->draw_char(Font_12, '1', 8, 2, true);

    Widget_Mng.Control(test3)->Clear();
    Widget_Mng.Control(test3)->Draw()->draw_char(Font_12, '3', 8, 2, true);
    Widget_Mng.Control(test3)->Move(15 + dis, 15);
    Widget_Mng.Control(test3)->Draw()->draw_circle(35, 20, rad, 1);
    Widget_Mng.Control(test3)->Draw()->draw_str(Font_12, "8_B!T0", 15, 22, true);
    Widget_Mng.Control(test3)->Show();

    //Widget_Mng.Control(test2)->Clear();
    //Widget_Mng.Control(test2)->Move(50, 5 + dis);
    //Widget_Mng.Control(test2)->Draw()->draw_char(Font_12, '2', 8, 2, true);
    //Widget_Mng.Control(test2)->Show();
}

void TaskWidget_Core(Task_Handler self)
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
