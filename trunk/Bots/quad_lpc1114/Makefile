

PROJECT = gbot
DEVICE = LPC1114x302

PROJECT_DIR := .#$(shell pwd)
SRC_DIR = $(PROJECT_DIR)/src
STARTUP_DIR = $(SRC_DIR)/startup
CMSIS_DIR = $(SRC_DIR)/cmsis
COOS_DIR = $(SRC_DIR)/CoOS
DRIVER_DIR = $(SRC_DIR)/driver
XUART_DIR = $(SRC_DIR)/xuart
QUAD_DIR = $(SRC_DIR)/quad
USER_DIR = $(SRC_DIR)/user
OUTPUT_DIR := $(PROJECT_DIR)/out

LKR_SCRIPT = $(SRC_DIR)/lpc1114_flash.ld

#---------------------------------------#
#STARTUP_OBJS = $(STARTUP_DIR)/cr_startup_lpc11.o
STARTUP_OBJS = $(STARTUP_DIR)/startup_lpc11xx.o
USER_OBJS = $(USER_DIR)/main.o $(USER_DIR)/quadtasks.o
XUART_OBJS = $(XUART_DIR)/xuart.o $(XUART_DIR)/xprintf.o
QUAD_OBJS = $(QUAD_DIR)/servo.o $(QUAD_DIR)/quad.o $(QUAD_DIR)/pwmtable.o

DRIVER_SRCS = $(wildcard $(DRIVER_DIR)/*.c)
DRIVER_OBJS = $(DRIVER_SRCS:%.c=%.o)

CMSIS_OBJS = $(CMSIS_DIR)/core_cm0.o $(CMSIS_DIR)/system_LPC11xx.o

COOS_KERNEL_SRCS = $(wildcard $(COOS_DIR)/kernel/*.c)
COOS_KERNEL_OBJS = $(COOS_KERNEL_SRCS:%.c=%.o)
COOS_PORT_OBJS = $(COOS_DIR)/portable/arch.o $(COOS_DIR)/portable/GCC/port.o

INCLUDES  = -I$(PROJECT_DIR) -I$(USER_DIR) -I$(QUAD_DIR)
INCLUDES += -I$(CMSIS_DIR) -I$(DRIVER_DIR) -I$(XUART_DIR)
INCLUDES += -I$(COOS_DIR)/kernel -I$(COOS_DIR)/portable

#--- ToolChain section ---#
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc # linker tool workaround
CP = $(CROSS_COMPILE)objcopy
OD = $(CROSS_COMPILE)objdump
SZ = $(CROSS_COMPILE)size
NM = $(CROSS_COMPILE)nm


OPT = 0 #optimize: 0, 1, 2, 3, or s

#--- Toolchain flags ---#
COMMON_FLAGS = -mcpu=cortex-m0 -mthumb
CFLAGS  = $(COMMON_FLAGS) -Wall -ffunction-sections -O$(OPT) -g3 -c -D$(DEVICE) $(INCLUDES) -D__NEWLIB__
LFLAGS  = $(COMMON_FLAGS) -nostartfiles --gc-sections -Wl,-Map=$(MAP_FILE) -T$(LKR_SCRIPT)
CPFLAGS = -Obinary
ODFLAGS = -S
NMFLAGS = -n

#--- toolchain outputs ---#
OBJECTS  = $(STARTUP_OBJS) $(USER_OBJS)
OBJECTS += $(DRIVER_OBJS) $(CMSIS_OBJS)
OBJECTS += $(XUART_OBJS) $(QUAD_OBJS)
OBJECTS += $(COOS_KERNEL_OBJS) $(COOS_PORT_OBJS)
ELF_FILE = $(OUTPUT_DIR)/$(PROJECT).elf
BIN_FILE = $(OUTPUT_DIR)/$(PROJECT).bin
LST_FILE = $(OUTPUT_DIR)/$(PROJECT).lst
MAP_FILE = $(OUTPUT_DIR)/$(PROJECT).map

#--- serial isp/iap ---#
LPC_ISP = $(PROJECT_DIR)/tool/lpc21isp
SERIALPORT = /dev/ttyUSB0
LPC_ISP_CMP = $(LPC_ISP) -donotstart -bin $(BIN_FILE) $(SERIALPORT) 115200 12000
TEST_BIN = /media/data/Electronics/ARM/NXP/firmware/lpcxpresso.examples.lpc1114.302/uart/Debug/uart.bin
#LPC_ISP_CMP = $(LPC_ISP) -bin $(TEST_BIN) $(SERIALPORT) 115200 12000


all: $(BIN_FILE)
	@echo optimize level = $(OPT) with code size:
	$(SZ) $(ELF_FILE)
	@echo build finished.

$(ELF_FILE): $(OBJECTS)
	mkdir -p $(OUTPUT_DIR)
	@echo linking objects...
	$(LD) $(LFLAGS) $^ -o $@

$(BIN_FILE): $(ELF_FILE)
	@echo creating binary image: $@
	$(CP) $(CPFLAGS) $< $@
	@echo creating code listing: $(LST_FILE)
	$(OD) $(ODFLAGS) $< > $(LST_FILE)
	@echo creating system mapping: $(MAP_FILE)
	$(NM) $(NMFLAGS) $< > $(MAP_FILE)

.c.o:
	@echo compiling: $(^F)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUTPUT_DIR) $(OBJECTS)

program:
	@echo running lpc21isp
	$(LPC_ISP_CMP)

