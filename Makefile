TARGET = min
DEBUG = 1
OPT = -Og
BUILD_DIR = build

C_SOURCES = $(wildcard src/*.c)
ASM_SOURCES = $(wildcard src/*.s)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(dir $(C_SOURCES))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(dir $(ASM_SOURCES))

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

ASFLAGS = $(MCU) -Wall -data-sections -ffunction-sections

CFLAGS += $(MCU) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
	CFLAGS += -g -gdwarf-2
endif

LDSCRIPT = linker.ld
LIBS = -lc -lm -lnosys
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# RULES

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(BIN) $< $@

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: flash server debug disasm

flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f /usr/share/openocd/scripts/chip/st/stm32/stm32.tcl \
		-f /usr/share/openocd/scripts/interface/stlink.cfg \
		-f /usr/share/openocd/scripts/target/stm32l4x.cfg \
		-c "program $(BUILD_DIR)/$(TARGET).bin verify reset exit 0x08000000"

server:
	openocd -f /usr/share/openocd/scripts/chip/st/stm32/stm32.tcl \
		-f /usr/share/openocd/scripts/interface/stlink.cfg \
		-f /usr/share/openocd/scripts/target/stm32l4x.cfg \

debug: $(BUILD_DIR)/$(TARGET).elf
	gdb-multiarch -x setup.gdb $(BUILD_DIR)/$(TARGET).elf 

disasm:
	arm-none-eabi-objdump -D $(BUILD_DIR)/$(TARGET).elf
