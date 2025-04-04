###############################################################################
# Makefile for the project Example
###############################################################################

## General Flags
PROJECT = Example
MCU = atmega128
TARGET = Example.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                        -DF_CPU=16000000UL -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Example.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\02_E_Drive\01_HW\01_AVR\01_FW\Atmega128_01\120324\." 

## Objects that must be built in order to link
OBJECTS = Main.o Interrupt.o User.o Cpu.o XbeeController.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Example.hex Example.eep Example.lss size

## Compile
Main.o: ../Main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Interrupt.o: ../Interrupt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

User.o: ../User.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Cpu.o: ../Cpu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

XbeeController.o: ../XbeeController.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Example.elf dep/* Example.hex Example.eep Example.lss Example.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

