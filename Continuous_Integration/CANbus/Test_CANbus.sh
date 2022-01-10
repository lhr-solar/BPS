#! /bin/bash -x

# meant to be run from the BPS directory

ulimit -r unlimited

./bps-simulator.out

ls BSP/Simulator/DataGeneration/Data

diff ./BSP/Simulator/DataGeneration/Data/CAN.csv ./Continuous_Integration/CANbus/expected.csv

exit $?

