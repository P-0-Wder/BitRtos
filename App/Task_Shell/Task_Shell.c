#include "Task_Shell.h"
#include "drv_serial.h"
#include "shell_port.h"
#include "oledfont.h"

Task_Handler Shell_Tsk_Hdl;

#define SHELL_SERIAL_PORT DrvSerial_1

/* internal variable */
TaskShell_State_List TaskState = Shell_State_Init;

static void Shell_RecCallback(uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        shellHandler(Shell_GetInstence(), data[i]);
    }
}

static int Shell_Write(const uint8_t *ch, uint16_t len)
{
    DrvSerial.write(SHELL_SERIAL_PORT, ch, len);
}

static bool TaskShell_Init(void)
{
    static TaskShell_State_List TaskState = Shell_State_Init;

    DrvSerial_Config_Typedef Serial1_Cfg;

    Serial1_Cfg.baudrate = Serial_115200;
    Serial1_Cfg.PreemptionPriority = 3;
    Serial1_Cfg.SubPriority = 0;
    Serial1_Cfg.mode = DrvSerial_MODE_DMA_TxRx;
    Serial1_Cfg.Irq_Callback = Shell_RecCallback;
    Serial1_Cfg.send_mode = DrvSerial_Send_Async;

    if (DrvSerial.ctl(SHELL_SERIAL_PORT, DrvSerial_Open, (uint32_t)&Serial1_Cfg, sizeof(Serial1_Cfg)))
    {
        Shell_Init(Shell_Write);
        return true;
    }

    return false;
}

static void TaskShell_InvertFont8(void)
{
    uint8_t inv = 0;
    uint8_t char_buff[7];

    char *test = "gjpqy";

    for (char c = 0; c <= strlen(test); c++)
    {
        shellPrint(Shell_GetInstence(), "%c :", test[c]);

        memset(char_buff, NULL, sizeof(char_buff));
        memcpy(char_buff, oled_asc2_0806[65 + (test[c] - 'a')], sizeof(char_buff));

        for (uint8_t i = 1; i < sizeof(oled_asc2_0806[0]); i++)
        {
            inv = 0;

            for (uint8_t b = 0; b < 8; b++)
            {
                if (char_buff[i] & (1 << b))
                {
                    inv |= 1 << (7 - b);
                }
            }

            shellPrint(Shell_GetInstence(), "0x%x, ", inv);
        }

        shellPrint(Shell_GetInstence(), "\r");
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, font, TaskShell_InvertFont8, invert font array);

void TaskShell_Core(Task_Handler self)
{
    switch ((uint8_t)TaskState)
    {
    case Shell_State_Init:
        TaskShell_Init();
        TaskState = Shell_State_Polling;
        break;

    case Shell_State_Polling:
        break;

    default:
        break;
    }
}
