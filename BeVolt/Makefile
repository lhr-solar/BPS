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

all:
	@echo "${RED}Not enough arguments.${NC}"
	@echo "	make ${CYAN}<BSP type>${NC}"
	@echo "Options:"
	@echo "	${CYAN}simulator${NC}"
	@echo "	${CYAN}stm32f413${NC}"

simulator:
	$(MAKE) -C BSP -C Simulator

stm32f413:
	$(MAKE) -C BSP -C STM32F413

clean:
	rm -fR Objects
	rm *.out
