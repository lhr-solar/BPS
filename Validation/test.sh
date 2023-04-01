ulimit -r unlimited
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/$1
# IF DEBUGGING, USE THE COMMAND BELOW!!
# gdb --args ./bps-simulator.out BSP/Simulator/Data/ManualTests/$1
