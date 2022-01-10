#! /bin/bash -x

# meant to be run from the BPS directory

ulimit -r unlimited

./bps-simulator.out

diff ./BSP/Simulator/DataGeneration/Data/CAN.csv ./Continuous_Integration/CANbus/expected.csv

exit $?

