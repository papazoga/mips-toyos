MCU=32MZ2048EFH144
TOOL=SBUR153373495
MICROCHIP_DIR=/opt/microchip
XC32_DIR=$(MICROCHIP_DIR)/xc32/v1.40
IPE_DIR=$(MICROCHIP_DIR)/mplabx/v3.26/mplab_ipe
BINDIR=$(XC32_DIR)/bin
CC=$(BINDIR)/xc32-gcc
BIN2HEX=$(BINDIR)/xc32-bin2hex
CFLAGS=-mprocessor=$(MCU) -Wall
ASFLAGS=-mprocessor=$(MCU)
IPECMD=$(IPE_DIR)/ipecmd.sh

OBJS=main.o timer.o interrupt.o sched.o asm.o blinky.o fifo.o uart.o

all: blinky.elf

blinky.elf: $(OBJS)
	$(CC) -mprocessor=$(MCU) $(OBJS) -o blinky.elf
	$(BIN2HEX) blinky.elf

clean:
	rm -f *.o *.elf *.hex
