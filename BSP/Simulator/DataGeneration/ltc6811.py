"""
LTC6811 helper class for the BSP SPI simulation.

For more information about the LTC6811, look at the LTC6811 datasheet by
Analog Devices. Some documentation also exists such as the LTC data format
in the team Google Drive under Electrical Team > BPS/BMS > BPS > Documentation/Reports
"""
import config
import debug

LTC6811_NUM_REG = 6

LTC6811_GPIO1_IDX = 0
LTC6811_GPIO2_IDX = 0
LTC6811_GPIO3_IDX = 0

PULL_MAX_VOLT = 40000
PULL_MIN_VOLT = 30000

cv_groups = {
    'A': 0,
    'B': 3,
    'C': 6,
    'D': 9,
    'E': 12
}

ad_commands = {
    'ADCV': 0,
    'ADOWPU': 1,
    'ADOWPD': 2
}

# Handler functions must be defined at the beginning because of the command_codes
# dictionary's defined location.
def wrcfga_handler(ltc6811):
    debug.log("wrcfga")
    # Copy contents of rx_data into config_reg
    ltc6811.config_reg = ltc6811.rx_data.copy()
    ltc6811.tx_available = 0   # Reset because there shouldn't be any data sent back

def rdcfga_handler(ltc6811):
    debug.log("rdcfga")
    ltc6811.tx_data = ltc6811.config_reg.copy()
    ltc6811.tx_available = 1

def adcv_md0_dcp0_ch0_handler(ltc6811):
    # Start Cell ADC Measurements
    debug.log("adcv_md0_dcp0_ch0")
    ltc6811.tx_available = 0   # Reset because there shouldn't be any data sent back
    # Start ADC conversion. Not much to do here.
    ltc6811.curr_ad_command = ad_commands['ADCV']

def pladc_handler(ltc6811):
    debug.log("poll adc")
    # While the uC is polling the LTC to see if the ADC is done, there is 
    # a count up timer in the LTC that measures how long the ADC took to
    # complete. If the LTC returns 0, then then the ADC still has not finished
    # yet.
    # Write a value greater than 0 to indicate the ADC finished.
    ltc6811.tx_data = [0] * LTC6811_NUM_REG
    ltc6811.tx_data[0] = 1
    ltc6811.tx_available = 1

def rdcv_helper(ltc6811, group):

    tx_idx = 0

    # Check which AD command was set to
    voltages = ltc6811.voltages
    if ltc6811.curr_ad_command == ad_commands['ADOWPU']:
        voltages = ltc6811.pullup_volt
    elif ltc6811.curr_ad_command == ad_commands['ADOWPD']:
        voltages = ltc6811.pulldown_volt

    for volt_idx in range(3):
        ltc6811.tx_data[tx_idx] = voltages[cv_groups[group]+volt_idx] & 0x00FF
        ltc6811.tx_data[tx_idx+1] = (voltages[cv_groups[group]+volt_idx] >> 8) & 0x00FF
        tx_idx += 2

def rdcva_handler(ltc6811):
    # Read 0-2 cell voltages (3 at a time)
    debug.log("rdcva")
    rdcv_helper(ltc6811, 'A')
    ltc6811.tx_available = 1

def rdcvb_handler(ltc6811):
    # Read 3-5 cell voltages (3 at a time)
    debug.log("rdcvb")
    rdcv_helper(ltc6811, 'B')
    ltc6811.tx_available = 1

def rdcvc_handler(ltc6811):
    # Read 6-8 cell voltages (3 at a time)
    debug.log("rdcvc")
    rdcv_helper(ltc6811, 'C')
    ltc6811.tx_available = 1

def rdcvd_handler(ltc6811):
    # Read 9-11 cell voltages (3 at a time)
    debug.log("rdcvd")
    rdcv_helper(ltc6811, 'D')
    ltc6811.tx_available = 1

def clrcell_handler(ltc6811):
    # Resets AUX registers to 1
    debug.log("clrcell")

