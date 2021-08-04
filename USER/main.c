#include "delay.h"
#include "runtime.h"
#include "task.h"
#include "task_delay.h"
#include "stddef.h"
#include "vcp.h"
#include "stdio.h"
#include "sbus.h"
#include "led.h"
#include "mpu_proc.h"
#include "radio.h"
#include "imu.h"
#include "mavlink.h"
#include "periph_serial.h"
#include "sdcard.h"
#include "blackbox.h"
#include "blackbox_fielddefs.h"
#include "debug.h"
#include "reboot.h"

//test part
#include "binary_tree.h"
#include "linked_list.h"
//test part

//create a task handler
Task_Handler gyro_sample_tsk_hdl = NULL;
Task_Handler acc_sample_tsk_hdl = NULL;
Task_Handler imu_tsk_hdl = NULL;
Task_Handler blink_tsk_hdl = NULL;

#if RADIO
Task_Handler radio_tsk_hdl = NULL;
#endif

#if SDCARD
Task_Handler blackbox_tsk_hdl = NULL;
#endif

#if DEBUG
Task_Handler debug_tsk_hdl = NULL;
#endif

Task_Handler test1_tsk_hdl = NULL;
Task_Handler test2_tsk_hdl = NULL;
Task_Handler test3_tsk_hdl = NULL;
Task_Handler test4_tsk_hdl = NULL;

void Task_test_1(Task_Handler self)
{
	volatile SYSTEM_RunTime Rt_tmp;
	Rt_tmp = Get_Cur_Runtime();

	//printf("task 1 exec: %d\r\n", ((Task *)self)->Exec_status.Exec_Times);
	//printf("Rt %d.%d.%d.%d.%d\r\n", Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);

	Task_Delay(self, Ms_Unit, 1);

	//Rt_tmp = Get_Cur_Runtime();
	printf("\r\ntask 1 delay finish Rt %d.%d.%d.%d.%d\r\n", Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);

	switch ((uint8_t)((Task *)test3_tsk_hdl)->Exec_status.State)
	{
	case Task_Ready:
		printf("Task 3 State : Task_Ready\r\n");
		break;

	case Task_Running:
		printf("Task 3 State : Task_Running\r\n");
		break;

	case Task_Stop:
		printf("Task 3 State : Task_Stop\r\n");
		break;

	case Task_Block:
		printf("Task 3 State : Task_Block\r\n");
		break;

	case Task_Pending:
		printf("Task 3 State : Task_Pending\r\n");
		break;

	case Task_Done:
		printf("Task 3 State : Task_Done\r\n");
		break;

	default:
		break;
	}

	printf("\r\n");
}

void Task_test_2(Task_Handler self)
{
	volatile SYSTEM_RunTime Rt_tmp;

	//Rt_tmp = Get_Cur_Runtime();
	//printf("task 2 exec: %d delay start at Rt %d.%d.%d.%d.%d\r\n", ((Task *)self)->Exec_status.Exec_Times, Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);

	Task_Delay(self, Ms_Unit, 20);

	//Rt_tmp = Get_Cur_Runtime();
	//printf("task in delay: %d     get task 1 state : %d \r\n", TaskDelay_GetNum(), ((Task *)test1_tsk_hdl)->Exec_status.State);
	//printf("task 2 delay finish Rt %d.%d.%d.%d.%d\r\n\r\n", Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);
}

void Task_test_3(Task_Handler self)
{
	volatile SYSTEM_RunTime Rt_tmp;

	Rt_tmp = Get_Cur_Runtime();
	printf("task 3 exec: %d delay start at Rt %d.%d.%d.%d.%d\r\n", ((Task *)self)->Exec_status.Exec_Times, Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);

	Task_Delay(self, Ms_Unit, 10);

	printf("task 3 delay finish Rt %d.%d.%d.%d.%d\r\n", Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);
	printf("task 3 exec: %d\r\n\r\n", ((Task *)self)->Exec_status.Exec_Times);
}

