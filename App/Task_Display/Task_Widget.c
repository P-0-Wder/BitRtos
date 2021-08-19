#include "Task_Widget.h"
#include "task_manager.h"
#include "runtime.h"
#include "widget_mng.h"

/* internal variable */
static TaskWidget_State_List TaskWidget_State = TaskWdiget_State_Init;
static Widget_Handle test1;
static Widget_Handle test2;
static Widget_Handle test3;

/* external function definition */

/* internal function definition */

static void TaskWidget_Init(void)
{
    test1 = Widget_Mng.Create(0, 0, 30, 10, "test1");
    Widget_Mng.Control(test1)->Draw()->draw_char(Font_12, 'A', 0, 0, false);
    Widget_Mng.Control(test1)->Show();

    test2 = Widget_Mng.Create(40, 12, 40, 20, "test2");
    Widget_Mng.Control(test2)->Draw()->draw_char(Font_12, 'B', 0, 0, false);
    Widget_Mng.Control(test2)->Show();

    test3 = Widget_Mng.Create(60, 34, 10, 20, "test3");
    Widget_Mng.Control(test3)->Draw()->draw_char(Font_12, 'B', 0, 0, false);
    Widget_Mng.Control(test3)->Show();
}

void TaskWidget_Core(Task_Handler self)
{
    switch ((uint8_t)TaskWidget_State)
    {
    case TaskWdiget_State_Init:
        TaskWidget_Init();
        break;

    case TaskWidget_State_Fresh:
        Widget_Mng.fresh_all();
        break;
    }
}
