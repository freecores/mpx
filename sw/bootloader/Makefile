###############################################################################
## MIPS Makefile
###############################################################################

# Tools
MIPS_PREFIX = mips-elf
GCC_MIPS    = $(MIPS_PREFIX)-gcc $(CFLAGS)
AS_MIPS     = $(MIPS_PREFIX)-as
LD_MIPS     = $(MIPS_PREFIX)-ld
DUMP_MIPS   = $(MIPS_PREFIX)-objdump
OBJCOPY     = $(MIPS_PREFIX)-objcopy

# Target
TARGET		= bootloader

# Options
LDSCRIPT    = linker_script
CFLAGS	    = -G 0 -O2 -g -Wall 
CFLAGS	   += -march=mips1 -mpatfree -mnohwdiv -mnohwmult
CFLAGS	   += -nostartfiles -nodefaultlibs -nostdlib -lgcc 
CFLAGS	   += -L ./ -L ../common/ -lstd -T$(LDSCRIPT)
ASFLAGS     = -Wa
LDFLAGS     = 

OBJ += boot.o
OBJ += exception.o
OBJ += main.o
OBJ += boot_serial.o
OBJ += xmodem.o
OBJ += serial.o
OBJ += timer.o

CFLAGS += -I. -I./../common


###############################################################################
# Rules
###############################################################################
all: $(TARGET).elf lst bin 
	
clean:
	-rm *.o *.map *.lst *.hex *.txt *.elf

%.o : %.s
	$(GCC_MIPS) -c $(ASFLAGS) $< -o $@

%.o : %.c
	$(GCC_MIPS) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ) $(LDSCRIPT) makefile
	$(GCC_MIPS) $(LDFLAGS) $(LIBS) $(OBJ) -o $@
	
lst:  $(TARGET).lst

%.lst: $(TARGET).elf
	$(DUMP_MIPS) -h -d -S $< > $@

bin: $(TARGET).bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	