void Task_test_4(Task_Handler self)
{
	volatile SYSTEM_RunTime Rt_tmp;
	Rt_tmp = Get_Cur_Runtime();

	/*
	printf("task 4 exec: %d\r\n", ((Task *)self)->Exec_status.Exec_Times);
	printf("task 3 exec: %d delay start at Rt %d.%d.%d.%d.%d\r\n", ((Task *)test3_tsk_hdl)->Exec_status.Exec_Times, Rt_tmp.hour, Rt_tmp.min, Rt_tmp.s, Rt_tmp.ms, Rt_tmp.us);

	printf("task 3 delay info\r\n");
	printf("task 3 state : %d\r\n", ((Task *)test3_tsk_hdl)->Exec_status.State);
	printf("on delay : %d\r\n", ((Task *)test3_tsk_hdl)->delay_info.on_delay);
	printf("delay us remian : %d\r\n", ((Task *)test3_tsk_hdl)->delay_info.on_delay);
	printf("\r\n");
	*/
}

int main(void)
{
	RCC_ClocksTypeDef SysFrq;

	//system runtime delay init
	delay_init();
	SystemInit();
	RunTime_Init();
	VCP_Init();

	RCC_GetClocksFreq(&SysFrq);

#if DEBUG
	Debug_Init();
	Debug_Output("Task System V1.3\r\n");
	Debug_Output("flight controller blackbox test \r\n\r\n");
#endif

#if RADIO
	Radio_Init(MavLink_RecData_Parse, MavLink_CallBack);
	radio_tsk_hdl = Task_Create("Radio Task", task_type_radio_trans, TASK_EXEC_100HZ, Group_7, Task_Priority_7, Radio_Upload_Task, 0);
#endif

#if OSD
	OSD_Init();
#endif

	Led_Init(&Led, LED_IO_CLK, LED_IO_Pin, LED_IO_PORT);

	mpu_init();
	SBus_init();
	IMU_init();

	//create task
	//the smaller number of the group id the higher priority of the group u got the number of the task id goes the same way
	//gyro_sample_tsk_hdl = Task_Create("Gyro Sample Task", task_type_gyro_sample, TASK_EXEC_4KHZ, Group_0, Task_Priority_1, mpu_gyro_proc_task, 100);
	//acc_sample_tsk_hdl = Task_Create("Acc Sample Task", task_type_acc_sample, TASK_EXEC_1KHZ, Group_0, Task_Priority_2, mpu_acc_proc_task, 100);
	//imu_tsk_hdl = Task_Create("IMU Task", task_type_imu_comput, TASK_EXEC_1KHZ, Group_0, Task_Priority_3, IMU_Task, 200);

	test1_tsk_hdl = Task_Create("test 1", task_type_none, TASK_EXEC_4KHZ, Group_0, Task_Priority_1, Task_test_1, 200);
	test2_tsk_hdl = Task_Create("test 2", task_type_none, TASK_EXEC_2KHZ, Group_0, Task_Priority_0, Task_test_2, 200);
	test3_tsk_hdl = Task_Create("test 3", task_type_none, TASK_EXEC_1KHZ, Group_0, Task_Priority_2, Task_test_3, 200);
	test4_tsk_hdl = Task_Create("bitree test", task_type_none, TASK_EXEC_1KHZ, Group_0, Task_Priority_5, Task_test_4, 200);

#if DEBUG
	debug_tsk_hdl = Task_Create("debug Task", task_type_debug, TASK_EXEC_50HZ, Group_7, Task_Priority_7, Debug_OutputCheck, 100);
#endif

#if SDCARD
	blackbox_Init(LOGITEM(FLIGHT_LOG_FIELD_SELECT_GYRO) | LOGITEM(FLIGHT_LOG_FIELD_SELECT_ACC));
	blackbox_tsk_hdl = Task_Create("Blackbox Task", task_type_blackbox, TASK_EXEC_1KHZ, Group_0, Task_Priority_0, blackbox_Task, 1024);
#endif

	TaskSystem_Start();

	return 0;
}
