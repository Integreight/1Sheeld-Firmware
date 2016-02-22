# MCU name
MCU = atmega162

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET = firmware

# List C source files here. (C dependencies are automatically generated.)
SRC =main.c  onesheeld.c mapping.c firmata.c gpio.c pwm.c timers.c uart.c

# Create object files and object files directories 
OBJECTDIRDEBUG = build/debug
OBJECTDIRRELEASE = build/release
OBJRELEASE = $(OBJECTDIRRELEASE)/firmata.o $(OBJECTDIRRELEASE)/gpio.o $(OBJECTDIRRELEASE)/main.o $(OBJECTDIRRELEASE)/mapping.o $(OBJECTDIRRELEASE)/onesheeld.o $(OBJECTDIRRELEASE)/pwm.o \
$(OBJECTDIRRELEASE)/timers.o $(OBJECTDIRRELEASE)/uart.o 
OBJDEBUG = $(OBJECTDIRDEBUG)/firmata.o $(OBJECTDIRDEBUG)/gpio.o $(OBJECTDIRDEBUG)/main.o $(OBJECTDIRDEBUG)/mapping.o $(OBJECTDIRDEBUG)/onesheeld.o $(OBJECTDIRDEBUG)/pwm.o \
$(OBJECTDIRDEBUG)/timers.o $(OBJECTDIRDEBUG)/uart.o 
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
LDFLAGSDEBUG = -Wl,-Map="$(OBJECTDIRDEBUG)/$(TARGET).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega162 -B.
LDFLAGSRELEASE = -Wl,-Map="$(OBJECTDIRRELEASE)/$(TARGET).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega162 -B.
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
GENDEPFLAGSDEBUG = -MD -MP -MF $(OBJECTDIRDEBUG)/$(@F).d -MT.$(OBJECTDIRDEBUG)/$(@F).d -MT$(OBJECTDIRDEBUG)/$(@F)
GENDEPFLAGSRELEASE = -MD -MP -MF $(OBJECTDIRRELEASE)/$(@F).d -MT.$(OBJECTDIRRELEASE)/$(@F).d -MT$(OBJECTDIRRELEASE)/$(@F)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS_DEBUG = -mmcu=$(MCU) -I. $(CFLAGS_DEBUG) $(CFLAGS) $(GENDEPFLAGSDEBUG)
ALL_CFLAGS_RELEASE = -mmcu=$(MCU) -I. $(CFLAGS_RELEASE) $(CFLAGS) $(GENDEPFLAGSRELEASE)
#####################################################################################################################
# Default target.
all: debug release
debug: begin gccversion sizebefore_debug build_debug sizeafter_debug end
release: begin gccversion sizebefore_release build_release sizeafter_release end
# Change the build target to build a HEX file or a library.
build_debug: elf_debug hex_debug eep_debug lss_debug sym_debug bin_debug
build_release: elf_release hex_release eep_release lss_release sym_release bin_release

elf_debug: $(OBJECTDIRDEBUG)/$(TARGET).elf
hex_debug: $(OBJECTDIRDEBUG)/$(TARGET).hex
eep_debug: $(OBJECTDIRDEBUG)/$(TARGET).eep
lss_debug: $(OBJECTDIRDEBUG)/$(TARGET).lss
sym_debug: $(OBJECTDIRDEBUG)/$(TARGET).sym
bin_debug: $(OBJECTDIRDEBUG)/$(TARGET).bin

elf_release: $(OBJECTDIRRELEASE)/$(TARGET).elf
hex_release: $(OBJECTDIRRELEASE)/$(TARGET).hex
eep_release: $(OBJECTDIRRELEASE)/$(TARGET).eep
lss_release: $(OBJECTDIRRELEASE)/$(TARGET).lss
sym_release: $(OBJECTDIRRELEASE)/$(TARGET).sym
bin_release: $(OBJECTDIRRELEASE)/$(TARGET).bin

begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
ELFSIZE_DEBUG = $(SIZE) --mcu=$(MCU) --format=avr $(OBJECTDIRDEBUG)/$(TARGET).elf
ELFSIZE_RELEASE = $(SIZE) --mcu=$(MCU) --format=avr $(OBJECTDIRRELEASE)/$(TARGET).elf

sizebefore_debug:
	@if test -f $(OBJECTDIRDEBUG)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE_DEBUG); \
	2>/dev/null; echo; fi

sizeafter_debug:
	@if test -f $(OBJECTDIRDEBUG)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE_DEBUG); \
	2>/dev/null; echo; fi

sizebefore_release:
	@if test -f $(OBJECTDIRRELEASE)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE_RELEASE); \
	2>/dev/null; echo; fi

