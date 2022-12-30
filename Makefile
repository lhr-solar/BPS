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

TEST = none
export TEST

OS = RTOS
export OS

.PHONY: stm32f413
stm32f413:
	$(MAKE) -C BSP -C STM32F413 -j

.PHONY: simulator
simulator:
	$(MAKE) -C BSP -C Simulator -j

.PHONY: simulator_legacy
simulator_legacy:
	$(MAKE) -C BSP -C Simulator_Deprecated -j

.PHONY: flash
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

.PHONY: clean
clean:
	rm -fR Objects
	rm -f *.log
	rm -f *.out
	rm -f BSP/Simulator/Data/*-*.json
