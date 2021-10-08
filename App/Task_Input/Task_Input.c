#include "task_manager.h"
#include "Task_Input.h"
#include "Input_IO_Def.h"

/* internal variable */
static Input_Data_TypeDef InputData;
static TaskInput_Stage_List Task_Stage = TaskInput_Initialize;

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
Input_Data_TypeDef TaskInput_GetData(void);

/* internal function */

/* external variable */

static Input_Error_List TaskInput_Init(void)
{
    /* encoder pin */
    DrvGPIO_Obj_TypeDef EncPin[Encoder_IO_Sum];

    InputData.error = Input_Initial;

    EncPin[Encoder_IO_A] = Encoder_A_Pin;
    EncPin[Encoder_IO_B] = Encoder_B_Pin;
    EncPin[Encoder_IO_Btn] = Encoder_Btn;

    /* init encoder */
    if (!DevEncoder.open(&Encoder_Obj, EncPin, true, Timer_3, TIM_Channel_1, TIM_Channel_2))
    {
        InputData.error = Input_Encoder_Error;
        return Input_Encoder_Error;
    }

    /* init gimbal */

    /* init direction button */

    /* init shoulder button */

    /* init shoulder toggle */

    /* init side pot */

    InputData.error = Input_NoError;
    return Input_NoError;
}

static int16_t TaskInput_AnalogIn_Map(int16_t in, int16_t range_min, int16_t range_max)
{
    int16_t map_val = 0;
    double gain = 0.0;

    return map_val;
}

static Input_Error_List TaskInput_Update(void)
{
    if (InputData.error != Input_NoError)
        return InputData.error;

    InputData.Enc_Val = DevEncoder.get(&Encoder_Obj);
}

Input_Data_TypeDef TaskInput_GetData(void)
{
    return InputData;
}

void TaskInput_Core(Task_Handler self)
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
