"""
LTC6811 helper class for the BSP SPI simulation.

For more information about the LTC6811, look at the LTC6811 datasheet by
Analog Devices. Some documentation also exists such as the LTC data format
in the team Google Drive under Electrical Team > BPS/BMS > BPS > Documentation/Reports
"""
import config


# Handler functions must be defined at the beginning because of the command_codes
# dictionary's defined location.
def wrcfga_handler(ltc6811):
    print("wrcfga")
    # Copy contents of rx_data into config_reg
    ltc6811.config_reg = ltc6811.rx_data.copy()
    ltc6811.tx_data = [0] * 6   # Reset because there shouldn't be any data sent back

def rdcfga_handler(ltc6811):
    print("rdcfga")
    ltc6811.tx_data = ltc6811.config_reg.copy()


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
    'SIM_LTC6811_RDCVA'     : {'code': 0x004, 'handler': None},
    'SIM_LTC6811_RDCVB'     : {'code': 0x006, 'handler': None},
    'SIM_LTC6811_RDCVC'     : {'code': 0x008, 'handler': None},
    'SIM_LTC6811_RDCVD'     : {'code': 0x00A, 'handler': None},
    'SIM_LTC6811_RDCVE'     : {'code': 0x009, 'handler': None},
    'SIM_LTC6811_RDCVF'     : {'code': 0x00B, 'handler': None},
    'SIM_LTC6811_RDAUXA'    : {'code': 0x00C, 'handler': None},
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
    'SIM_LTC6811_ADCV'      : {'code': 0x260, 'handler': None},       # 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPU'    : {'code': 0x268, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPD'    : {'code': 0x228, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_CVST'      : {'code': 0x207, 'handler': None},       # 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADOL'      : {'code': 0x201, 'handler': None},       # 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
    'SIM_LTC6811_ADAX'      : {'code': 0x460, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_ADAXD'     : {'code': 0x400, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_AXST'      : {'code': 0x407, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADSTAT'    : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_ADSTATD'   : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_STATST'    : {'code': 0x40F, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
    'SIM_LTC6811_ADCVAX'    : {'code': 0x46F, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
    'SIM_LTC6811_ADCVSC'    : {'code': 0x467, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
    'SIM_LTC6811_CLRCELL'   : {'code': 0x711, 'handler': None},
    'SIM_LTC6811_CLRAUX'    : {'code': 0x712, 'handler': None},
    'SIM_LTC6811_CLRSTAT'   : {'code': 0x713, 'handler': None},
    'SIM_LTC6811_PLADC'     : {'code': 0x714, 'handler': None},
    'SIM_LTC6811_DIAGN'     : {'code': 0x715, 'handler': None},
    'SIM_LTC6811_WRCOMM'    : {'code': 0x721, 'handler': None},
    'SIM_LTC6811_RDCOMM'    : {'code': 0x722, 'handler': None},
    'SIM_LTC6811_STCOMM'    : {'code': 0x723, 'handler': None}
}

mux_addresses = {
    'SIM_LTC1380_MUX1' : 0x90,        # MUX addresses on Minion boards
    'SIM_LTC1380_MUX2' : 0x92
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
        # Copy data
        for byte in ic.tx_data:
            protocol.append(byte)
        
        pec = calc_pec(ic.tx_data)
        protocol.append((pec >> 8) & 0x00FF)    # PEC0
        protocol.append(pec & 0x00FF)           # PEC1

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
        data = [0] * 6
        for i in range(6):
            data[i] = protocol.pop(0)   # Pop the data

        # data_pec saved but not needed
        data_pec = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)

        # Staging phase of LTC object
        ic.stage(cmd, data)

        # Update LTC object
        ic.update()



def print_protocol(protocol):
    '''
    @brief  Prints formatted data message in easy to read format
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
        self.voltages = [0] * 8
        self.temperatures_cel = [0, 0] * 8

        # reg suffixes indicate data stored in LTC6811 registers
        self.config_reg = [0] * 6   # 6B: Configuration register of LTC

        # staging buffers that will be used when formatting bus
        self.curr_cmd_code = 0  # 12-bits
        self.tx_data = [0] * 6  # 6B: POV of simulator
        self.rx_data = [0] * 6  # 6B

        # As long this is called once, pec will work
        pec_init()


    def __str__(self):
        ltc_str = "Config: "
        for byte in self.config_reg:
            ltc_str += hex(byte) + " "

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
        if data == None:
            data = [0] * 6  # Initialize as empty
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
                cmd_handler = command_codes[cmd_key]['handler']
                
                # Execute only if handler exists
                if cmd_handler is not None:
                    cmd_handler(self)


    def set_voltage(self, voltages):
        self.voltages = voltages.copy()
    
    def set_temperatures(self, temperatures_cel):
        self.temperatures_cel = temperatures_cel.copy()


