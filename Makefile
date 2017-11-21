# MCU name
MCU = atmega162

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET = firmware

# List C source files here. (C dependencies are automatically generated.)
SRC = onesheeld.c firmata.c eeprom.c $(MCU)/gpio.c $(MCU)/mapping.c $(MCU)/pwm.c $(MCU)/timers.c $(MCU)/uart.c main.c

#change these variables according to your lib dirs
DEV_SPECS = "C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\ATmega_DFP\1.2.132\gcc\dev"
INCLUDE_DIR = "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\include"

# determine the building configuration debug or release
CONFIG = DEBUG
LOWER_VAR_CONFIG  =  $(shell echo $(CONFIG) | tr A-Z a-z)
# Create object files and object files directories 
OBJECTDIRCLASSIC = build/$(MCU)/classic/$(LOWER_VAR_CONFIG)
OBJECTDIRPLUS = build/$(MCU)/plus/$(LOWER_VAR_CONFIG)
OBJPLUS = $(OBJECTDIRPLUS)/firmata.o $(OBJECTDIRPLUS)/gpio.o $(OBJECTDIRPLUS)/main.o $(OBJECTDIRPLUS)/mapping.o $(OBJECTDIRPLUS)/onesheeld.o $(OBJECTDIRPLUS)/pwm.o \
$(OBJECTDIRPLUS)/timers.o $(OBJECTDIRPLUS)/uart.o $(OBJECTDIRPLUS)/eeprom.o
OBJCLASSIC = $(OBJECTDIRCLASSIC)/firmata.o $(OBJECTDIRCLASSIC)/gpio.o $(OBJECTDIRCLASSIC)/main.o $(OBJECTDIRCLASSIC)/mapping.o $(OBJECTDIRCLASSIC)/onesheeld.o $(OBJECTDIRCLASSIC)/pwm.o\
$(OBJECTDIRCLASSIC)/timers.o $(OBJECTDIRCLASSIC)/uart.o $(OBJECTDIRCLASSIC)/eeprom.o 
# Optimization level, can be [0, 1, 2, 3, s]. 
OPT_DEBUG = 1
OPT_RELEASE = s
# Compiler flag to set the C Standard level.
CSTANDARD = -std=gnu99
CDEFS_DEBUG = -DDEBUG
CDEFS_RELEASE = -DNDEBUG

CFLAGS_DEBUG = -g2 $(CDEFS_DEBUG) -O$(OPT_DEBUG)
CFLAGS_RELEASE = $(CDEFS_RELEASE) -O$(OPT_RELEASE)
#---------------- Compiler Options C ----------------
CFLAGS = -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

# Math library
MATH_LIB = -lm

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGSCLASSIC = -Wl,-Map="$(OBJECTDIRCLASSIC)/$(TARGET).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(MCU) -B$(DEV_SPECS)/$(MCU)
LDFLAGSPLUS = -Wl,-Map="$(OBJECTDIRPLUS)/$(TARGET).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(MCU) -B$(DEV_SPECS)/$(MCU)
#####################################################################################################################

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_CLEANING = Cleaning project:
MSG_BINARY_FILE = Creating binary output file:


# Compiler flags to generate dependency files.
GENDEPFLAGSCLASSIC = -MD -MP -MF $(OBJECTDIRCLASSIC)/$(@F).d -MT.$(OBJECTDIRCLASSIC)/$(@F).d -MT$(OBJECTDIRCLASSIC)/$(@F)
GENDEPFLAGSPLUS = -MD -MP -MF $(OBJECTDIRPLUS)/$(@F).d -MT.$(OBJECTDIRPLUS)/$(@F).d -MT$(OBJECTDIRPLUS)/$(@F)

# Combine all necessary flags and optional flags.
# Add target processor to flags.

