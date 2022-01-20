#include "TaskInfo_widget.h"
#include "task_manager.h"
#include "mmu.h"

typedef struct
{
    char *name;
    uint8_t group_pri;
    uint8_t task_pri;
    uint32_t frq;
    uint32_t detect_frq;
    float cpu_occupy;
    float stk_occupy;      //still in develop
    uint32_t remain_stack; //still in develop
    uint32_t total_stack;
} TaskDspInfo_TypeDef;

typedef struct
{
    uint8_t num;
    TaskDspInfo_TypeDef *info;
} TaskInfo_DspLayer_TypeDef;

static int32_t EncoderVal = 0;
static bool TaskWidget_CreateState = false;
static Widget_Handle TaskList_Widget_Hdl = 0;
static Widget_Handle TaskInfo_Widget_Hdl = 0;
static TaskInfo_DspLayer_TypeDef TaskInfo_Dsp;
static TaskInfo_DspStage_List stage = InfoDspStage_CreateWidget;

static void TaskInfo_DspClear(void);
static bool TaskInfo_SetStage(int8_t *offset);
static bool TaskInfo_GetInfo(Widget_Handle hdl);
static void TaskInfo_SwitchWidget(int8_t *offset);
static bool TaskInfo_ShowNameList(Widget_Handle hdl);

static void TaskInfo_DspClear(void)
{
    TaskInfo_Dsp.num = 0;
    TaskInfo_Dsp.info = NULL;
}

static bool TaskInfo_CreateWidget(Widget_Handle hdl)
{
    TaskList_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "Task List", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);
    TaskInfo_Widget_Hdl = Widget_Mng.Create_Sub(hdl, HandleToWidgetObj(hdl)->width, HandleToWidgetObj(hdl)->height, "Task Info Ditial", HIDE_WIDGET_FRAME, SHOW_WIDGET_NAME);

    if ((TaskList_Widget_Hdl == WIDGET_CREATE_ERROR) || (TaskInfo_Widget_Hdl == WIDGET_CREATE_ERROR))
        return false;

    return true;
}

static bool TaskInfo_GetInfo(Widget_Handle hdl)
{
    uint8_t i = 0;
    if (hdl == 0)
        return false;
    TaskInfo_DspClear();

    TaskInfo_Dsp.num = Task_Get_TaskNum();
    TaskInfo_Dsp.info = (TaskDspInfo_TypeDef *)MMU_Malloc(TaskInfo_Dsp.num * sizeof(TaskDspInfo_TypeDef));

    if (TaskInfo_Dsp.info == NULL)
        return false;

    for (i = 0; i < TaskInfo_Dsp.num; i++)
    {
        if (!Task_GetInfo_ByIndex(i, &TaskInfo_Dsp.info[i]))
        {
            MMU_Free(TaskInfo_Dsp.info);
            TaskInfo_Dsp.num = 0;
            return false;
        }
    }

    return true;
}

static void TaskInfo_ResetCtl(void)
{
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->UI()->Reset_SelectUICtl();
}

static bool TaskInfo_ShowNameList(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    return true;
}

static bool TaskInfo_SetStage(int8_t *offset)
{
    if (((stage + offset) < InfoDspStage_DspExit) || ((stage + offset) > InfoDspStage_DspError))
        return false;

    stage += *offset;
    *offset = 0;

    return true;
}

static bool TaskInfo_Free(void)
{
    if (TaskInfo_Dsp.num)
        MMU_Free(TaskInfo_Dsp.info);

    TaskInfo_Dsp.num = 0;

    return true;
}

static bool TaskInfo_UpdateDspList(void)
{
    Widget_Mng.Control(TaskList_Widget_Hdl)->Clear();
    Widget_Mng.Control(TaskList_Widget_Hdl)->Show();

    return true;
}

static bool TaskInfo_UpdataDspDitial(void)
{
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->Clear();
    Widget_Mng.Control(TaskInfo_Widget_Hdl)->Show();

    return true;
}

TaskInfo_DspStage_List TaskInfo_DspUpdate(Widget_Handle hdl, int8_t *encoder_in)
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

        stage = InfoDspStage_GetTaskInfo;
        break;

    case InfoDspStage_GetTaskInfo:
        // if (!TaskInfo_CreateUICtl(hdl))
        // {
        //     stage = InfoDspStage_DspError;
        //     break;
        // }

        stage = InfoDspStage_DspTaskName;
        break;

    case InfoDspStage_DspTaskName:
        TaskInfo_UpdateDspList();
        break;

    case InfoDspStage_DspTaskInfo:
        TaskInfo_UpdataDspDitial();
        break;

    case InfoDspStage_DspExit:
        stage = InfoDspStage_GetTaskInfo;
        break;

    default:
        return InfoDspStage_DspError;
    }

    Widget_Mng.Control(hdl)->Show();

    return stage;
}
