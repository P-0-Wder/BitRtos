#ifndef __PERIPH_EXTI_H
#define __PERIPH_EXTI_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*exti_callback)(void);

void periph_exti_init();

#endif
