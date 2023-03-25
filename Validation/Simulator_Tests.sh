sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ critical_overcurrent.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ critical_overtemp_discharging.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ critical_overvolt.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ critical_undervolt.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ overcurrent.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ overtemp_charging.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ overtemp_discharging.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ overvolt.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ undervolt.json
sudo python3 Validation/verify_test.py BSP/Simulator/Data/ManualTests/ killswitch.json
echo -e "End of Manual Tests \n\n"

echo "Starting automated tests"
rm -rf ../BSP/Simulator/Simulator-Out/
./Automatic_Tester.out 9