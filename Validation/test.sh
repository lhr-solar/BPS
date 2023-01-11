#! /bin/bash -x

# meant to be run from the BPS directory by a GitHub workflow with sudo

ulimit -r unlimited

gdb --args ./bps-simulator.out BSP/Simulator/Data/$1.json
python3 Validation/verify_test.py $1

exit $?