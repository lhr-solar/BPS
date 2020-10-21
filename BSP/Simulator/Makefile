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
# Source
######################################
SRC = $(wildcard Src/*.c)	# BSP code

SRC += \
$(wildcard $(TARGET_PATH)/Drivers/Src/*.c)	# Driver code

ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
SRC += \
$(filter-out $(TARGET_PATH)/Apps/Src/main.c, $(wildcard $(TARGET_PATH)/Apps/Src/*.c)) \
$(wildcard $(TARGET_PATH)/Tests/$(TEST_FILE))		# Apps (not main)
else
SRC += $(wildcard $(TARGET_PATH)/Apps/Src/*.c)		# Apps (with  main)
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
-I$(TARGET_PATH)/Apps/Inc \
-I$(TARGET_PATH)/Drivers/Inc \
-I$(TARGET_PATH)/BSP/Inc \
-I$(TARGET_PATH)/Config/Inc \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-CPU/Posix/GNU \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uC-LIB \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Source \
-I$(TARGET_PATH)/RTOS/uCOS-III-Simulator/uCOS-III/Ports/POSIX/GNU \

LIB = -lm -lpthread		# Standard libraries used

FLAGS = -Wall -g -std=c11 $(INC_DIR)

######################################
# Defines
######################################
DEF = -D_XOPEN_SOURCE=600 -DSIMULATION -D$(OS)

######################################
# Build
######################################
all: $(TARGET_PATH)/$(TARGET)

$(TARGET_PATH)/$(TARGET): $(OBJ) $(BUILD_DIR)
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIB)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) -c -o $@ $< $(FLAGS) $(DEF)

$(BUILD_DIR):
	mkdir $@
