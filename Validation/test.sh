#! /bin/bash -x

# meant to be run from the BPS directory by a GitHub workflow with sudo

ulimit -r unlimited

./bps-simulator.out BSP/Simulator/Data/$1.json
python3 Validation/verify_test.py $1 > $1.result
diff $1.result Validation/Test_Results/$1.result

exit $?