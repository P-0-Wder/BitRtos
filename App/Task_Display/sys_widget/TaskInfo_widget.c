#include "TaskInfo_widget.h"
#include "task_manager.h"
#include "mmu.h"

typedef struct
{
    uint8_t num;
    Task_Base_Info *info;
} TaskInfo_DspLayer_TypeDef;

static int32_t EncoderVal = 0;
static bool TaskWidget_CreateState = false;
static Widget_Handle TaskList_Widget_Hdl = 0;
static Widget_Handle TaskInfo_Widget_Hdl = 0;
static TaskInfo_DspLayer_TypeDef TaskInfo_Dsp;
static TaskInfo_DspStage_List stage = InfoDspStage_CreateWidget;
static int8_t task_index = -1;

static UI_TriggerLabel_Handle *TaskName_LabelList;
static UI_TriggerLabel_Handle TaskInfo_Label = 0;

static void TaskInfo_DspClear(void);
static bool TaskInfo_SetStage(int8_t *offset);
static bool TaskInfo_GetInfo(Widget_Handle hdl);
static void TaskInfo_SwitchWidget(int8_t *offset);

static void TaskInfo_BackLabel_TriggerCallback(void)
{
    task_index = -1;
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->Hide();
    stage = InfoDspStage_DspTaskName;
}

static void TaskList_BackLabel_TriggerCallback(void)
{
    task_index = -1;
    Widget_Mng.Control(TaskList_Widget_Hdl)->Hide();
    stage = InfoDspStage_DspExit;
}

static void TaskInfo_SelectedTask_TriggerCallback(void)
{
    for (uint8_t i = 0; i < TaskInfo_Dsp.num; i++)
    {
        if (TaskName_LabelList[i] == Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->Get_CurSelected_UI())
        {
            task_index = i;
            Task_GetInfo_ByIndex(task_index, &TaskInfo_Dsp.info[task_index]);
            TaskInfo_Dsp.info[task_index].remain_stk_size = TaskInfo_Dsp.info[task_index].stk_depth - Task_GetStackRemain(TaskInfo_Dsp.info[task_index].tsk_hdl);

            Widget_Mng.Control(TaskList_Widget_Hdl)->Hide();
            stage = InfoDspStage_DspTaskInfo;
            break;
        }
    }
}

static void TaskInfo_DspClear(void)
{
    TaskInfo_Dsp.num = 0;
    TaskInfo_Dsp.info = NULL;
}

static bool TaskInfo_CreateWidget(Widget_Handle hdl)
{
    TaskList_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "Task List", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    TaskInfo_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "Task Info", HIDE_WIDGET_FRAME, HIDE_WIDGET_NAME);
    TaskInfo_Label = Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->TriggerLabel()->create("back", 0, 50);

    if ((TaskList_Widget_Hdl == WIDGET_CREATE_ERROR) || (TaskInfo_Widget_Hdl == WIDGET_CREATE_ERROR) || (TaskInfo_Label == NULL))
        return false;

    Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->TriggerLabel()->set_callback(TaskInfo_Label, TaskInfo_BackLabel_TriggerCallback);

    return true;
}

static bool Get_TaskInfo(Widget_Handle hdl)
{
    uint8_t i = 0;
    int16_t y_offset = 0;

    if (hdl == 0)
        return false;
    TaskInfo_DspClear();

    TaskInfo_Dsp.num = Task_Get_TaskNum();
    TaskInfo_Dsp.info = (Task_Base_Info *)MMU_Malloc(TaskInfo_Dsp.num * sizeof(Task_Base_Info));

    if (TaskInfo_Dsp.info == NULL)
        return false;

    /* get task info first */
    for (i = 0; i < TaskInfo_Dsp.num; i++)
    {
        if (!Task_GetInfo_ByIndex(i, &TaskInfo_Dsp.info[i]))
        {
            MMU_Free(TaskInfo_Dsp.info);
            TaskInfo_Dsp.num = 0;
            return false;
        }
    }

    /* second create trigger_label_handle */
    TaskName_LabelList = (UI_TriggerLabel_Handle *)MMU_Malloc(sizeof(UI_TriggerLabel_Handle) * TaskInfo_Dsp.num + 1);
    if (TaskName_LabelList == NULL)
        return false;

    /* third create trigger_label controller */
    for (i = 0; i < TaskInfo_Dsp.num; i++)
    {
        TaskName_LabelList[i] = Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->TriggerLabel()->create(TaskInfo_Dsp.info[i].name, 0, y_offset);
        y_offset += UICTL_TRIGGERLABEL_HEIGHT;

        /* set trigger callback */
        Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->TriggerLabel()->set_callback(TaskName_LabelList[i], TaskInfo_SelectedTask_TriggerCallback);
    }

    TaskName_LabelList[TaskInfo_Dsp.num + 1] = Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->TriggerLabel()->create("back", 0, y_offset);
    Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->TriggerLabel()->set_callback(TaskName_LabelList[TaskInfo_Dsp.num + 1], TaskList_BackLabel_TriggerCallback);

    return true;
}

