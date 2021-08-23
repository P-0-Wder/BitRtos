#include "oled1306.h"
#include "drv_spi.h"
#include "drv_gpio.h"
#include "OledIO_Def.h"
#include "SrvOled.h"

/* internal varible definition */
static Oled_Obj_TypeDef Oled1306Obj;
static DrvGPIO_Obj_TypeDef DC_IO_Obj;
static DrvGPIO_Obj_TypeDef RS_IO_Obj;
static bool SrvPerInit_State = false;
static bool SrvInit_State = false;

/* internal function definition */
static bool SrvOled_PreInit(void);
static void SrvOled_BusInit(SPI_List BusID);

/* external function definition */
static bool SrvOled_Init(void);
static bool SrvOled_Fresh(uint8_t **bit_map);
static SrvOled_DspRange SrvOled_GetDev_DspRange(void);

SrvOled_TypeDef SrvOled = {
    .init = SrvOled_Init,
    .fresh = SrvOled_Fresh,
    .get_range = SrvOled_GetDev_DspRange,
};

/******************************* IO & Bus Init Function Section ***********************************/

static void SrvOled_BusInit(SPI_List BusID)
{
    static DrvSpi_Obj_TypeDef BusInit_Structure;

    BusInit_Structure.SPIx = BusID;
    BusInit_Structure.speed = SPI_CLOCK_INITIALIZATON;
    BusInit_Structure.CPOL = SPI_CPOL_High;
    BusInit_Structure.CPHA = SPI_CPHA_2Edge;

    Oled1306Obj.BusObj = &BusInit_Structure;

    GenSPI_Drv.open(&BusInit_Structure);
}

static void SrvOled_BusTransmit(uint8_t tx)
{
    GenSPI_Drv.transmit((DrvSpi_Obj_TypeDef *)Oled1306Obj.BusObj, tx);
}

static void SrvOled_DCPin_Init(void)
{
    DC_IO_Obj.CLK = OLED1306_DC_PORT_CLK;
    DC_IO_Obj.Port = OLED1306_DC_PORT;
    DC_IO_Obj.Pin = OLED1306_DC_PIN;
    DC_IO_Obj.IO_Type = GPIO_Output;

    GenGPIO_Drv.open(&DC_IO_Obj, DC_IO_Obj.IO_Type);
}

static void SrvOled_RSPin_Init(void)
{
    RS_IO_Obj.CLK = OLED1306_RS_PORT_CLK;
    RS_IO_Obj.Port = OLED1306_RS_PORT;
    RS_IO_Obj.Pin = OLED1306_RS_PIN;
    RS_IO_Obj.IO_Type = GPIO_Output;

    GenGPIO_Drv.open(&RS_IO_Obj, RS_IO_Obj.IO_Type);
}

static void SrvOled_DCPin_Ctl(Oled_DC_State_List state)
{
    DrvGPIO_IO_Level_TypeDef level = GPIO_ERR;

    if (state == Oled_DC_Enable)
    {
        level = GPIO_LOW;
    }
    else if (state == Oled_DC_Disable)
        level = GPIO_HIGH;

    GenGPIO_Drv.set(&DC_IO_Obj, level);
}

static void SrvOled_RSPin_Ctl(Oled_RS_State_List state)
{
    DrvGPIO_IO_Level_TypeDef level = GPIO_ERR;

    if (state == Oled_RS_Enable)
    {
        level = GPIO_LOW;
    }
    else if (state == Oled_RS_Disable)
        level = GPIO_HIGH;

    GenGPIO_Drv.set(&RS_IO_Obj, level);
}

/******************************* IO & Bus Init Function Section ***********************************/

static bool SrvOled_PreInit(void)
{
    Oled1306Obj.Bus = Oled_SpiBus;
    Oled1306Obj.BusID = SPI_1;

    Oled1306Obj.bus_init = SrvOled_BusInit;
    Oled1306Obj.bus_transmit = SrvOled_BusTransmit;

    Oled1306Obj.rs_init = SrvOled_RSPin_Init;
    Oled1306Obj.rs_ctl = SrvOled_RSPin_Ctl;

    Oled1306Obj.dc_init = SrvOled_DCPin_Init;
    Oled1306Obj.dc_ctl = SrvOled_DCPin_Ctl;

    SrvPerInit_State = true;

    return SrvPerInit_State;
}

static bool SrvOled_Init(void)
{
    SrvInit_State = false;

    if (SrvOled_PreInit())
    {
        if (DrvOled.init(&Oled1306Obj))
        {
            SrvInit_State = true;
        }
    }

    return SrvInit_State;
}

static bool SrvOled_Fresh(uint8_t **bit_map)
{
    if (!SrvInit_State)
        return false;

    //hardfault triggerd
    DrvOled.fresh(&Oled1306Obj, bit_map);
    return true;
}

static SrvOled_DspRange SrvOled_GetDev_DspRange(void)
{
    SrvOled_DspRange range;

    range.width = DrvOled.get_max_width();
    range.height = DrvOled.get_max_height();

    return range;
}
