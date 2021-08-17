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

/******************************* IO & Bus Init Function Section ***********************************/

static void SrvOled_BusInit(SPI_List BusID)
{
    DrvSpi_Obj_TypeDef BusInit_Structure;

    BusInit_Structure.SPIx = BusID;
    BusInit_Structure.speed = SPI_CLOCK_INITIALIZATON;
    BusInit_Structure.CPOL = SPI_CPOL_High;
    BusInit_Structure.CPHA = SPI_CPHA_2Edge;

    GenSPI_Drv.open(&BusInit_Structure);
}

static void SrvOled_DCPin_Init(void)
{
    GenGPIO_Drv.open();
}

static void SrvOled_RSPin_Init(void)
{
    GenGPIO_Drv.open();
}

static void SrvOled_DCPin_Ctl(Oled_DC_State_List state)
{
}

static void SrvOled_RSPin_Ctl(Oled_RS_State_List state)
{
}

/******************************* IO & Bus Init Function Section ***********************************/

static bool SrvOled_PreInit(void)
{
    Oled1306Obj.Bus = Oled_SpiBus;
    Oled1306Obj.BusID = SPI_1;

    Oled1306Obj.bus_init = SrvOled_BusInit;

    Oled1306Obj.rs_init = SrvOled_RSPin_Init;
    Oled1306Obj.rs_ctl = SrvOled_RSPin_Ctl;

    Oled1306Obj.dc_init = SrvOled_DCPin_Init;
    Oled1306Obj.dc_ctl = SrvOled_DCPin_Ctl;
}

static bool SrvOled_Init(void)
{
    if (SrvOled_PreInit())
    {
        DrvOled.init(&Oled1306Obj);

        return true;
    }

    return false;
}

static void SrvOled_Fresh(uint8_t **bit_map)
{
}
