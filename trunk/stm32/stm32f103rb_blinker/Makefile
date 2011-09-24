################################
# project: STM32 LED Blinker
#    file: Makefile
###############################

CC  = arm-none-eabi-gcc
LD  = arm-none-eabi-ld
CP  = arm-none-eabi-objcopy

all: main.bin

main.o: main.c
	$(CC) -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb -o main.o main.c

main.elf: main.o
	$(LD) -nostartfiles -Tstm32_flash.ld -o main.elf main.o

main.bin: main.elf
	$(CP) -Obinary main.elf main.bin

clean:
	rm -rf *.o *.elf *.bin

program:
	python stm32loader.py -p /dev/ttyUSB0 -e -w -v main.bin

