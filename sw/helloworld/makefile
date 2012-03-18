###############################################################################
## MIPS Makefile
###############################################################################

# Tools
MIPS_DIR    = D:/mips-elf/bin
MIPS_PREFIX = mips-elf
GCC_MIPS    = $(MIPS_DIR)/$(MIPS_PREFIX)-gcc $(CFLAGS)
AS_MIPS     = $(MIPS_DIR)/$(MIPS_PREFIX)-as
LD_MIPS     = $(MIPS_DIR)/$(MIPS_PREFIX)-ld
DUMP_MIPS   = $(MIPS_DIR)/$(MIPS_PREFIX)-objdump
OBJCOPY     = $(MIPS_DIR)/$(MIPS_PREFIX)-objcopy

# Target
TARGET		= test

# Options
LDSCRIPT    = linker_script
#CFLAGS	    = -G 0 -Os -g -Wall -DMIPS -march=mips1 -mpatfree -mnohwdiv -mnohwmult -nostartfiles -T$(LDSCRIPT)
CFLAGS	    = -G 0 -O2 -g -Wall -march=mips1 -mpatfree -mnohwdiv -mnohwmult -nostartfiles -nodefaultlibs -nostdlib -lgcc -L ./ -lstd -T$(LDSCRIPT)
ASFLAGS     = -Wa
LDFLAGS     = 

# Source Files
OBJ = 
include sources.inc

CFLAGS += -I. $(INCLUDE_DIRS)

###############################################################################
# Rules
###############################################################################
all: $(TARGET).elf lst bin 
	
clean:
	-del *.o *.obj *.map *.lst *.hex *.txt *.axf *.elf

%.o : %.s
	$(GCC_MIPS) -c $(ASFLAGS) $< -o $@

%.o : %.c
	$(GCC_MIPS) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ) $(LDSCRIPT) makefile
	$(GCC_MIPS) $(LDFLAGS) $(LIBS) $(OBJ) -o $@
	
lst:  $(TARGET).lst

%.lst: $(TARGET).elf
	#$(DUMP_MIPS) -h -d -S $< > $@

bin: $(TARGET).bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	