sizeafter_release:
	@if test -f $(OBJECTDIRRELEASE)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE_RELEASE); \
	2>/dev/null; echo; fi

# Display compiler version information.
gccversion : 
	@$(CC) --version

flashdebug:
	avrdude -c usbasp -p m162 -P usb -v -u -D -U efuse:w:0xfb:m -U hfuse:w:0xd8:m -U lfuse:w:0xfd:m -u -U flash:w:$(OBJECTDIRDEBUG)/$(TARGET).hex
	
flashrelease:
	avrdude -c usbasp -p m162 -P usb -v -u -D -U efuse:w:0xfb:m -U hfuse:w:0xd8:m -U lfuse:w:0xfd:m -u -U flash:w:$(OBJECTDIRRELEASE)/$(TARGET).hex

erase: 
	avrdude -c usbasp -p m162 -P usb -v -u -e

# Create final output files (.hex, .eep) from ELF output file.
$(OBJECTDIRDEBUG)/%.hex: $(OBJECTDIRDEBUG)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "$(OBJECTDIRDEBUG)/$(TARGET).elf" "$(OBJECTDIRDEBUG)/$(TARGET).hex"


$(OBJECTDIRDEBUG)/%.eep: $(OBJECTDIRDEBUG)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OBJECTDIRDEBUG)/$(TARGET).elf" "$(OBJECTDIRDEBUG)/$(TARGET).eep" || exit 0
	#-j .eeprom --set-section-flags=.eeprom="alloc,load" \
	#--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
$(OBJECTDIRDEBUG)/%.lss: $(OBJECTDIRDEBUG)/%.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
$(OBJECTDIRDEBUG)/%.sym: $(OBJECTDIRDEBUG)/%.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
# Create a binary output file from ELF output file 
$(OBJECTDIRDEBUG)/%.bin: $(OBJECTDIRDEBUG)/%.elf 
	@echo
	@echo $(MSG_BINARY_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Link: create ELF output file from object files.
.SECONDARY : $(OBJECTDIRDEBUG)/$(TARGET).elf
.PRECIOUS : $(OBJDEBUG)
$(OBJECTDIRDEBUG)/%.elf: $(OBJDEBUG)
	@echo
	@echo $(MSG_LINKING) $<
	$(CC) -o $(OBJECTDIRDEBUG)/$(TARGET).elf $(OBJDEBUG) $(LDFLAGSDEBUG) 
	
# Compile: create object files from C source files.
$(OBJECTDIRDEBUG)/%.o : %.c
	$(shell mkdir build 2>/dev/null)
	$(shell mkdir $(OBJECTDIRDEBUG) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_DEBUG)  $< -o $@   
#####################################################################################################################
# Create final output files (.hex, .eep) from ELF output file :Release version.
$(OBJECTDIRRELEASE)/%.hex: $(OBJECTDIRRELEASE)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "$(OBJECTDIRRELEASE)/$(TARGET).elf" "$(OBJECTDIRRELEASE)/$(TARGET).hex"


$(OBJECTDIRRELEASE)/%.eep: $(OBJECTDIRRELEASE)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OBJECTDIRRELEASE)/$(TARGET).elf" "$(OBJECTDIRRELEASE)/$(TARGET).eep" || exit 0
	#-j .eeprom --set-section-flags=.eeprom="alloc,load" \
	#--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
$(OBJECTDIRRELEASE)/%.lss: $(OBJECTDIRRELEASE)/%.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
$(OBJECTDIRRELEASE)/%.sym: $(OBJECTDIRRELEASE)/%.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
# Create a binary output file from ELF output file 
$(OBJECTDIRRELEASE)/%.bin: $(OBJECTDIRRELEASE)/%.elf 
	@echo
	@echo $(MSG_BINARY_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Link: create ELF output file from object files.
.SECONDARY : $(OBJECTDIRRELEASE)/$(TARGET).elf
.PRECIOUS : $(OBJRELEASE)
$(OBJECTDIRRELEASE)/%.elf: $(OBJRELEASE)
	@echo
	@echo $(MSG_LINKING) $<
	$(CC) -o $(OBJECTDIRRELEASE)/$(TARGET).elf $(OBJRELEASE) $(LDFLAGSRELEASE) 
	
# Compile: create object files from C source files.
$(OBJECTDIRRELEASE)/%.o : %.c
	$(shell mkdir build 2>/dev/null)	
	$(shell mkdir $(OBJECTDIRRELEASE) 2>/dev/null)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS_RELEASE)  $< -o $@   
#####################################################################################################################
# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVEDIR) build

# Create object files directory


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion debug release\
build elf hex bin eep lss clean clean_list flashdebug flashrelease erase


