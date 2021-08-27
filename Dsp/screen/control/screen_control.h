#ifndef __SCREEN_CONTROL_H
#define __SCREEN_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Oled_DspProp_Def.h"

void Screen_Routate(Oled_Routate_Direction_Def dir);
void Screen_Mirror(Oled_Mirror_Direction_Def dir);

#endif
