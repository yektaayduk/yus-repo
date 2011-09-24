################################
# project: STM32 LED Blinker
#    file: Makefile
###############################

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

LKR_SCRIPT = stm32_flash.ld

FAMILY = STM32F10X_MD
INCLUDES = -I./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/ \
			-I./Libraries/CMSIS/CM3/CoreSupport/ \
			-I./Libraries/STM32F10x_StdPeriph_Driver/inc

CFLAGS  = -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb -D$(FAMILY) $(INCLUDES)
AFLAGS  = -mcpu=cortex-m3 -mthumb 
LFLAGS  = -nostartfiles -T$(LKR_SCRIPT)
CPFLAGS = -Obinary

all: main.bin

main.o: main.c
	$(CC) $(CFLAGS) -o main.o main.c

startup.o: startup.s
	$(AS) $(AFLAGS) -o startup.o startup.s

main.elf: main.o startup.o
	$(LD) $(LFLAGS) -o main.elf main.o startup.o

main.bin: main.elf
	$(CP) $(CPFLAGS) main.elf main.bin

clean:
	rm -rf *.o *.elf *.bin

program:
	python stm32loader.py -p /dev/ttyUSB0 -e -w -v main.bin

