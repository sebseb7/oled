# makefile for OLED control with
# Atmel ATmega644 microcontroller
#
# adapted from WinAVR makefile sample
# by Cathy Saxton, robotics@idleloop.com
#
# on the command line:
#   make          - build the program (identical to 'make all')
#   make all      - build the program
#   make clean    - remove all built files
#   make program  - download program (using AVRDUDE defines below)
#   make setfuse  - set fuse bits (using AVRDUDE defines below)
# by default the 'debug' version will be built with the above commands;
# add the target 'release' to the make line for release versions:
#   make release         - build the 'release' version
#   make release clean   - remove all release version built files
#   make release program - download release version of program
#
# stuff you may want to edit:
#
#   PRG         - the name of the program
#   BUILD_ROOT  - the location for generated files
#   MCU_TARGET  - the microprocessor for which we're building
#   SRC         - the list of CPP files to be compiled
#   HEADERS     - the list of H files used by the program
#
#   AVRDUDE_PART        - AVRDUDE's part id for the microprocessor
#   AVRDUDE_PROGRAMMER  - the programmer used for downloading
#   AVRDUDE_PORT        - the port the programmer is plugged into
#       the last 2 can be specified by environment variables

PRG            = M168
BUILD_ROOT     = Build
MCU_TARGET     = atmega168

SRC            = main.cpp Timer.cpp Comm.cpp OLED.cpp
HEADERS        = Util.h Timer.h Comm.h OLED.h

# AVRDUDE definitions; m644 is the ATmega644;
# you can define environment variables to override the programmer and port settings
AVRDUDE_PART       = m168
ifndef AVRDUDE_PROGRAMMER
AVRDUDE_PROGRAMMER = avrispmkii
endif
ifndef AVRDUDE_PORT
AVRDUDE_PORT       = usb
endif
AVRDUDE_FLAGS      = -p $(AVRDUDE_PART) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) 

# fuse bits for mega644
# efuse: 0xfd .... .101 - BOD 2.7V
# hfuse: 0xd1 1101 0001 - no OCD, no JTAG, SPI, no watchdog, EEPROM preserve, boot size, reset vector unprog.
# lfuse: 0xc2 1100 0010 - no ckdiv8, no ckout, SUT for BOD, internal oscillator @ 8 MHz
EFUSE = 0xfd
HFUSE = 0xd1
LFUSE = 0xc2

# math library -- set MATH to include math library when linking;
# this is needed for things like sin and cos (declared in <math.h>)
# MATH = yes

# printf / scanf levels -- default level will be used if nothing set here
# PRINTF_LEVEL = MINIMAL
# PRINTF_LEVEL = FLOAT
# SCANF_LEVEL = MINIMAL
# SCANF_LEVEL = FLOAT

# other useful settings...
# examples:
# DEFS         = -D FOO -D BAR
# LIBS         = libdir/mylib.a libdir/lib2.a
#                (libdir relative to makefile's dir)
DEFS           =
LIBS           =
OPTIMIZE       = -Os

#===========================================================
# The stuff below should generally not need to be modified!
#===========================================================

# build directory and define for debug/release builds
ifneq ( , $(findstring release,$(MAKECMDGOALS)))
	DEFS          += -D RELEASE
	BUILD_DIR      = $(BUILD_ROOT)/Release
	ifeq (release, $(MAKECMDGOALS))		# release is only target; act like "all"
		REL_DEP    = all
	endif
else
	DEFS          += -D DEBUG
	BUILD_DIR      = $(BUILD_ROOT)/Debug
endif

PRG_OBJ        = $(SRC:%.cpp=$(BUILD_DIR)/%.o)
OBJ            = $(PRG_OBJ)
INC            = -I"."

