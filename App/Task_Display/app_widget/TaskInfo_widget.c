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
    float stk_occupy;
    uint32_t remain_stack;
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
} TaskInfoDsp_BLock_TypeDef;

typedef enum
{
    Stage_UICtl_Init = 0,
    Stage_UpdateDsp,
} TaskInfo_DspStage_List;

static TaskInfoDsp_BLock_TypeDef *TaskInfo_UICtl_List = NULL;
static TaskInfo_DspStage_List stage = Stage_UICtl_Init;

static bool TaskInfo_CreateUICtl(Widget_Handle hdl);
static bool TaskInfo_DspUpdate(Widget_Handle hdl);

static bool TaskInfo_CreateUICtl(Widget_Handle hdl)
{
    uint8_t task_num = 0;

    if (hdl == 0)
        return false;

    task_num = Task_Get_TaskNum();
    TaskInfo_UICtl_List = (TaskInfoDsp_BLock_TypeDef *)MMU_Malloc(task_num * sizeof(TaskInfoDsp_BLock_TypeDef));

    if (TaskInfo_UICtl_List == NULL)
        return false;

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
