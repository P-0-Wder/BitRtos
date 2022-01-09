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
    UI_StrInput_Handle task_name_ctl;
    UI_DigInput_Handle task_group_ctl;
    UI_DigInput_Handle task_pri_ctl;
    UI_DigInput_Handle task_frq_ctl;
    UI_DigInput_Handle task_detectfrq_ctl;
    UI_DigInput_Handle task_cpu_occupy;
    UI_DigInput_Handle task_stk_occupy;
    UI_DigInput_Handle task_total_stk;
    UI_DigInput_Handle task_remain_stk;
} TaskInfoDsp_Info_TypeDef;

typedef struct
{
    uint8_t num;
    TaskDspInfo_TypeDef *info;
    TaskInfoDsp_Info_TypeDef *ui_ctl;
} TaskInfo_DspLayer_TypeDef;

typedef enum
{
    Stage_UICtl_Init = 0,
    Stage_DspTaskName,
    Stage_DspTaskInfo,
    Stage_Sum,
} TaskInfo_DspStage_List;

static TaskInfo_DspLayer_TypeDef TaskInfo_Dsp;
static TaskInfo_DspStage_List stage = Stage_UICtl_Init;

static void TaskInfo_DspClear(void);
static bool TaskInfo_ShowNameList(Widget_Handle hdl);
static bool TaskInfo_CreateUICtl(Widget_Handle hdl);
static bool TaskInfo_DspUpdate(Widget_Handle hdl);

static void TaskInfo_DspClear(void)
{
    TaskInfo_Dsp.num = 0;
    TaskInfo_Dsp.info = NULL;
    TaskInfo_Dsp.ui_ctl = NULL;
}

static bool TaskInfo_CreateUICtl(Widget_Handle hdl)
{
    uint8_t i = 0;
    if (hdl == 0)
        return false;
    TaskInfo_DspClear();

    TaskInfo_Dsp.num = Task_Get_TaskNum();
    TaskInfo_Dsp.info = (TaskDspInfo_TypeDef *)MMU_Malloc(TaskInfo_Dsp.num * sizeof(TaskDspInfo_TypeDef));
    TaskInfo_Dsp.ui_ctl = (TaskInfoDsp_Info_TypeDef *)MMU_Malloc(TaskInfo_Dsp.num * sizeof(TaskInfoDsp_Info_TypeDef));

    if ((TaskInfo_Dsp.info == NULL) || (TaskInfo_Dsp.ui_ctl == NULL))
        return false;

    for (i = 0; i < TaskInfo_Dsp.num; i++)
    {
        if (!Task_GetInfo_ByIndex(i, &TaskInfo_Dsp.info[i]))
        {
            MMU_Free(TaskInfo_Dsp.info);
            MMU_Free(TaskInfo_Dsp.ui_ctl);
            TaskInfo_Dsp.num = 0;
            return false;
        }
    }

    // create widget ui controller
    for (i = 0; i < TaskInfo_Dsp.num; i++)
    {
        // Widget_Mng.Control(hdl)->UI()->StrInput()->create();
    }

    return true;
}

static bool TaskInfo_ShowNameList(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    return true;
}

bool TaskInfo_SetStage(int8_t offset)
{
    if (((stage + offset) < 0) || ((stage + offset) > Stage_Sum))
        return false;

    stage += offset;

    return true;
}

bool TaskInfo_DspUpdate(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    while (true)
    {
        Widget_Mng.Control(hdl)->Clear();

        switch (stage)
        {
        case Stage_UICtl_Init:
            if (!TaskInfo_CreateUICtl(hdl))
            {
                return false;
            }

            stage = Stage_DspTaskName;
            break;

        case Stage_DspTaskName:

            Widget_Mng.Control(hdl)->Show();
            return true;

        case Stage_DspTaskInfo:

            Widget_Mng.Control(hdl)->Show();
            return true;

        default:
            return false;
        }
    }
}

static bool TaskInfo_Free(void)
{
    if (TaskInfo_Dsp.num)
    {
        MMU_Free(TaskInfo_Dsp.info);
    }

    TaskInfo_Dsp.num = 0;

    return true;
}