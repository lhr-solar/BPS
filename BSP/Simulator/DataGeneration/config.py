"""
Configuration constants.
"""

directory_path = "BSP/Simulator/DataGeneration/Data/"

files = dict(
    ADC = "ADC.csv",
    CAN = "CAN.csv",
    Contactor = "Contactor.csv",
    I2C = "I2C.csv",
    Lights = "Lights.csv",
    PLL = "PLL.csv",
    SPIR = "SPIR.csv",
    SPIW = "SPIW.csv",
    Timer = "Timer.csv",
    WDTimer = "WDTimer.csv",
    Strobelight = "Strobe.csv",
    Fans = "Fans.csv"

    # Add more files if needed here.
)

num_batt_modules_per_ltc6811 = 8         # With last LTC6811 being exception
num_temp_sensors_per_batt_module = 2     # With last LTC6811 being exception

num_batt_modules_series = 31             # Number of battery modules in series
num_batt_cells_parallel_per_module = 14  # Number of battery cells in parallel per module
batt_cell_capacity_mah = 2950            # Samsung's Lithium Ion Battery capacity per cell

'''
Total battery pack is calculated by the number of battery cells in parallel per module.
Capacity does not increase as you put more cells in series.
'''
total_batt_pack_capacity_mah = num_batt_cells_parallel_per_module * batt_cell_capacity_mah



rand_volt_range = {
    'LOW' : [25000, 27000],
    'NOM' : [27000, 40000],
    'HIGH' : [40000, 42000]
}

rand_temp_dischg_range = {
    'LOW' : [10000, 30000],
    'NOM' : [30000, 72000],
    'HIGH' : [73000, 90000]
}

rand_temp_chg_range = {
    'LOW' : [10000, 25000],
    'NOM' : [25000, 48000],
    'HIGH' : [48000, 60000]
}
