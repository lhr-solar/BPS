#! /bin/bash 

BPS_DIR=../..

(cd ${BPS_DIR}; ./bps-simulator.out)

diff ${BPS_DIR}/BSP/Simulator/DataGeneration/Data/CAN.csv expected.csv

exit $?

