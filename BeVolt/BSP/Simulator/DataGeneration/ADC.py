import csv

"""
This module will generate ADC data to simulate Current readings
"""

# path/name of file
file = "Data/ADC.csv"

def amps_to_adc(current, sensor):
    if sensor == 'h':
        # Map [-50A, 50A] -> [-4V, 4V]
        hall_effect = current * 4/50
    elif sensor == 'l':
        # Map [-100A, 100A] -> [-4V, 4V]
        hall_effect = current * 4/100
    # Map [-4V, 4V] -> [0V, 3.3V]
    opamp_gain = 3
    opamp_offset = 4.096
    adc_voltage = (hall_effect + opamp_offset) / opamp_gain * 1000  # mV

    # Convert mV to 12-bit value
    adc_data = (int(adc_voltage) << 12) / 3300
    return int(adc_data)

def main():
    print(amps_to_adc(50, 'l'))

if __name__ == '__main__':
    main()
