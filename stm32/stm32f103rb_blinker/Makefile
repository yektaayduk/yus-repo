main.o: main.c
	arm-none-eabi-gcc -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb -o main.o main.c

main.elf: main.o
	arm-none-eabi-ld -nostartfiles -Tstm32_flash.ld -o main.elf main.o

main.bin: main.elf
	arm-none-eabi-objcopy -Obinary main.elf main.bin
