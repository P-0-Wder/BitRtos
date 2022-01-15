#ifndef __VERSION_WIDGET_H
#define __VERSION_WIDGET_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "widget_mng.h"
#include "version_interface.h"

typedef enum
{
    VersionDspStage_Init = 0,
    VersionDspStage_Update,
    VersionDspStage_Error,
    VersionDspStage_Sum,
} VersionWidget_DspStage_List;

VersionWidget_DspStage_List VersionWidget_Update(Widget_Handle hdl);

#endif
