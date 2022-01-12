#include "task_manager.h"
#include "Task_Input.h"
#include "Input_IO_Def.h"

Task_Handle SigInput_Tsk_Hdl;

/* internal variable */
static Input_Data_TypeDef InputData;
static TaskInput_Stage_List Task_Stage = TaskInput_Initialize;

static Gimbal_Proc_TypeDef Gimbal_L_ProcObj;
static Gimbal_Proc_TypeDef Gimbal_R_ProcObj;

static AnalogProc_Obj_TypeDef L_Pot_ProcObj;
static AnalogProc_Obj_TypeDef R_Pot_ProcObj;

static DrvGPIO_Obj_TypeDef EncPin[Encoder_IO_Sum];

static TaskInput_Callback EncoderBtn_Push_Callback = NULL;
static TaskInput_Callback EncoderBtn_Release_Callback = NULL;

/* input hardware abstract object */
static DevEncoder_Obj_TypeDef Encoder_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_L_Obj;
//static DevGimbal_Obj_TypeDef Gimbal_R_Obj;
//static DevButton_TypeDef Btn_LShoulder_Obj;
//static DevButton_TypeDef Btn_RShoulder_Obj;
//static DirButton_Obj_TypeDef Btn_5Dir_Obj;

/* internal funciton */
static Input_Error_List TaskInput_Init(void);
static Input_Error_List TaskInput_Update(void);
static void TaskInput_EncoderBtn_Callback(void);

/* internal function */

/* external variable */
Input_Data_TypeDef *TaskInput_GetData(void);

static Input_Error_List TaskInput_Init(void)
{
    InputData.error = Input_Initial;

    EncPin[Encoder_IO_A] = Encoder_A_Pin;
    EncPin[Encoder_IO_B] = Encoder_B_Pin;
    EncPin[Encoder_IO_Btn] = Encoder_Btn;

    /* init encoder */
    if (!DevEncoder.set_btn_callback(&Encoder_Obj, TaskInput_EncoderBtn_Callback) ||
        !DevEncoder.open(&Encoder_Obj, EncPin, true, Timer_8, TIM_Channel_1, TIM_Channel_2))
    {
        InputData.error = Input_Encoder_Error;
        return Input_Encoder_Error;
    }

    DevEncoder.invert(&Encoder_Obj, Encoder_Dir_Invert);

    /* init gimbal */

    /* init direction button */

    /* init shoulder button */

    /* init shoulder toggle */

    /* init side pot */

    InputData.error = Input_NoError;
    return Input_NoError;
}

static int16_t TaskInput_Analog_Filter(int16_t in)
{
    int16_t flt_val = 0;

    /* filter the input analog value */

    return flt_val;
}

static int16_t TaskInput_AnalogValue_Map(int16_t in)
{
    int16_t map_val = 0;
    double gain = 0.0;

    /* include analog input liner and none liner condition map */

    return map_val;
}

static void TaskInput_EncoderBtn_Callback(void)
{
    if (DevEncoder.trigger_button(&Encoder_Obj, &EncPin[Encoder_IO_Btn]))
    {
        if (EncoderBtn_Push_Callback != NULL)
            EncoderBtn_Push_Callback();
    }
    else
    {
        if (EncoderBtn_Release_Callback != NULL)
            EncoderBtn_Release_Callback();
    }
}

void TaskInput_SetCallback(TaskInput_Callback_TypeList type, TaskInput_Callback callback)
{
    switch (type)
    {
    case DevEncoderBtn_Push_Callback:
        EncoderBtn_Push_Callback = callback;
        break;

    case DevEncoderBtn_Release_Callback:
        EncoderBtn_Release_Callback = callback;
        break;

    default:
        break;
    }
}

static Input_Error_List TaskInput_Update(void)
{
    if (InputData.error != Input_NoError)
        return InputData.error;

    /* get encdoer */
    InputData.Enc_Val = DevEncoder.get(&Encoder_Obj);

    /* analog input */
    /* get left gimbal */

    /* get right gimbal */

    /* get left side pot value */

    /* get right side pot value */

    /* get battery percent */
}

static void TaskInput_Gimbal_Cali(void)
{
}

Input_Data_TypeDef *TaskInput_GetData(void)
{
    return &InputData;
}

void TaskInput_Core(Task_Handle self)
{
    switch (Task_Stage)
    {
    case TaskInput_Initialize:
        if (TaskInput_Init() != Input_NoError)
        {
            Task_Stage = TaskInput_Init_Error;
        }
        else
            Task_Stage = TaskInput_Updating;
        break;

    case TaskInput_Init_Error:
        /* TODO Error Process */
        break;

    case TaskInput_Updating:
        TaskInput_Update();
        break;

    default:
        break;
    }
}
