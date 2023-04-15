ulimit -r unlimited

set -e # to exit bash script upon any commmand failing

python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overcurrent.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overtemp_discharging.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_overvolt.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/critical_undervolt.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overcurrent.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overtemp_charging.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overtemp_discharging.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/overvolt.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/undervolt.json
python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/killswitch.json
echo -e "End of Manual Tests \n\n"

echo "Starting automated tests"
rm -rf ../BSP/Simulator/Simulator-Out/
./Automatic_Tester.out 9