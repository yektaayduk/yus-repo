################################
# project: STM32 LED Blinker
#    file: Makefile
###############################

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

LKR_SCRIPT = stm32_flash.ld

CFLAGS  = -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb
LFLAGS  = -nostartfiles -T$(LKR_SCRIPT)
CPFLAGS = -Obinary

all: main.bin

main.o: main.c
	$(CC) $(CFLAGS) -o main.o main.c

main.elf: main.o
	$(LD) $(LFLAGS) -o main.elf main.o

main.bin: main.elf
	$(CP) $(CPFLAGS) main.elf main.bin

clean:
	rm -rf *.o *.elf *.bin

program:
	python stm32loader.py -p /dev/ttyUSB0 -e -w -v main.bin

