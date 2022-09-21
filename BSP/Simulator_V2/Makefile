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
$(wildcard $(TARGET_PATH)/Tasks/Src/*.c)	\
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
-I$(TARGET_PATH)/Tasks/Inc	\
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

FLAGS = -Wall -Werror -g -std=c11 $(INC_DIR) -DSIMULATION -D_GNU_SOURCE

######################################
# Defines
######################################
ifneq ($(INPUT), none)
DEF = -D_XOPEN_SOURCE=700 -DSIMULATION -D$(OS) -DSIMULATOR_JSON_PATH=$(INPUT)
else
DEF = "Error: you should pass in an input file with INPUT="<path to file>"
endif


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
