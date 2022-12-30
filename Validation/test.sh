#! /bin/bash -x

# meant to be run from the BPS directory by a GitHub workflow with sudo

ulimit -r unlimited

./bps-simulator.out $1

exit $?