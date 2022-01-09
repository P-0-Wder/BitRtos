#ifndef __TASKINFO_WIDGET_H
#define __TASKINFO_WIDGET_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "widget_mng.h"

bool TaskInfo_SetStage(int8_t offset);
bool TaskInfo_DspUpdate(Widget_Handle hdl);
#endif
