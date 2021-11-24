#ifndef __SHELL_PORT_H
#define __SHELL_PORT_H

#include "sys.h"
#include "shell.h"

extern Shell shell;

void Shell_Init(void);
void user_shell_write_byte(const int *ch,...);

#endif

