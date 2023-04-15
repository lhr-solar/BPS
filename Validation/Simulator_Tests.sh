ulimit -r unlimited

#exit bash script upon any commmand failing
yell() { echo "$0: $*" >&2; } #screams out the command that failed
die() { yell "$*"; exit -1; } #kills the bash script and exits with an error code 
try() { "$@" || die "$* has failed"; } #if a command fails, call die on that function

#manual tests
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

#automated tests
echo "Starting automated tests"
rm -rf ../BSP/Simulator/Simulator-Out/
try ./Automatic_Tester.out 9