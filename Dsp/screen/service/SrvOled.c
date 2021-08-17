#include "oled1306.h"
#include "drv_spi.h"
#include "drv_gpio.h"

Oled_Obj_TypeDef Oled1306Obj;

/* internal function definition */
static bool SrvOled_PreInit(void);

/* external function definition */
static bool SrvOled_Init(void);
static void SrvOled_Fresh(uint8_t **bit_map);

static bool SrvOled_PreInit(void)
{
    Oled1306Obj.Bus = Oled_SpiBus;
    Oled1306Obj.BusID = SPI_1;
}

static bool SrvOled_Init(void)
{
    if (SrvOled_PreInit())
    {
        DrvOled.init();

        return true;
    }

    return false;
}

static void SrvOled_Fresh(uint8_t **bit_map)
{
}
