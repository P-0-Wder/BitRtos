#include "oled1306.h"
#include "drv_spi.h"
#include "drv_gpio.h"

Oled_Obj_TypeDef Oled1306Obj;

/* internal function definition */
static bool SrvOled_PreInit(void);
static void SrvOled_BusInit(SPI_List BusID);

/* external function definition */
static bool SrvOled_Init(void);
static void SrvOled_Fresh(uint8_t **bit_map);

static void SrvOled_BusInit(SPI_List BusID)
{
    DrvSpi_Obj_TypeDef BusInit_Structure;

    BusInit_Structure.SPIx = BusID;
    BusInit_Structure.speed = SPI_CLOCK_INITIALIZATON;
    BusInit_Structure.CPOL = ;
    BusInit_Structure.CPHA = ;

    GenSPI_Drv.open(&BusInit_Structure);
}

static bool SrvOled_PreInit(void)
{
    Oled1306Obj.Bus = Oled_SpiBus;
    Oled1306Obj.BusID = SPI_1;

    Oled1306Obj.bus_init = SrvOled_BusInit;
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
