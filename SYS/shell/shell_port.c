#include "shell.h"
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "shell_port.h"
#include "drv_serial.h"

#define SHELL_BUFF_SIZE 512
#define Shell_SerialPort DrvSerial_1

static Shell shell;
char shell_buff[SHELL_BUFF_SIZE];
int dma_shell_buf[40] = {0};

static void user_shell_write(uint8_t *ch, uint16_t len)
{
	DrvSerial.write(Shell_SerialPort, ch, len);
}

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

static SYS_Shell_Serial_IRQ_Callback(uint8_t *data, uint16_t len)
{
}

void Shell_Init(void)
{
	DrvSerial_Config_Typedef Serial1_Cfg;

	Serial1_Cfg.baudrate = Serial_921600;
	Serial1_Cfg.PreemptionPriority = 3;
	Serial1_Cfg.SubPriority = 0;
	Serial1_Cfg.mode = DrvSerial_MODE_DMA_TxRx;
	Serial1_Cfg.Irq_Callback = SYS_Shell_Serial_IRQ_Callback;
	Serial1_Cfg.send_mode = DrvSerial_Send_Async;

	DrvSerial.ctl(DrvSerial_1, DrvSerial_Open, (uint32_t)&Serial1_Cfg, sizeof(Serial1_Cfg));

	shell.write = user_shell_write;

	shellInit(&shell, shell_buff, SHELL_BUFF_SIZE);
}
