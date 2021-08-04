#ifndef __DELAY_H
#define __DELAY_H
#include <sys.h>

void delay_init(void);
void delay_us(uint32_t US);
void delay_ms(uint32_t MS);

#endif
