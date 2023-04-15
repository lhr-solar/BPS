ulimit -r unlimited

#set -e # to exit bash script upon any commmand failing

yell() { echo "$0: $*" >&2; }
die() { yell "$*"; exit 111; }
try() { "$@" || die "$* has failed"; }

try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overcurrent.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overtemp_discharging.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overvolt.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_undervolt.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overcurrent.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overtemp_charging.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overtemp_discharging.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overvolt.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/undervolt.json
try python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/killswitch.json
echo -e "End of Manual Tests \n\n"

echo "Starting automated tests"
rm -rf ../BSP/Simulator/Simulator-Out/
try ./Automatic_Tester.out 9