static void TaskInfo_ResetCtl(void)
{
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->Reset_SelectUICtl();
}

static bool TaskInfo_SetStage(int8_t *offset)
{
    if (((stage + offset) < InfoDspStage_DspExit) || ((stage + offset) > InfoDspStage_DspError))
        return false;

    stage += *offset;
    *offset = 0;

    return true;
}

static void TaskInfo_UpdateDspList(int8_t *encoder_in, bool *btn)
{
    static bool get_TaskInfo = false;

    if (!get_TaskInfo)
        get_TaskInfo = Get_TaskInfo(TaskList_Widget_Hdl);

    if (*btn)
    {
        Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->TriggerLabel()->trigger(Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->Get_CurSelected_UI());
        *btn = false;
    }

    Widget_Mng.Control(TaskList_Widget_Hdl)->Clear();
    Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->Show_Selector(encoder_in);
    Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->Fresh();
    Widget_Mng.Control(TaskList_Widget_Hdl)->Show();
}

static void TaskInfo_UpdataDspDitial(uint8_t *encoder_in, bool *btn)
{
    int16_t y_offset = 0;
    char dsp_buff[32];

    memset(dsp_buff, NULL, 32);

    if (*btn)
    {
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->TriggerLabel()->trigger(Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->Get_CurSelected_UI());
        *btn = false;
    }

    Widget_Mng.Control(TaskInfo_Widget_Hdl)->Clear();
    if (task_index >= 0)
    {
        /* display task name */ /* use sprintf */
        sprintf(dsp_buff, "name: %s", TaskInfo_Dsp.info[task_index].name);
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, dsp_buff, 0, 0, true);
        memset(dsp_buff, NULL, 32);
        y_offset += UICTL_DEFAULT_HEIGHT;

        /* display priority */
        sprintf(dsp_buff, "Pri:  %d : %d", TaskInfo_Dsp.info[task_index].group, TaskInfo_Dsp.info[task_index].priority);
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, dsp_buff, 0, y_offset, true);
        memset(dsp_buff, NULL, 32);
        y_offset += UICTL_DEFAULT_HEIGHT;

        /* display set frequence & actually frequence */
        sprintf(dsp_buff, "Frq:  %d / %d", TaskInfo_Dsp.info[task_index].set_frq, TaskInfo_Dsp.info[task_index].exec_frq);
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, dsp_buff, 0, y_offset, true);
        memset(dsp_buff, NULL, 32);
        y_offset += UICTL_DEFAULT_HEIGHT;

        /* display task cpu occupy */
        sprintf(dsp_buff, "CPU:  %.3f%%", TaskInfo_Dsp.info[task_index].cpu_opy);
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, dsp_buff, 0, y_offset, true);
        memset(dsp_buff, NULL, 32);
        y_offset += UICTL_DEFAULT_HEIGHT;

        /* display task stack occupy */
        sprintf(dsp_buff, "Mem:  %.2f%%", (TaskInfo_Dsp.info[task_index].remain_stk_size / (float)TaskInfo_Dsp.info[task_index].stk_depth) * 100.0);
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, dsp_buff, 0, y_offset, true);
        memset(dsp_buff, NULL, 32);
        y_offset += UICTL_DEFAULT_HEIGHT;
    }
    else
        Widget_Mng.Control(TaskInfo_Widget_Hdl)->Draw()->draw_str(Font_8, "get info failed", 0, 0, true);

    Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->Show_Selector(encoder_in);
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->Fresh();
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->Show();
}

TaskInfo_DspStage_List TaskInfo_DspUpdate(Widget_Handle hdl, int8_t *encoder_in, bool *btn)
{
    if (hdl == 0)
        return false;

    switch (stage)
    {
    case InfoDspStage_CreateWidget:
        TaskWidget_CreateState = TaskInfo_CreateWidget(hdl);

        if (!TaskWidget_CreateState)
        {
            stage = InfoDspStage_DspError;
            break;
        }
        else
            stage = InfoDspStage_DspTaskName;

    case InfoDspStage_DspTaskName:
        TaskInfo_UpdateDspList(encoder_in, btn);
        break;

    case InfoDspStage_DspTaskInfo:
        TaskInfo_UpdataDspDitial(encoder_in, btn);
        break;

    case InfoDspStage_DspExit:
        *btn = false;
        Widget_Mng.Control(TaskList_Widget_Hdl)->UI()->Reset_SelectUICtl();
        stage = InfoDspStage_DspTaskName;
        break;

    default:
        return InfoDspStage_DspError;
    }

    return stage;
}