def adowpu_md0_dcp0_ch0_handler(ltc6811):
    # Open Wire measure pull up voltages
    debug.log("adowpu_md0_dcp0_ch0")
    ltc6811.curr_ad_command = ad_commands['ADOWPU']
    ltc6811.tx_available = 0   # Reset because there shouldn't be any data sent back
    for wire_idx in range(12):
        if ltc6811.open_wires[wire_idx] == 1:
            ltc6811.pullup_volt[wire_idx] = PULL_MAX_VOLT
        else:
            ltc6811.pullup_volt[wire_idx] = PULL_MIN_VOLT

def adowpd_md0_dcp0_ch0_handler(ltc6811):
    # Open Wire measure pull down voltages
    debug.log("adowpd_md0_dcp0_ch0")
    ltc6811.curr_ad_command = ad_commands['ADOWPD']
    ltc6811.tx_available = 0   # Reset because there shouldn't be any data sent back
    for wire_idx in range(12):
        if ltc6811.open_wires[wire_idx] == 1:
            ltc6811.pulldown_volt[wire_idx] = PULL_MIN_VOLT
        else:
            ltc6811.pulldown_volt[wire_idx] = PULL_MAX_VOLT

def adax_md0_chg1_handler(ltc6811):
    # Start AUX ADC GPIO1 converstion
    debug.log("adax_md0_chg1")
    ltc6811.tx_available = 0   # Reset because there shouldn't be any data sent back
    ltc6811.update_gpios()
    ltc6811.tx_available = 0

def rdauxa_handler(ltc6811):
    # Read AUX ADC GPIO1,2,3 register
    debug.log("rdauxa")
    ltc6811.tx_data = [0] * LTC6811_NUM_REG   # Clear
    ltc6811.tx_data[0] = ltc6811.gpio[LTC6811_GPIO1_IDX] & 0x00FF
    ltc6811.tx_data[1] = (ltc6811.gpio[LTC6811_GPIO1_IDX] >> 8) & 0x00FF
    ltc6811.tx_available = 1

def wrcomm_handler(ltc6811):
    '''
    rx_data[0] = (AUX_I2C_START << 4) + ((mux_addr & 0xF0)>>4); 				
    rx_data[1] = ((mux_addr & 0x0F) << 4) + AUX_I2C_NACK;
    
    // Sends what channel to open. 4th bit is the enable bit, the 4MSB bits will be by default high
    // Turn all channels off
    rx_data[2] = (AUX_I2C_BLANK << 4) + 0xF;
    rx_data[3] = (0 << 4) + AUX_I2C_NACK_STOP; or rx_data[3] = ((8 + tempChannel) << 4) + AUX_I2C_NACK_STOP;

    // Rest is no transmit with all data bits set to high, makes sure there's nothing else we're sending
    rx_data[4] = (AUX_I2C_NO_TRANSMIT << 4) + 0xF;
    rx_data[5] = (0xF << 4) + AUX_I2C_NACK_STOP;
    '''
    # Write I2C
    debug.log("wrcomm")
    ltc6811.tx_available = 0
    i2c_addr = ((ltc6811.rx_data[0] & 0x0F) << 4) | ((ltc6811.rx_data[1] >> 4) & 0x0F)
    i2c_data = (ltc6811.rx_data[3] >> 4) & 0x0F

    mux_en = False
    mux_addr = i2c_addr
    mux_sel = 0
    if i2c_data > 0:
        mux_en = True
        mux_sel = i2c_data - 8
    ltc6811.set_muxes(mux_en, mux_addr, mux_sel)


def stcomm_handler(ltc6811):
    # Start I2C
    debug.log("stcomm")
    # Transmit


