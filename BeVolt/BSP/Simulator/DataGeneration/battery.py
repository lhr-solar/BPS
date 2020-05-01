"""
Battery class to simulate the real Li-ion battery cells
used in BeVolt

All specifications taken from the specification sheets
on the team Google Drive under the Battery folder
"""

# A battery consists of 31 Modules in series
class Battery:
    def __init__(self, current, capacity, soc=None):
        self.num_modules = 31
        self.current = current
        self.capacity = capacity
        self.soc = soc if soc is not None else capacity
        self.modules = self.create_modules()
        self.voltage = self.calc_voltage()
    

    def __str__(self):
        return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rSoC: {self.soc} mAh\n\r"


    def create_modules(self):
        module_list = []
        for i in range(self.num_modules):
            module_list.append(self.Module(self.current, self.capacity/self.num_modules, self.soc/self.num_modules))
        return module_list


    def update(self):
        for module in self.modules:
            module.update()
        self.soc = self.calc_soc()
        self.voltage = self.calc_voltage()


    def calc_soc(self):
        return sum([module.soc for module in self.modules])


    def calc_voltage(self):
        return sum([module.voltage for module in self.modules])


    # A module consists of 14 Cells in parallel
    class Module:
        def __init__(self, current, capacity, soc=None):
            self.num_cells = 14
            self.current = current
            self.capacity = capacity
            self.soc = soc if soc is not None else capacity
            self.cells = self.create_cells()
            self.voltage = self.calc_voltage()
        

        def __str__(self):
            return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rSoC: {self.soc} mAh\n\r"


        def create_cells(self):
            cell_list = []
            for i in range(self.num_cells):
                cell_list.append(self.Cell(self.current/self.num_cells, self.capacity/self.num_cells, self.soc/self.num_cells))
            return cell_list
        

        def update(self):
            for cell in self.cells:
                cell.update()
            self.soc = self.calc_soc()
            self.voltage = self.calc_voltage()


        def calc_soc(self):
            return sum([cell.soc for cell in self.cells])


        def calc_voltage(self):
            return sum([cell.voltage for cell in self.cells]) / self.num_cells


        class Cell:
            def __init__(self, current, capacity, soc=None):
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
                self.soc = soc if soc is not None else capacity
                # Temperature values (degrees C)
                # TODO
                # Variable values
                self.voltage = self.calc_voltage()
            

            def __str__(self):
                return f"Voltage: {self.voltage} V\n\rCurrent: {self.current} A\n\rSoC: {self.soc} mAh\n\r"
            

            def update(self):
                self.soc = self.calc_soc()
                self.voltage = self.calc_voltage()
            

            def calc_soc(self):
                return self.soc - (self.current/3600)


            def calc_voltage(self):
                slope = (self.end_voltage - self.cutoff_voltage) / self.capacity
                return self.cutoff_voltage + ((self.capacity- self.soc) * slope)
