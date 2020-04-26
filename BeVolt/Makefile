TARGET = bps-simulator

CC = gcc

SRC = \
$(wildcard Apps/Src/*.c) \
$(wildcard BSP/Simulator/Src/*.c) \
$(wildcard Drivers/Src/*.c) \
$(wildcard Config/Src/*.c)

INC_DIR = \
-IApps/Inc \
-IBSP/Inc \
-IDrivers/Inc \
-IConfig/Inc

FLAGS = -W -g -std=c11 $(INC_DIR)

BUILD_DIR = Objects
OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))
vpath %.c $(sort $(dir $(SRC)))

$(BUILD_DIR)/$(TARGET): $(OBJ) $(BUILD_DIR)
	$(CC) -o $@ $< $(FLAGS)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) -c -o $@ $< $(FLAGS)

$(BUILD_DIR):
	mkdir $@

.PHONY: clean

clean:
	rm -fR $(BUILD_DIR)
