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

DEFINES = none
export DEFINES

debug = 0
export debug

TEST = none
export TEST

ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
endif

OS = RTOS
export OS

BUILD_HELPER_SRC := BSP/STM32F413/BuildHelper/error_verbose.c
BUILD_HELPER := gcc -O -o error_verbose.log $(BUILD_HELPER_SRC) -lncurses

.PHONY: stm32f413
stm32f413:
ifneq ($(TEST), none)
	@echo -e "Making STM32 build for file ${PURPLE}'${TEST_FILE}'${NC}"
else
	@echo -e "Making STM32 build with ${RED}NO${NC}test."
endif
	@$(BUILD_HELPER)
	$(MAKE) -C BSP -C STM32F413 -j || ./error_verbose.log

.PHONY: simulator
simulator:
	@$(BUILD_HELPER)
	$(MAKE) -C BSP -C Simulator -j || ./error_verbose.log

flash:
	$(MAKE) -C BSP -C STM32F413 flash

.PHONY: help
help:
	@echo -e "Format: ${ORANGE}make ${BLUE}<BSP type>${NC}${ORANGE}TEST=${PURPLE}<Test type>${NC}"
	@echo -e "BSP types (required):"
	@echo -e "	${BLUE}simulator${NC}"
	@echo -e "	${BLUE}stm32f413${NC}"
	@echo -e ""
	@echo -e "Test types (optional):"
	@echo -e "	Set TEST only if you want to build a test."
	@echo -e "	Otherwise, don't include TEST in the command."
	@echo -e "	To build a test, replace ${PURPLE}<Test type>${NC} with the name of the file"
	@echo -e "	excluding the file type (.c) e.g. say you want to test Voltage.c, call"
	@echo -e "		${ORANGE}make ${BLUE}stm32f413 ${ORANGE}TEST=${PURPLE}Voltage${NC}"
	@echo -e ""
	@echo -e "When building for the board, you can specify custom define values for config.h."
	@echo -e "See Config/Inc/config.h for all values."
	@echo -e "Specify with the following ${PURPLE}format${NC}(quotes ARE needed):"
	@echo -e "		${ORANGE}make ${BLUE}DEFINES=${GREEN}\"VARIABLE=VALUE VARIABLE=VALUE\"\n"

.PHONY: clean
clean:
	rm -fR Objects
	rm -f *.log
	rm -f *.out
	