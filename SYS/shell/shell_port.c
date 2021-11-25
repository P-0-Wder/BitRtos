#include "shell.h"
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "shell_port.h"
#include "drv_serial.h"

#define SHELL_BUFF_SIZE SERIAL_MAX_RECLEN

static Shell shell;
static char shell_buff[SHELL_BUFF_SIZE];

/*can print the characters to the terminal with this function */
static void user_printf_shell(const int *ch, ...)
{
	shellWriteString(&shell, ch);
}

static void bt_printf(const char *fmt, ...)
{
	va_list args;
	static char *bt_log_buf = NULL;
	int i = 0;
	int x = 0;
	va_start(args, fmt);

	vsnprintf(bt_log_buf, strlen(bt_log_buf) - 1, fmt, args);
	user_printf_shell(bt_log_buf);
	va_end(args);
}

void Shell_Init(Shell_Write_Callback callback)
{
	shell.write = callback;

	shellInit(&shell, shell_buff, sizeof(shell_buff));
}

Shell *Shell_GetInstence(void)
{
	return &shell;
}