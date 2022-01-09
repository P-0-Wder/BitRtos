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
    TaskInfoDsp_Info_TypeDef *info;
} TaskInfo_DspLayer_TypeDef;

typedef enum
{
    Stage_UICtl_Init = 0,
    Stage_UpdateDsp,
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
}

static bool TaskInfo_CreateUICtl(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;
    TaskInfo_DspClear();

    TaskInfo_Dsp.num = Task_Get_TaskNum();
    TaskInfo_Dsp.info = (TaskInfoDsp_Info_TypeDef *)MMU_Malloc(TaskInfo_Dsp.num * sizeof(TaskInfoDsp_Info_TypeDef));

    if (TaskInfo_Dsp.info == NULL)
        return false;

    for (uint8_t i = 0; i < TaskInfo_Dsp.num; i++)
    {
    }

    return true;
}

bool TaskInfo_DspUpdate(Widget_Handle hdl)
{
    if (hdl == 0)
        return false;

    while (true)
    {
        switch (stage)
        {
        case Stage_UICtl_Init:
            return TaskInfo_CreateUICtl(hdl);

        case Stage_UpdateDsp:
            break;

        default:
            break;
        }
    }

    return true;
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