ALL_CFLAGS_CLASSIC = -mmcu=$(MCU) -I$(MCU)/ -I. -I$(INCLUDE_DIR) -B$(DEV_SPECS)/$(MCU) -DCLASSIC_BOARD $(CFLAGS_$(CONFIG)) $(CFLAGS) $(GENDEPFLAGSCLASSIC)
ALL_CFLAGS_PLUS = -mmcu=$(MCU) -I$(MCU)/ -I. -I$(INCLUDE_DIR) -B$(DEV_SPECS)/$(MCU) -DPLUS_BOARD $(CFLAGS_$(CONFIG)) $(CFLAGS) $(GENDEPFLAGSPLUS)
#####################################################################################################################
# Default target.
all: classic plus
classic: begin gccversion sizebefore_classic build_classic sizeafter_classic end
plus: begin gccversion sizebefore_plus build_plus sizeafter_plus end
# Change the build target to build a HEX file or a library.
build_classic: elf_classic hex_classic eep_classic lss_classic sym_classic bin_classic
build_plus: elf_plus hex_plus eep_plus lss_plus sym_plus bin_plus

elf_classic: $(OBJECTDIRCLASSIC)/$(TARGET).elf
hex_classic: $(OBJECTDIRCLASSIC)/$(TARGET).hex
eep_classic: $(OBJECTDIRCLASSIC)/$(TARGET).eep
lss_classic: $(OBJECTDIRCLASSIC)/$(TARGET).lss
sym_classic: $(OBJECTDIRCLASSIC)/$(TARGET).sym
bin_classic: $(OBJECTDIRCLASSIC)/$(TARGET).bin

elf_plus: $(OBJECTDIRPLUS)/$(TARGET).elf
hex_plus: $(OBJECTDIRPLUS)/$(TARGET).hex
eep_plus: $(OBJECTDIRPLUS)/$(TARGET).eep
lss_plus: $(OBJECTDIRPLUS)/$(TARGET).lss
sym_plus: $(OBJECTDIRPLUS)/$(TARGET).sym
bin_plus: $(OBJECTDIRPLUS)/$(TARGET).bin

begin:
ifneq ($(CONFIG),DEBUG)
ifneq ($(CONFIG),RELEASE)
	$(error please define CONFIG variable as DEBUG or RELEASE)
endif
endif
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
ELFSIZE_CLASSIC = $(SIZE) --mcu=$(MCU) --format=avr $(OBJECTDIRCLASSIC)/$(TARGET).elf
ELFSIZE_PLUS = $(SIZE) --mcu=$(MCU) --format=avr $(OBJECTDIRPLUS)/$(TARGET).elf

sizebefore_classic:
	@if test -f $(OBJECTDIRCLASSIC)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE_CLASSIC); \
	2>/dev/null; echo; fi

sizeafter_classic:
	@if test -f $(OBJECTDIRCLASSIC)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE_CLASSIC); \
	2>/dev/null; echo; fi

sizebefore_plus:
	@if test -f $(OBJECTDIRPLUS)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE_PLUS); \
	2>/dev/null; echo; fi

sizeafter_plus:
	@if test -f $(OBJECTDIRPLUS)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE_PLUS); \
	2>/dev/null; echo; fi

# Display compiler version information.
gccversion : 
	@$(CC) --version

flashclassic:
	avrdude -c usbasp -p $(MCU) -P usb -v -u -D -U efuse:w:0xfb:m -U hfuse:w:0xd8:m -U lfuse:w:0xfd:m -u -U flash:w:$(OBJECTDIRCLASSIC)/$(TARGET).hex
	
flashplus:
	avrdude -c usbasp -p $(MCU) -P usb -v -u -D -U efuse:w:0xfb:m -U hfuse:w:0xd8:m -U lfuse:w:0xfd:m -u -U flash:w:$(OBJECTDIRPLUS)/$(TARGET).hex

erase: 
	avrdude -c usbasp -p $(MCU) -P usb -v -u -e

# Create final output files (.hex, .eep) from ELF output file.
$(OBJECTDIRCLASSIC)/%.hex: $(OBJECTDIRCLASSIC)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "$(OBJECTDIRCLASSIC)/$(TARGET).elf" "$(OBJECTDIRCLASSIC)/$(TARGET).hex"


