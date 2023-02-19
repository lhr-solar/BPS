######################################
# Target
######################################
TARGET = bps-simulator.out
TARGET_PATH = ../..

######################################
# GCC
######################################
CC = gcc

######################################
# Cleanliness
######################################
READABLE = true
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

######################################
# Source
######################################
SRC = $(wildcard Src/*.c)	# BSP code

SRC1 = \
$(wildcard $(TARGET_PATH)/Drivers/Src/*.c)	# Driver code
# don't compile LTC drivers
SRC2 = $(subst $(TARGET_PATH)/Drivers/Src/LTC6811.c,,$(SRC1))
SRC += $(subst $(TARGET_PATH)/Drivers/Src/LTC681x.c,,$(SRC2))

ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
SRC += \
$(wildcard $(TARGET_PATH)/Tasks/Src/*.c)	\
$(filter-out $(TARGET_PATH)/Apps/Src/main.c, $(wildcard $(TARGET_PATH)/Apps/Src/*.c)) \
$(wildcard $(TARGET_PATH)/Tests/$(TEST_FILE))		# Apps (not main)
else
SRC += \
$(wildcard $(TARGET_PATH)/Tasks/Src/*.c)	\
$(wildcard $(TARGET_PATH)/Apps/Src/*.c)		# Apps (with  main)
endif

SRC += \
$(wildcard $(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU/*.c) \
$(wildcard $(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU/Posix/GNU/*.c) \
$(wildcard $(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-LIB/*.c) \
$(wildcard $(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Source/*.c) \
$(wildcard $(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Ports/POSIX/GNU/*.c) # RTOS

######################################
# Objects
######################################
BUILD_DIR = $(TARGET_PATH)/Objects
OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))	# Create object file list
vpath %.c $(sort $(dir $(SRC)))		# In case files aren't found

######################################
# Flags
######################################

INC_DIR = \
-I$(TARGET_PATH)/Tasks/Inc  \
-I$(TARGET_PATH)/Apps/Inc \
-I$(TARGET_PATH)/Drivers/Inc \
-I$(TARGET_PATH)/BSP/Inc \
-I$(TARGET_PATH)/Config/Inc \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU/Posix/GNU \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-LIB \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Source \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Ports/POSIX/GNU \

LIB = -lm -lpthread     # Standard libraries used

FLAGS = -Wall -Werror -g -std=c11 $(INC_DIR) -DSIMULATION -D_GNU_SOURCE

######################################
# Defines
######################################
DEF = -D_XOPEN_SOURCE=700 -DSIMULATION -D$(OS)


######################################
# Build
######################################
all: $(TARGET_PATH)/$(TARGET)

$(TARGET_PATH)/$(TARGET): $(OBJ) $(BUILD_DIR) linking
ifeq ($(READABLE), true)
	@$(CC) -o $@ $(OBJ) $(FLAGS) $(LIB)
else
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIB)
endif

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR) c_compile_msg
ifeq ($(READABLE), true)
	@$(CC) -c -o $@ $< $(FLAGS) $(DEF)
else
	$(CC) -c -o $@ $< $(FLAGS) $(DEF)
endif

.INTERMEDIATE: c_compile_msg
.INTERMEDIATE: linking

c_compile_msg:
	@echo "${GREEN}Compiling C files...${NC}"
linking:
	@echo "${BLUE}Linking and building bps-simulator executable...${NC}"

$(BUILD_DIR):
	mkdir $@
