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
    SPI = "SPI.csv",
    Timer = "Timer.csv",
    WDTimer = "WDTimer.csv",
    Strobelight = "Strobe.csv"

    # Add more files if needed here.
)

num_batt_modules_series = 31             # Number of battery modules in series
num_batt_cells_parallel_per_module = 14  # Number of battery cells in parallel per module
batt_cell_capacity_mah = 2950            # Samsung's Lithium Ion Battery capacity per cell

'''
Total battery pack is calculated by the number of battery cells in parallel per module.
Capacity does not increase as you put more cells in series.
'''
total_batt_pack_capacity_mah = num_batt_cells_parallel_per_module * batt_cell_capacity_mah