# Dictionary of all LTC6811 command codes
# command_codes is nested dictionary
# {cmd_key : {code, handler}}
# cmd_key is the string representation of the code for readability
# code is the number value of the command
# handler is the function to call to handle its respective command
command_codes = {
    'SIM_LTC6811_WRCFGA'    : {'code': 0x001, 'handler': wrcfga_handler},
    'SIM_LTC6811_WRCFGB'    : {'code': 0x024, 'handler': None},
    'SIM_LTC6811_RDCFGA'    : {'code': 0x002, 'handler': rdcfga_handler},
    'SIM_LTC6811_RDCFGB'    : {'code': 0x026, 'handler': None},
    'SIM_LTC6811_RDCVA'     : {'code': 0x004, 'handler': rdcva_handler},
    'SIM_LTC6811_RDCVB'     : {'code': 0x006, 'handler': rdcvb_handler},
    'SIM_LTC6811_RDCVC'     : {'code': 0x008, 'handler': rdcvc_handler},
    'SIM_LTC6811_RDCVD'     : {'code': 0x00A, 'handler': rdcvd_handler},
    'SIM_LTC6811_RDCVE'     : {'code': 0x009, 'handler': None},
    'SIM_LTC6811_RDCVF'     : {'code': 0x00B, 'handler': None},
    'SIM_LTC6811_RDAUXA'    : {'code': 0x00C, 'handler': rdauxa_handler},
    'SIM_LTC6811_RDAUXB'    : {'code': 0x00E, 'handler': None},
    'SIM_LTC6811_RDAUXC'    : {'code': 0x00D, 'handler': None},
    'SIM_LTC6811_RDAUXD'    : {'code': 0x00F, 'handler': None},
    'SIM_LTC6811_RDSTATA'   : {'code': 0x010, 'handler': None},
    'SIM_LTC6811_RDSTATB'   : {'code': 0x012, 'handler': None},
    'SIM_LTC6811_WRSCTRL'   : {'code': 0x014, 'handler': None},
    'SIM_LTC6811_WRPWM'     : {'code': 0x020, 'handler': None},
    'SIM_LTC6811_WRPSB'     : {'code': 0x01C, 'handler': None},
    'SIM_LTC6811_RDSCTRL'   : {'code': 0x016, 'handler': None},
    'SIM_LTC6811_RDPWM'     : {'code': 0x022, 'handler': None},
    'SIM_LTC6811_RDPSB'     : {'code': 0x01E, 'handler': None},
    'SIM_LTC6811_STSCTRL'   : {'code': 0x019, 'handler': None},
    'SIM_LTC6811_CLRSCTRL'  : {'code': 0x018, 'handler': None},
    'SIM_LTC6811_ADCV_DEFAULT'          : {'code': 0x260, 'handler': None},       # 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADCV_MD0_DCP0_CH0'     : {'code': 0x260, 'handler': adcv_md0_dcp0_ch0_handler},    # 0 1 0 0 1 1 0 0 0 0 0
    'SIM_LTC6811_ADOWPU_DEFAULT'        : {'code': 0x268, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPU_MD0_DCP0_CH0'   : {'code': 0x4E8, 'handler': adowpu_md0_dcp0_ch0_handler},  # 0 1 1 1 1 1 0 1 0 0 0
    'SIM_LTC6811_ADOWPD_DEFAULT'        : {'code': 0x228, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPD_MD0_DCP0_CH0'   : {'code': 0x4C8, 'handler': adowpd_md0_dcp0_ch0_handler},  # 0 1 1 1 0 1 0 1 0 0 0
    'SIM_LTC6811_CVST'      : {'code': 0x207, 'handler': None},       # 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADOL'      : {'code': 0x201, 'handler': None},       # 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
    'SIM_LTC6811_ADAX_DEFAULT'          : {'code': 0x460, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_ADAX_MD0_CHG1'         : {'code': 0x461, 'handler': adax_md0_chg1_handler},        # 1 0 0 0 1 1 0 0 0 0 1
    'SIM_LTC6811_ADAXD'     : {'code': 0x400, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_AXST'      : {'code': 0x407, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADSTAT'    : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_ADSTATD'   : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_STATST'    : {'code': 0x40F, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
    'SIM_LTC6811_ADCVAX'    : {'code': 0x46F, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
    'SIM_LTC6811_ADCVSC'    : {'code': 0x467, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
    'SIM_LTC6811_CLRCELL'   : {'code': 0x711, 'handler': clrcell_handler},
    'SIM_LTC6811_CLRAUX'    : {'code': 0x712, 'handler': None},
    'SIM_LTC6811_CLRSTAT'   : {'code': 0x713, 'handler': None},
    'SIM_LTC6811_PLADC'     : {'code': 0x714, 'handler': pladc_handler},
    'SIM_LTC6811_DIAGN'     : {'code': 0x715, 'handler': None},
    'SIM_LTC6811_WRCOMM'    : {'code': 0x721, 'handler': wrcomm_handler},
    'SIM_LTC6811_RDCOMM'    : {'code': 0x722, 'handler': None},
    'SIM_LTC6811_STCOMM'    : {'code': 0x723, 'handler': stcomm_handler}
}

mux_addresses = {
    'SIM_LTC1380_MUX1' : 0x90,        # MUX addresses on Minion boards
    'SIM_LTC1380_MUX2' : 0x92
}

mux_idxs = {
    'SIM_LTC1380_MUX1' : 0,
    'SIM_LTC1380_MUX2' : 1
}

CRC15_POLY = 0x4599
pec15_table = [0] * 256


def format_full_protocol(ics):
    '''
    @brief  Formats the tx_data in the LTC6811 objects to what the LTC6811.c
            driver expects.
    @param  ics : list of LTC6811 objects
    '''

    protocol = []

    for ic in ics:

        if ic.tx_available == 0:
            return None

        # Copy data
        for byte in ic.tx_data:
            protocol.append(byte)
        
        pec = calc_pec(ic.tx_data)
        protocol.append((pec >> 8) & 0x00FF)    # PEC0
        protocol.append(pec & 0x00FF)           # PEC1

        # reset tx_available since we consumed it
        ic.tx_available = 0

    return protocol



def parse_full_protocol(ics, protocol):
    '''
    @brief  Parses the data and stores data into the LTC6811 objects rx_data
            fields
    @param  ics : list of LTC6811 objects
    @param  protocol : data that LTC6811 hardware expects to receive.
                        Format is in google drive in
                        Electrical Team > BPS/BMS > BPS > Documentation/Reports
                        
    '''

    # First to bytes are the command
    # The next two lines of code pops the first 4 bytes which are the cmd
    #   related bytes.
    cmd = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)
    cmd_pec = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)

    # Check if only the cmd was sent. There are some commands that the LTC6811
    # driver can send that only contains the 4B of command code and nothing else.
    if len(protocol) == 0:
        for ic in ics:
            # Staging phase of LTC object
            ic.stage(cmd)

            # Update LTC object
            ic.update()
        return

    # Update all the LTCs with new cmd
    for ic in ics:
        data = [0] * LTC6811_NUM_REG
        for i in range(LTC6811_NUM_REG):
            data[i] = protocol.pop(0)   # Pop the data

        # data_pec saved but not needed
        data_pec = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)

        # Staging phase of LTC object
        ic.stage(cmd, data)

        # Update LTC object
        ic.update()



def print_protocol(protocol):
    '''
    @brief  debug.logs formatted data message in easy to read format
    @param  protocol : data that the LTC6811 hardware expects to receive
    '''
    print()
    print("Idx : Data")
    idx = 0
    for byte in protocol:
        print(" {0:2} : {1}".format(idx, hex(protocol[idx])))
        idx += 1



def pec_init():
    '''
    @brief  Initializes PEC look up table
    @note   Must be called once
    '''
    for i in range(256):
        remainder = i << 7
        for bit in range(8, 0, -1):
            if remainder & 0x4000:
                remainder = ((remainder << 1))
                remainder = (remainder ^ CRC15_POLY)
            else:
                remainder = ((remainder << 1))
        pec15_table[i] = (remainder&0x0000FFFF)



def calc_pec(bytes):
    """
    @brief  calculates 16-bit PEC value of the byte array
    @param  bytes : array of bytes that the algorithm will use to calculate
                    PEC. For the LTC6811, the length of the array is either
                    2 (command) or 6 (data) bytes long
    """
    address = 0
    remainder = 16  # PEC seed
    for i in range(len(bytes)):
        address = ((remainder >> 7) ^ bytes[i]) & 0xff   #calculate PEC table address
        remainder = (remainder << 8 ) ^ pec15_table[address]
    return (remainder*2 & 0x0000FFFF) # The CRC15 has a 0 in the LSB so the final value must be multiplied by 2


class LTC6811:

    def __init__(self, batt_modules):
        self.batt_modules = batt_modules

        # Temporary solution.
        # Voltages and temperatures should be reference batt modules object list
        self.voltages = [0] * 12            # Max number batt minion brd can handle: 12 batt
        self.open_wires = [0] * 12          # 0 means close, 1 means open
        self.pullup_volt = [0] * 12
        self.pulldown_volt = [0] * 12

        self.ltc1380s = [self.LTC1380(mux_addresses['SIM_LTC1380_MUX1']), self.LTC1380(mux_addresses['SIM_LTC1380_MUX2'])]

        # reg suffixes indicate data stored in LTC6811 registers
        self.config_reg = [0] * LTC6811_NUM_REG # 6B: Configuration register of LTC
        self.gpio = [0] * 5                     # 10B: 5 GPIO Registers (2B per GPIO)        

        # staging buffers that will be used when formatting bus
        self.curr_cmd_code = 0  # 12-bits
        self.curr_ad_command = ad_commands['ADCV']
        self.tx_available = 0
        self.tx_data = [0] * LTC6811_NUM_REG    # 6B: POV of simulator
        self.rx_data = [0] * LTC6811_NUM_REG    # 6B

        # As long this is called once, pec will work
        pec_init()


    def __str__(self):
        ltc_str = "Config: "
        for byte in self.config_reg:
            ltc_str += hex(byte & 0x00FF) + " "

        ltc_str += '\nGPIO: '
        for gpio in self.gpio:
            ltc_str += hex(gpio & 0xFFFF) + " "

        ltc_str += '\nTx_Data: '
        for byte in self.tx_data:
            ltc_str += hex(byte & 0x00FF) + " "

        ltc_str += '\nRx_Data: '
        for byte in self.tx_data:
            ltc_str += hex(byte & 0x00FF) + " "

        return ltc_str


    def stage(self, new_cmd_code, data=None):
        '''
        @brief  Prepares the LTC6811 object to update the LTC simulator.
        @note   This must be called before update(). This is the staging phase to update
                the object with new parameters.
        @param  new_cmd_code : the new command that was received from the BPS firmware.
        @param  data : data that was received from the BPS firmware.
        '''
        self.curr_cmd_code = new_cmd_code
        # Handle cmd cases such as MD, ST, etc.
        if data == None:
            data = [0] * LTC6811_NUM_REG  # Initialize as empty
        self.rx_data = data.copy()
    

    def update(self):
        '''
        @brief  Updates the LTC6811 object with the new command that was set in stage_update.
        '''

        # command_codes is nested dictionary
        # {cmd_key : {code, handler}}
        # cmd_key is the string representation of the code for readability
        # code is the number value of the command
        # handler is the function to call to handle its respective command
        for cmd_key in command_codes:
            if self.curr_cmd_code == command_codes[cmd_key]['code']:
                cmd_handler = command_codSIM_LTC1380_MUX1es[cmd_key]['handler']
                
                # Execute only if handler exists
                if cmd_handler is not None:
                    cmd_handler(self)


    def set_voltage(self, voltages):
        self.voltages = voltages.copy()
    
    def set_temperatures(self, temperatures_cel):
        temp_idx = 0

        # Separate the two temperature sensors per module into separate LTC1380s
        # [a,b] where a goes to mux1 and b goes to mux2
        for a, b in temperatures_cel:
            self.ltc1380s[mux_idxs['SIM_LTC1380_MUX1']].temperatures_cel[temp_idx] = a
            self.ltc1380s[mux_idxs['SIM_LTC1380_MUX2']].temperatures_cel[temp_idx] = b
            temp_idx += 1

    def set_open_wires(self, open_wires):
        '''
        @brief  updates array of open_wires
        @param  open_wires : list of open_wires, each index corresponds to batt module.
                            0 for not open, 1 for open
        '''
        self.open_wires = open_wires.copy()

    def set_single_wire(self, idx, open):
        '''
        @brief  sets a single wire to open or closed
        @param  idx : batt module 0-indexed
        @param  open : 0 for not open (closed), 1 for open
        '''
        self.open_wires[idx] = open

    def set_voltage_pullup(self, pu_volt):
        self.pullup_volt = pu_volt.copy()

    def set_voltage_pulldown(self, pd_volt):
        self.pulldown_volt = pd_volt.copy()

    def set_muxes(self, en, mux_addr, mux_sel):
        for ltc1380 in self.ltc1380s:
            ltc1380.update(en, mux_addr, mux_sel)

    def update_gpios(self):
        gpio1 = -1
        for ltc1380 in self.ltc1380s:
            value = ltc1380.get_temperature()
            if value != -1:
                # Valid
                gpio1 = value

        if gpio1 == -1:
            while True:
                pass

        self.gpio[LTC6811_GPIO1_IDX] = gpio1
        

    class LTC1380:
        def __init__(self, mux_addr):
            self.mux_addr = mux_addr
            self.mux_en = False
            self.mux_sel = 0
            self.temperatures_cel = [0] * 8

        def set_temperatures(self, temperatures_cel):
            self.temperatures_cel = temperatures_cel

        def enable(self, mux_en):
            '''
            @brief  Enable inputs or not.
            @param  mux_en : True or False if enabled or disabled
            '''
            self.mux_en = mux_en

        def select_input(self, input):
            if input < 8 and input >= 0:
                self.mux_sel = input
            else:
                self.mux_sel = 0

        def get_temperature(self):
            if self.mux_en == True:
                return self.temperatures_cel[self.mux_sel]
            else:
                return -1

        def update(self, mux_en, mux_addr, mux_sel):
            if self.mux_addr == mux_addr:
                self.enable(mux_en)
                self.select_input(mux_sel)






if __name__ == "__main__":
    # LTC6811 objects
    minions = []
    voltages = [25000, 26000, 27000, 28000, 39000, 40000, 41000, 42000, 0, 0, 0, 0]
    temperatures = [[25000, 25001], [26000, 26001], [27000, 27001], [28000, 28001], [29000, 29001], [30000, 30001], [31000, 31001], [32000, 32001]]
    config = [0, 1, 2, 3, 4, 5]
    for i in range(4):
        minions.append(LTC6811(None))
        minions[i].set_voltage(voltages)
        minions[i].set_temperatures(temperatures)

    # Unit Testing

    # Test xxcfga_handler
    fail = 0
    minions[0].rx_data = config
    wrcfga_handler(minions[0])
    if minions[0].config_reg != config:
        fail = 1
    debug.log("wrcfga_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    fail = 0
    rdcfga_handler(minions[0])
    if minions[0].tx_data != config:
        fail = 1
    debug.log("rdcfga_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test rdcva_handler
    rdcva_handler(minions[0])
    tx_idx = 0
    fail = 0
    for i in range(3):
        batt_voltage = voltages[cv_groups['A']+i]
        tx_voltage = (minions[0].tx_data[tx_idx] & 0x00FF) | ((minions[0].tx_data[tx_idx+1] & 0x00FF) << 8)
        tx_idx += 2
        if batt_voltage != tx_voltage:
            fail = 1
    debug.log("rdcva_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test rdcvb_handler
    rdcvb_handler(minions[0])
    tx_idx = 0
    fail = 0
    for i in range(3):
        batt_voltage = voltages[cv_groups['B']+i]
        tx_voltage = (minions[0].tx_data[tx_idx] & 0x00FF) | ((minions[0].tx_data[tx_idx+1] & 0x00FF) << 8)
        tx_idx += 2
        if batt_voltage != tx_voltage:
            fail = 1
    debug.log("rdcvb_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test rdcvc_handler
    rdcvc_handler(minions[0])
    tx_idx = 0
    fail = 0
    for i in range(3):
        batt_voltage = voltages[cv_groups['C']+i]
        tx_voltage = (minions[0].tx_data[tx_idx] & 0x00FF) | ((minions[0].tx_data[tx_idx+1] & 0x00FF) << 8)
        tx_idx += 2
        if batt_voltage != tx_voltage:
            fail = 1
    debug.log("rdcvc_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test ADOWPU
    minions[0].set_single_wire(2, 1)
    adowpu_md0_dcp0_ch0_handler(minions[0])
    rdcva_handler(minions[0])
    tx_idx = 0
    fail = 0
    batt_voltage = [30000, 30000, 40000]
    for i in range(3):
        tx_voltage = (minions[0].tx_data[tx_idx] & 0x00FF) | ((minions[0].tx_data[tx_idx+1] & 0x00FF) << 8)
        tx_idx += 2
        if batt_voltage[i] != tx_voltage:
            fail = 1
    debug.log("adowpu_md0_dcp0_ch0_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test ADOWPD
    minions[0].set_single_wire(2, 1)
    adowpd_md0_dcp0_ch0_handler(minions[0])
    rdcva_handler(minions[0])
    tx_idx = 0
    fail = 0
    batt_voltage = [40000, 40000, 30000]
    for i in range(3):
        tx_voltage = (minions[0].tx_data[tx_idx] & 0x00FF) | ((minions[0].tx_data[tx_idx+1] & 0x00FF) << 8)
        tx_idx += 2
        if batt_voltage[i] != tx_voltage:
            fail = 1
    debug.log("adowpd_md0_dcp0_ch0_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test WRCOMM and STCOMM
    minions[0].rx_data = [0xF9, 0x0F, 0xFF, 0x0F, 0xFF, 0xFF]
    wrcomm_handler(minions[0])
    fail = 0
    if minions[0].ltc1380s[mux_idxs['SIM_LTC1380_MUX1']].mux_addr != mux_addresses['SIM_LTC1380_MUX1'] or minions[0].ltc1380s[mux_idxs['SIM_LTC1380_MUX1']].mux_en is True:
        fail = 1
    minions[0].rx_data = [0xF9, 0x2F, 0xFF, 0x9F, 0xFF, 0xFF]
    wrcomm_handler(minions[0])
    if minions[0].ltc1380s[mux_idxs['SIM_LTC1380_MUX2']].mux_addr != mux_addresses['SIM_LTC1380_MUX2'] or minions[0].ltc1380s[mux_idxs['SIM_LTC1380_MUX2']].mux_sel != 0x01 or minions[0].ltc1380s[mux_idxs['SIM_LTC1380_MUX2']].mux_en is not True:
        fail = 1
    debug.log("wrcomm_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test ADAX
    adax_md0_chg1_handler(minions[0])
    fail = 0
    # Last WRCOMM command wrote to MUX2
    if minions[0].gpio[0] != temperatures[1][1]:
        fail = 1
    debug.log("adax_md0_chg1_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    # Test RDAUXA
    rdauxa_handler(minions[0])
    fail = 0
    tx_voltage = (minions[0].tx_data[0] & 0x00FF) | ((minions[0].tx_data[1] & 0x00FF) << 8)
    if temperatures[1][1] != tx_voltage:
        fail = 1
    debug.log("rdauxa_handler: ", end='')
    debug.log('FAIL' if fail else 'Success')

    for minion in minions:
        debug.log(minion)