$(OBJECTDIRCLASSIC)/%.eep: $(OBJECTDIRCLASSIC)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OBJECTDIRCLASSIC)/$(TARGET).elf" "$(OBJECTDIRCLASSIC)/$(TARGET).eep" || exit 0
	#-j .eeprom --set-section-flags=.eeprom="alloc,load" \
	#--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
$(OBJECTDIRCLASSIC)/%.lss: $(OBJECTDIRCLASSIC)/%.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
$(OBJECTDIRCLASSIC)/%.sym: $(OBJECTDIRCLASSIC)/%.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
# Create a binary output file from ELF output file 
$(OBJECTDIRCLASSIC)/%.bin: $(OBJECTDIRCLASSIC)/%.elf 
	@echo
	@echo $(MSG_BINARY_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Link: create ELF output file from object files.
.SECONDARY : $(OBJECTDIRCLASSIC)/$(TARGET).elf
.PRECIOUS : $(OBJCLASSIC)
$(OBJECTDIRCLASSIC)/%.elf: $(OBJCLASSIC)
	@echo
	@echo $(MSG_LINKING) $<
	$(CC) -o $(OBJECTDIRCLASSIC)/$(TARGET).elf $(OBJCLASSIC) $(LDFLAGSCLASSIC) 
	
# Compile: create object files from C source files.
$(OBJECTDIRCLASSIC)/%.o : %.c
	$(shell mkdir build 2>/dev/null)
	$(shell mkdir -p $(OBJECTDIRCLASSIC) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_CLASSIC)  $< -o $@   
$(OBJECTDIRCLASSIC)/%.o : $(MCU)/%.c
	$(shell mkdir build 2>/dev/null)
	$(shell mkdir -p $(OBJECTDIRCLASSIC) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_CLASSIC)  $< -o $@ 
#####################################################################################################################
# Create final output files (.hex, .eep) from ELF output file :Release version.
$(OBJECTDIRPLUS)/%.hex: $(OBJECTDIRPLUS)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "$(OBJECTDIRPLUS)/$(TARGET).elf" "$(OBJECTDIRPLUS)/$(TARGET).hex"


$(OBJECTDIRPLUS)/%.eep: $(OBJECTDIRPLUS)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OBJECTDIRPLUS)/$(TARGET).elf" "$(OBJECTDIRPLUS)/$(TARGET).eep" || exit 0
	#-j .eeprom --set-section-flags=.eeprom="alloc,load" \
	#--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
$(OBJECTDIRPLUS)/%.lss: $(OBJECTDIRPLUS)/%.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
$(OBJECTDIRPLUS)/%.sym: $(OBJECTDIRPLUS)/%.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
# Create a binary output file from ELF output file 
$(OBJECTDIRPLUS)/%.bin: $(OBJECTDIRPLUS)/%.elf 
	@echo
	@echo $(MSG_BINARY_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Link: create ELF output file from object files.
.SECONDARY : $(OBJECTDIRPLUS)/$(TARGET).elf
.PRECIOUS : $(OBJPLUS)
$(OBJECTDIRPLUS)/%.elf: $(OBJPLUS)
	@echo
	@echo $(MSG_LINKING) $<
	$(CC) -o $(OBJECTDIRPLUS)/$(TARGET).elf $(OBJPLUS) $(LDFLAGSPLUS) 
	
# Compile: create object files from C source files.
$(OBJECTDIRPLUS)/%.o : %.c
	$(shell mkdir build 2>/dev/null)	
	$(shell mkdir -p $(OBJECTDIRPLUS) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_PLUS)  $< -o $@   
$(OBJECTDIRPLUS)/%.o : $(MCU)/%.c
	$(shell mkdir build 2>/dev/null)	
	$(shell mkdir -p $(OBJECTDIRPLUS) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_PLUS)  $< -o $@  
#####################################################################################################################
# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVEDIR) build

# Create object files directory


# Listing of phony targets.
.PHONY : all begin finish end sizebefore_classic build_classic sizeafter_classic elf_classic hex_classic eep_classic lss_classic sym_classic bin_classic elf_plus hex_plus eep_plus \ lss_plus sym_plus bin_plus sizebefore_plus build_plus sizeafter_plus clean clean_list flashclassic flashplus erase


