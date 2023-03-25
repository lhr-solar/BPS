sudo bash Validation/test.sh critical_overcurrent.json
sudo bash Validation/test.sh critical_overtemp_discharging.json
sudo bash Validation/test.sh critical_overvolt.json
sudo bash Validation/test.sh critical_undervolt.json
sudo bash Validation/test.sh overcurrent.json
sudo bash Validation/test.sh overtemp_charging.json
sudo bash Validation/test.sh overtemp_discharging.json
sudo bash Validation/test.sh overvolt.json
sudo bash Validation/test.sh undervolt.json
sudo bash Validation/test.sh killswitch.json
echo -e "End of Manual Tests \n\n"
sudo bash Validation/automatedTest.sh