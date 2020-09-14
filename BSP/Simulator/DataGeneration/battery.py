"""
Battery class to simulate the real Li-ion battery cells
used in BeVolt

All specifications taken from the specification sheets
on the team Google Drive under the Battery folder
"""
import config
import random

# A battery consists of 31 Modules in series
class Battery:
    def __init__(self, current, capacity, charge=None, state='discharging', mode='normal'):
        self.state = state
        self.mode = mode
        self.num_modules = 31
        self.num_temperature_sensors = 62
        self.current = current
        self.capacity = capacity
        self.charge = charge if charge is not None else capacity
        self.modules = self.create_modules()
        self.voltage = self.calc_voltage()
        self.temperature = self.calc_avg_temperature()
    

    def __str__(self):
        return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rTemperature: {self.calc_avg_temperature()} °C\n\rCharge: {self.charge} mAh\n\r"


    def create_modules(self):
        module_list = []
        for i in range(self.num_modules):
            module_list.append(self.Module(self.current, self.capacity/self.num_modules, self.charge/self.num_modules, self.state, self.mode))
        return module_list


    def update(self):
        for module in self.modules:
            module.update()
        self.charge = self.calc_charge()
        self.voltage = self.calc_voltage()


    def calc_charge(self):
        return sum([module.charge for module in self.modules])


    def calc_voltage(self):
        return sum([module.voltage for module in self.modules])

    def calc_avg_temperature(self):
        avg = 0
        for module in self.modules:
            for sensor in module.temperatures_cel:
                avg += int(sensor)
        return avg / self.num_temperature_sensors


    # A module consists of 14 Cells in parallel
    class Module:
        def __init__(self, current, capacity, charge=None, state='discharging', mode='normal'):
            self.state = state
            self.mode = mode
            self.num_cells = 14
            self.current = current
            self.capacity = capacity
            self.charge = charge if charge is not None else capacity
            self.cells = self.create_cells()
            self.voltage = self.calc_voltage()
            self.temperatures_cel = self.calc_random_temperatures()
            self.connected = True
        

        def __str__(self):
            return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rTemperature: {self.calc_avg_temperature()} °C\n\rCharge: {self.charge} mAh\n\r"


        def create_cells(self):
            cell_list = []
            for i in range(self.num_cells):
                cell_list.append(self.Cell(self.current/self.num_cells, self.capacity/self.num_cells, self.charge/self.num_cells, self.state, self.mode))
            return cell_list
        

        def update(self):
            for cell in self.cells:
                cell.update()
            self.charge = self.calc_charge()
            self.voltage = self.calc_voltage()
            self.temperatures_cel = self.calc_random_temperatures()


        def calc_charge(self):
            return sum([cell.charge for cell in self.cells])


        def calc_voltage(self):
            return sum([cell.voltage for cell in self.cells]) / self.num_cells

        def calc_random_temperatures(self):
            celsius = []
            for i in range(2):
                '''
                Inputs of gauss functions are mean and std deviation.
                Mean is calculated from range as specified in comments.
                '''
                if self.state == 'charging':
                    if self.mode == 'low':
                        celsius.append(random.gauss(17.5, 2.25))    # range: [10, 25]
                    elif self.mode == 'normal':
                        celsius.append(random.gauss(36.5, 3.5))     # range: [25, 48]
                    elif self.mode == 'high':
                        celsius.append(random.gauss(54, 1.75))      # range: [48, 60]

                elif self.state == 'discharging':
                    if self.mode == 'low':
                        celsius.append(random.gauss(20, 3.25))      # range: [10, 30]
                    elif self.mode == 'normal':
                        celsius.append(random.gauss(51, 6.75))      # range: [30, 72]
                    elif self.mode == 'high':
                        celsius.append(random.gauss(81, 2.75))    # range: [72, 90]
            return celsius

        def calc_avg_temperature(self):
            avg = 0
            for sensor in self.temperatures_cel:
                avg += int(sensor)
            return avg / len(self.temperatures_cel)

        class Cell:
            def __init__(self, current, capacity, charge=None, state='discharging', mode='normal'):
                # Operation
                self.state = state
                self.mode = mode                
                # Voltage values (V)
                self.end_voltage = 2.5
                self.nominal_voltage = 3.6
                self.cutoff_voltage = 4.2
                # Current values (A)
                self.standard_charging_current = 1.5
                self.max_charging_current = 4
                self.max_discharging_current = 15
                self.current = current
                # Capacity values (mAh)
                self.discharge_capacity = 2950
                self.capacity = capacity
                self.charge = charge if charge is not None else capacity
                # Temperature values (degrees C)
                # TODO
                # Variable values
                self.voltage = self.calc_voltage()
            

            def __str__(self):
                return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rCharge: {self.charge} mAh\n\r"
            

            def update(self):
                self.charge = self.calc_charge()
                self.voltage = self.calc_voltage()
            

            def calc_charge(self):
                return self.charge - (self.current/3600)


            def calc_voltage(self):
                slope = (self.end_voltage - self.cutoff_voltage) / self.capacity
                return self.cutoff_voltage + ((self.capacity- self.charge) * slope)



if __name__ == "__main__":
    battery = Battery(30, config.total_batt_pack_capacity_mah, 2500 * config.num_batt_cells_parallel_per_module)
    for i in range(50):
        print(battery)
        #for module in battery.modules:
        #    print(module)
        battery.update()