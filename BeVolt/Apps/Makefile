TARGET = bps-simulator

CC = gcc

DEP = $(wildcard *.h)
SRC = $(wildcard *.c)

SRC_DIR = Src

INC_DIR = \
-IInc \
-I../BSP/Inc \
-I../Drivers/Inc \
-I../Config/Inc

FLAGS = -W -g -std=c11 $(INC_DIR)

BUILD_DIR = Objects
OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))

#all: $(BUILD_DIR)/$(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEP)
	$(CC) -c -o $@ $< $(FLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

$(BUILD_DIR):
	mkdir $@

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/*.o