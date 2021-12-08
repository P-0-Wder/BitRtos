#include "Task_Encoder.h"
#include "Dev_Encoder.h"
#include "shell_port.h"
#include "drv_serial.h"
#include "shell_port.h"

encoder_sta_list encoder_sta = encoder_init_sta;
encoder_sta_getlist_t encoder_sta_getlist;
Task_Handler Encoder_Tsk_Hdl;

void Task_Encoder_Init(void)
{
    static int encoder_sta = encoder_init_sta;
    GPIO_IO_Encoder_Init(Encoder_Port,Encoder_IO_CLK,GPIO_Pin_7,Encoder_IO_A_PinSource,GPIO_AF_TIM8);//GPIOC_Pin7
    GPIO_IO_Encoder_Init(Encoder_Port,Encoder_IO_CLK,GPIO_Pin_6,Encoder_IO_B_PinSource,GPIO_AF_TIM8);//GPIOC_Pin6
    periph_Timer_Encoder_Mode_Init(Timer_8,TIM_Channel_1,TIM_Channel_2);
}

void encoder_get_state(Timer_list timerx)
{
    encoder_sta_getlist.num_rotation_circle += (short)Timer_Port[timerx]->CNT;
    Timer_Port[timerx]->CNT = 0;
}


void Task_Encoder_Core(Task_Handler self)
{
    switch(encoder_sta)
    {
        case encoder_init_sta:
            Task_Encoder_Init();
            encoder_sta = encoder_get_sta;
            break;
        case encoder_get_sta:
            encoder_get_state(Timer_8);
        break;

    }
}



/*The rotation number of the encoder can be reported to the shell display */
void encoder_get_to_shell_show(void)
{
    bt_printf("emcoder_num:%d\r\n",encoder_sta_getlist.num_rotation_circle/4);
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
encoder_get_to_shell_show, encoder_get_to_shell_show, getconder);
