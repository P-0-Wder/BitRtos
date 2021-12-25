#ifndef __OS_WIDGET_H
#define __OS_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include "task_manager.h"

typedef struct
{
    bool (*init)();
    bool (*task_info)();
    bool (*show_task_list)();
    bool (*cpu_ocupy)();
    bool (*mem_remain)();
    bool (*os_timestamp)();
} OSCore_DisWidget_TypeDef;

#endif