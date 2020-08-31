TARGET = bps-simulator.out

TOP_DIR = ../..
# RTOS path
RTOS_DIR = $(TOP_DIR)/RTOS/uCOS-III-Simulator

CC = gcc

# since current path is in the BSP folder, go to the top level with $(TOP_DIR)
SRC = \
$(wildcard $(TOP_DIR)/BSP/Simulator/Src/*.c) \
$(wildcard $(TOP_DIR)/Drivers/Src/*.c) \
$(wildcard $(TOP_DIR)/Config/Src/*.c)	\
$(wildcard $(RTOS_DIR)/uCOS-III/Source/*.c)	\
$(wildcard $(RTOS_DIR)/uCOS-III/Ports/POSIX/GNU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-CPU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-CPU/Posix/GNU/*.c)	\
$(wildcard $(RTOS_DIR)/uC-LIB/*.c)

ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
# This line adds everything in Apps/Src/*.c except for main.c
SRC += \
$(filter-out $(TOP_DIR)/Apps/Src/main.c, $(wildcard $(TOP_DIR)/Apps/Src/*.c))	\
$(wildcard $(TOP_DIR)/Tests/$(TEST_FILE))
else
SRC += $(wildcard $(TOP_DIR)/Apps/Src/*.c)
endif

INC_DIR = \
-I$(TOP_DIR)/Apps/Inc \
-I$(TOP_DIR)/BSP/Inc \
-I$(TOP_DIR)/Drivers/Inc \
-I$(TOP_DIR)/Config/Inc	\
-I$(RTOS_DIR)/uCOS-III/Source	\
-I$(RTOS_DIR)/uCOS-III/Ports/POSIX/GNU	\
-I$(RTOS_DIR)/uC-CPU	\
-I$(RTOS_DIR)/uC-CPU/Posix/GNU	\
-I$(RTOS_DIR)/uC-LIB

LIB = -lm -lpthread

FLAGS = -Wall -g -std=c11 $(INC_DIR) -DSIMULATION -D_GNU_SOURCE

BUILD_DIR = $(TOP_DIR)/Objects
OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))
vpath %.c $(sort $(dir $(SRC)))

all: $(TARGET_PATH)/$(TARGET)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) -c -o $@ $< $(FLAGS)

$(TARGET_PATH)/$(TARGET): $(OBJ) $(BUILD_DIR)
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIB)

$(BUILD_DIR):
	mkdir $@

.PHONY: clean

clean:
	rm -fR $(BUILD_DIR)
