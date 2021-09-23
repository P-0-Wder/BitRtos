##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.7.1] date: [Thu Mar 19 10:12:08 CST 2020]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = BitRTOS

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

platrom = macos

#######################################
# paths
#######################################
# Build path
BUILD_DIR = Objects

######################################
# source
######################################
# C sources
C_SOURCES =  \
SYS/data_structure/queue.c \
SYS/data_structure/linked_list.c \
SYS/data_structure/binary_tree.c \
SYS/faultcheck/fault_check.c \
SYS/delay/delay.c \
SYS/runtime/runtime.c \
SYS/task_manager/task_manager.c \
SYS/common/reboot.c \
SYS/common/sys.c \
SYS/crc/crc16.c \
Dsp/screen/font/oledfont.c \
Dsp/screen/device/oled1306.c \
Dsp/screen/service/SrvOled.c \
Dsp/widget/wgt_core/widget_mng.c \
Dsp/widget/wgt_core/UI_Controller.c \
Dsp/wgt_core/GenDsp.c \
FWLIB/src/misc.c \
FWLIB/src/stm32f4xx_adc.c \
FWLIB/src/stm32f4xx_can.c \
FWLIB/src/stm32f4xx_cec.c \
FWLIB/src/stm32f4xx_crc.c \
FWLIB/src/stm32f4xx_cryp_aes.c \
FWLIB/src/stm32f4xx_cryp_des.c \
FWLIB/src/stm32f4xx_cryp_tdes.c \
FWLIB/src/stm32f4xx_cryp.c \
FWLIB/src/stm32f4xx_dac.c \
FWLIB/src/stm32f4xx_dbgmcu.c \
FWLIB/src/stm32f4xx_dcmi.c \
FWLIB/src/stm32f4xx_dfsdm.c \
FWLIB/src/stm32f4xx_dma.c \
FWLIB/src/stm32f4xx_dma2d.c \
FWLIB/src/stm32f4xx_dsi.c \
FWLIB/src/stm32f4xx_exti.c \
FWLIB/src/stm32f4xx_flash_ramfunc.c \
FWLIB/src/stm32f4xx_flash.c \
FWLIB/src/stm32f4xx_fmpi2c.c \
FWLIB/src/stm32f4xx_gpio.c \
FWLIB/src/stm32f4xx_hash_md5.c \
FWLIB/src/stm32f4xx_hash_sha1.c \
FWLIB/src/stm32f4xx_hash.c \
FWLIB/src/stm32f4xx_i2c.c \
FWLIB/src/stm32f4xx_iwdg.c \
FWLIB/src/stm32f4xx_lptim.c \
FWLIB/src/stm32f4xx_ltdc.c \
FWLIB/src/stm32f4xx_pwr.c \
FWLIB/src/stm32f4xx_qspi.c \
FWLIB/src/stm32f4xx_rcc.c \
FWLIB/src/stm32f4xx_rng.c \
FWLIB/src/stm32f4xx_rtc.c \
FWLIB/src/stm32f4xx_sai.c \
FWLIB/src/stm32f4xx_sdio.c \
FWLIB/src/stm32f4xx_spdifrx.c \
FWLIB/src/stm32f4xx_spi.c \
FWLIB/src/stm32f4xx_syscfg.c \
FWLIB/src/stm32f4xx_tim.c \
FWLIB/src/stm32f4xx_usart.c \
FWLIB/src/stm32f4xx_wwdg.c \
Bsp/periph_dma.c \
Bsp/periph_flash.c \
Bsp/periph_gpio.c \
Bsp/periph_iic.c \
Bsp/periph_nvic.c \
Bsp/periph_serial_irq.c \
Bsp/periph_serial.c \
Bsp/periph_spi.c \
Bsp/periph_timer_irq.c \
Bsp/periph_timer.c \
Bsp/periph_dma_irq.c \
Bsp/periph_adc.c \
Drv/drv_adc.c \
Drv/drv_spi.c \
Drv/drv_gpio.c \
Drv/drv_serial.c \
USB/config/vcp.c \
USB/config/tm_stm32f4_usb_vcp.c \
USB/core/usb_bsp.c \
USB/core/usb_core.c \
USB/core/usb_dcd_int.c \
USB/core/usb_dcd.c \
USB/core/usbd_cdc_core.c \
USB/core/usbd_cdc_vcp.c \
USB/core/usbd_core.c \
USB/core/usbd_desc.c \
USB/core/usbd_ioreq.c \
USB/core/usbd_req.c \
USB/core/usbd_usr.c \
USER/stm32f4xx_it.c\
USER/system_stm32f4xx.c \
USER/main.c \
App/Task_Display/Task_Widget.c \
App/Task_Input/SrvInput.c \

# ASM sources
ASM_SOURCES =  \
startup_stm32f405xx.s
#startup_stm32f411xe.s

#######################################
# binaries
#######################################

ifeq ($(plafrom), win)
BINPATH = C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2020-q4-major/bin
endif

ifeq ($(plafrom), macos)
BINPATH = /usr/local/arm-none-eabi-gcc/gcc-arm-none-eabi-10-2020-q4-major/bin/
endif

PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-D__FPU_PRESENT=1 \
-D__FPU_USED=1 \

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IF4_CORE \
-IFWLIB/inc \
-IBsp \
-IDrv \
-IDsp \
-IDsp/screen/OledIO_Definition \
-IDsp/screen/service \
-IDsp/screen/device \
-IDsp/screen/font \
-IDsp/widget/wgt_core \
-ISYS/task_manager \
-ISYS/data_structure \
-ISYS/runtime \
-ISYS/delay \
-ISYS/mem \
-ISYS/common \
-ISYS/mutex \
-ISYS/os_cli \
-ISYS/event \
-ISYS/task \
-ISYS/crc \
-ISYS/faultcheck \
-IUSB/config \
-IUSB/core \
-IUSER \
-IApp/Task_Display \
-IApp/Task_Input \

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F405VGTx_FLASH.ld
#LDSCRIPT = STM32F411CEUx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys -u_printf_float
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
# -rm -fR $(BUILD_DIR)
# -rd /s /q $(BUILD_DIR)
#######################################
clean:
	-rd /s /q $(BUILD_DIR)
	
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

OPENOCD := openocd -f interface/cmsis-dap.cfg \
        -f target/stm32f4x.cfg \

# download your program
flash: all
	$(OPENOCD) -c init \
        -c 'reset halt' \
        -c 'flash write_image erase $(BUILD_DIR)/$(TARGET).elf' \
        -c 'reset run' \
        -c exit
debug:
	$(OPENOCD)

	