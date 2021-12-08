#ifndef __TASK_ENCODER_H
#define __TASK_ENCODE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "task_manager.h"

typedef enum
{
    encoder_init_sta,
    encoder_get_sta,
}encoder_sta_list;

typedef struct 
{
    int num_rotation_circle;
    int encoder_key;
}encoder_sta_getlist_t;

void Task_Encoder_Core(Task_Handler self);
void Task_Encoder_Init(void);
extern Task_Handler Encoder_Tsk_Hdl;

#endif 
