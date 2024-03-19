######################################
# target
######################################
TARGET = bps-leader

# sets debugging mode on or off based on BPS makefile
DEBUG = $(debug)

READABLE = true

######################################
# building variables
######################################
# optimization
# change to -Og if you want the compiler to generate assembly that is very close
# to your source code. Change to -O3 for best performance
ifeq ($(debug), 0)
OPT = -Og
else
OPT = -O3
endif

# COLORS
RED=\033[0;31m
GREEN=\033[0;32m
ORANGE=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
LIGHTGRAY=\033[0;37m
DARKGRAY=\033[1;30m
YELLOW=\033[0;33m
NC=\033[0m # No Color

#######################################
# paths
#######################################
# Top Level
TOP_DIR = ../..
# Build path
BUILD_DIR = $(TOP_DIR)/Objects
# RTOS path
RTOS_DIR = $(TOP_DIR)/RTOS/uCOS-III-STM32F4

######################################
# source
######################################

# C sources
# since current path is in the BSP folder, go to the top level with $(TOP_DIR)/
# Note: Config/Src/*.c needs to be last in this list, so that we see compile line prints at the END of compilation.

C_SOURCES =  \
$(wildcard $(TOP_DIR)/Tasks/Src/*.c)	\
$(wildcard $(TOP_DIR)/Drivers/Src/*.c)	\
$(wildcard $(TOP_DIR)/BSP/STM32F413/Src/*.c)	\
$(wildcard $(TOP_DIR)/CMSIS/DSP_Lib/Source/*.c)	\
$(wildcard $(TOP_DIR)/BSP/STM32F413/STM32F4xx_StdPeriph_Driver/Src/*.c)	\
$(wildcard $(RTOS_DIR)/uCOS-III/Source/*.c)	\
$(wildcard $(RTOS_DIR)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-CPU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-CPU/ARM-Cortex-M4/GNU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-LIB/*.c)	\
$(wildcard $(RTOS_DIR)/uC-LIB/Ports/ARM-Cortex-M4/GNU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-Serial/Driver/ST/*.c)	\
$(wildcard $(RTOS_DIR)/uC-Serial/Line/*.c)	\
$(wildcard $(RTOS_DIR)/uC-Serial/OS/*.c)	\
$(wildcard $(RTOS_DIR)/uC-Serial/OS/uCOS-III/*.c)	\
$(wildcard $(RTOS_DIR)/uC-Serial/Source/*.c)	\
$(wildcard $(TOP_DIR)/Config/Src/*.c) 

MESSAGES = $(wildcard $(TOP_DIR)/Config/Src/*.c)

ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
# This line adds everything in Apps/Src/*.c except for main.c
C_SOURCES += \
$(filter-out $(TOP_DIR)/Apps/Src/main.c, $(wildcard $(TOP_DIR)/Apps/Src/*.c))	\
$(wildcard $(TOP_DIR)/Tests/$(TEST_FILE))
else
C_SOURCES += $(wildcard $(TOP_DIR)/Apps/Src/*.c)
endif

# ASM sources
ASM_SOURCES =  \
$(TOP_DIR)/BSP/STM32F413/Src/startup_stm32f413xx.s	\
$(wildcard $(RTOS_DIR)/uC-CPU/ARM-Cortex-M4/GNU/*.s)	\
$(wildcard $(RTOS_DIR)/uC-LIB/Ports/ARM-Cortex-M4/GNU/*.s)	\
$(wildcard $(RTOS_DIR)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/*.s)	


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

SF = st-flash
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# float-abi
FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32F413_423xx	\
-DUSE_STDPERIPH_DRIVER	\
-D$(OS)

# AS includes
AS_INCLUDES = 

# C includes
# since current path is in the BSP folder, go to the top level with $(TOP_DIR)/
C_INCLUDES =  \
-I$(TOP_DIR)/Tasks/Inc	\
-I$(TOP_DIR)/Apps/Inc	\
-I$(TOP_DIR)/Drivers/Inc	\
-I$(TOP_DIR)/Config/Inc	\
-I$(TOP_DIR)/BSP/Inc	\
-I$(TOP_DIR)/CMSIS/Device/ST/STM32F4xx/Include	\
-I$(TOP_DIR)/CMSIS/Include	\
-I$(TOP_DIR)/BSP/STM32F413/STM32F4xx_StdPeriph_Driver/Inc	\
-I$(RTOS_DIR)/uCOS-III/Source	\
-I$(RTOS_DIR)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU	\
-I$(RTOS_DIR)/uC-CPU	\
-I$(RTOS_DIR)/uC-CPU/ARM-Cortex-M4/GNU	\
-I$(RTOS_DIR)/uC-LIB	\
-I$(RTOS_DIR)/uC-LIB/Ports/ARM-Cortex-M4/GNU	\
-I$(RTOS_DIR)/uC-Serial/Driver/ST	\
-I$(RTOS_DIR)/uC-Serial/Line	\
-I$(RTOS_DIR)/uC-Serial/OS	\
-I$(RTOS_DIR)/uC-Serial/OS/uCOS-III	\
-I$(RTOS_DIR)/uC-Serial/Source	


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections 
CFLAGS_SPECIAL = $(CFLAGS) -ftrack-macro-expansion=0 -fno-diagnostics-show-caret

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2
endif

# custom user defines here
ifneq ($(DEFINES), none)
# need to split defines
PREFIXED = $(addprefix -D,$(DEFINES))
SPLIT = $(subst /, -D, $(PREFIXED))

CFLAGS += $(SPLIT)
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ./GCC/STM32F413RHTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(MESSAGES:.c=.o))) # messages

# The ifneq... @... endif modifies the make console output to not print every file name,
# assuming make is run with READABLE= some nonzero number

# explicit rule for config messages, to hide macro expansion only for this file
# other macro expansions we want for debugging / errors
$(BUILD_DIR)/config_msgs.o: config_msgs.c Makefile | $(BUILD_DIR)
	@$(CC) -c $(CFLAGS_SPECIAL) $(TOP_DIR)/Config/Src/config_msgs.c -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) c_compile_msg
ifeq ($(READABLE), true) 
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
else
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
endif

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR) asm_compile_msg
ifeq ($(READABLE), true)
	@$(AS) -c $(CFLAGS) $< -o $@
else
	$(AS) -c $(CFLAGS) $< -o $@
endif

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile build_msg
ifeq ($(READABLE), true)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@
else
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
endif

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

.INTERMEDIATE: c_compile_msg
.INTERMEDIATE: asm_compile_msg
.INTERMEDIATE: build_msg

c_compile_msg:
	@echo "${GREEN}Compiling C files...${NC}"

asm_compile_msg:
	@echo "${GREEN}Compiling ASM files...${NC}"

build_msg:
	@echo "${CYAN}Generating BPS executable...${NC}"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
flash:
	$(SF) write $(BUILD_DIR)/$(TARGET).bin 0x8000000

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