# compile of .c auto with $(CC) -c $(CPPFLAGS) $(CFLAGS)
# compile of .cpp auto with $(CXX) -c $(CPPFLAGS) $(CXXFLAGS)
# (CPPFLAGS are preprocessor flags)
CC             = avr-gcc
CXX            = avr-g++

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
AVRDUDE        = avrdude     # for downloading

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) $(INC) -mmcu=$(MCU_TARGET) $(DEFS)
override CPPFLAGS      = -g -Wall $(OPTIMIZE) $(INC) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(BUILD_DIR)/$(PRG).map

# linking options for gcc:
# -Wl,[options] -- pass [options] to the linker (comma-sep are passed as separate options]
# -u [symbol] -- pretend [symbol] is undefined; to force linking of library modules
# -l[library] -- links with lib[library].a
# Examples:
#   -Wl,-u,vfprintf    passes '-u vprintf' to linker; linker treats vfprintf as undefined
#   -lm                links to library libm.a (math stuff)

# printf/scanf library selection
ifeq ($(PRINTF_LEVEL), MINIMAL)
	override LDFLAGS += -Wl,-u,vfprintf -lprintf_min
endif
ifeq ($(PRINTF_LEVEL), FLOAT)
	override LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
	MATH = yes
endif
ifeq ($(SCANF_LEVEL), MINIMAL)
	override LDFLAGS += -Wl,-u,vfscanf -lscanf_min
endif
ifeq ($(SCANF_LEVEL), FLOAT)
	override LDFLAGS += -Wl,-u,vfscanf -lscanf_flt
	MATH = yes
endif

# math library
ifdef MATH
	override LDFLAGS += -lm
endif

#
#	Build Targets
#

# NOTE: the first target is the default when "make" is called with
# no arguments, so "all" should remain first!

all: elf lst bin
#all: elf lst bin ebin
#all: elf lst bin eeprom_files

release: $(REL_DEP)
	@echo > /dev/null	# needed to avoid "nothing to do" message

clean:
	cd $(BUILD_DIR); rm -f *.o $(PRG).elf *.lst *.map *.bin *.hex *.srec

program: all
	avrdude $(AVRDUDE_FLAGS) -e -U flash:w:$(BUILD_DIR)/$(PRG).bin:r

setfuse:
ifdef EFUSE
	avrdude $(AVRDUDE_FLAGS) -u -U efuse:w:$(EFUSE):m
endif
	avrdude $(AVRDUDE_FLAGS) -u -U hfuse:w:$(HFUSE):m
	avrdude $(AVRDUDE_FLAGS) -u -U lfuse:w:$(LFUSE):m

#
#	Helper targets
#

elf: $(BUILD_DIR)/$(PRG).elf
bin: $(BUILD_DIR)/$(PRG).bin	# binary file (to download to robot)
lst: $(BUILD_DIR)/$(PRG).lst	# code/asm listing

eeprom_files: ehex ebin esrec
ehex:  $(BUILD_DIR)/$(PRG)_eeprom.hex
ebin:  $(BUILD_DIR)/$(PRG)_eeprom.bin
esrec: $(BUILD_DIR)/$(PRG)_eeprom.srec

# our list of "phony" targets -- this prevents thinking a rule is satisfied by a file...
.PHONY: all release clean program setfuse elf bin lst eeprom_files ehex ebin esrec

#
#	Dependencies
#

# rebuild the obj files if any .h (or the makefile) changes
$(PRG_OBJ): $(HEADERS) makefile

#
#	Build Rules
#

# note: this is the default compile command, but is needed here because of the build path...
$(BUILD_DIR)/%.o: %.cpp
	$(shell mkdir $(BUILD_ROOT) 2> /dev/null; mkdir $(BUILD_DIR) 2>/dev/null)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(BUILD_DIR)/$(PRG).elf: $(OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# for .lst for the entire project
$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.elf
	$(OBJDUMP) -h -S $< > $@

# for .lst from a single object file
$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.o
	$(OBJDUMP) -h -S $< > $@

#
#	EEPROM build rules
#

$(BUILD_DIR)/%_eeprom.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

$(BUILD_DIR)/%_eeprom.srec: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

$(BUILD_DIR)/%_eeprom.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@
