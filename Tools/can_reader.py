"""
python script to read from a serial port and print the output to the console
"""

import argparse
import serial
import serial.tools.list_ports
from collections import namedtuple
import binascii

class CarCANMsg():

    CANMeta = namedtuple('CANMeta', ['idx', 'len'])

    def __init__(self, id, idx, data) -> None:
        self.id = id
        self.idx = idx
        self.data = data

    @classmethod
    def from_human_readable_bytes(cls, b):
        id = int.from_bytes(binascii.unhexlify(b'0'+b[0:3]), byteorder='big', signed=False)
        idx = int.from_bytes(binascii.unhexlify(b[4:6]), byteorder='big', signed=False) if cls.CANMetaLUT[id].idx else None
        data = binascii.unhexlify(b[7:])
        return cls(id, idx, data)
    
    def __str__(self) -> str:
        return f'ID:{self.id}, IDX:{self.idx}, DATA:{" ".join([f"{b:02x}" for b in self.data])}'

    class CarCANID():
        DASH_KILL_SWITCH                                     = 0x001
        BPS_TRIP                                             = 0x002
        ANY_SYSTEM_FAILURES                                  = 0x003
        IGNITION                                             = 0x004
        ANY_SYSTEM_SHUTOFF                                   = 0x005
        BPS_ALL_CLEAR                                        = 0x101
        BPS_CONTACTOR_STATE                                  = 0x102
        CURRENT_DATA                                         = 0x103
        VOLTAGE_DATA_ARRAY                                   = 0x104
        TEMPERATURE_DATA_ARRAY                               = 0x105
        STATE_OF_CHARGE_DATA                                 = 0x106
        WDOG_TRIGGERED                                       = 0x107
        CAN_ERROR                                            = 0x108
        BPS_COMMAND_MSG                                      = 0x109
        SUPPLEMENTAL_VOLTAGE                                 = 0x10B
        CHARGING_ENABLED                                     = 0x10C
        CAR_STATE                                            = 0x580
        IO_STATE                                             = 0x581
        MOTOR_CONTROLLER_BUS                                 = 0x242
        VELOCITY                                             = 0x243
        MOTOR_CONTROLLER_PHASE_CURRENT                       = 0x244
        MOTOR_VOLTAGE_VECTOR                                 = 0x245
        MOTOR_CURRENT_VECTOR                                 = 0x246
        MOTOR_BACKEMF                                        = 0x247
        MOTOR_TEMPERATURE                                    = 0x24B
        ODOMETER__BUS_AMP_HOURS                              = 0x24E
        ARRAY_CONTACTOR_STATE_CHANGE                         = 0x24F
        SUNSCATTER_A_MPPT_1_ARRAY_VOLTAGE_SETPOINT           = 0x600
        SUNSCATTER_A_ARRAY_VOLTAGE_MEASUREMENT               = 0x601
        SUNSCATTER_A_ARRAY_CURRENT_MEASUREMENT               = 0x602
        SUNSCATTER_A_BATTERY_VOLTAGE_MEASUREMENT             = 0x603
        SUNSCATTER_A_BATTERY_CURRENT_MEASUREMENT             = 0x604
        SUNSCATTER_A_OVERRIDE_ENABLEDISABLE_COMMAND          = 0x605
        SUNSCATTER_A_FAULT                                   = 0x606
        SUNSCATTER_B_MPPT_2_ARRAY_VOLTAGE_SETPOINT           = 0x610
        SUNSCATTER_B_ARRAY_VOLTAGE_MEASUREMENT               = 0x611
        SUNSCATTER_B_ARRAY_CURRENT_MEASUREMENT               = 0x612
        SUNSCATTER_B_BATTERY_VOLTAGE_MEASUREMENT             = 0x613
        SUNSCATTER_B_BATTERY_CURRENT_MEASUREMENT             = 0x614
        SUNSCATTER_B_OVERRIDE_ENABLEDISABLE_COMMAND          = 0x615
        SUNSCATTER_B_FAULT                                   = 0x616
        BLACKBODY_RTD_SENSOR_MEASUREMENT                     = 0x620
        BLACKBODY_IRRADIANCE_SENSOR_1_MEASUREMENT            = 0x630
        BLACKBODY_IRRADIANCE_SENSOR_2_MEASUREMENT            = 0x631
        BLACKBODY_IRRADIANCERTD_BOARD_ENABLEDISABLE_COMMAND  = 0x632
        BLACKBODY_IRRADIANCERTD_BOARD_FAULT                  = 0x633
        PV_CURVE_TRACER_PROFILE                              = 0x640

    CANMetaLUT = {
        CarCANID.DASH_KILL_SWITCH:                                     CANMeta(idx=False, len=1),
        CarCANID.BPS_TRIP:                                             CANMeta(idx=False, len=1),
        CarCANID.ANY_SYSTEM_FAILURES:                                  CANMeta(idx=False, len=1),
        CarCANID.IGNITION:                                             CANMeta(idx=False, len=1),
        CarCANID.ANY_SYSTEM_SHUTOFF:                                   CANMeta(idx=False, len=1),
        CarCANID.BPS_ALL_CLEAR:                                        CANMeta(idx=False, len=1),
        CarCANID.BPS_CONTACTOR_STATE:                                  CANMeta(idx=False, len=1),
        CarCANID.CURRENT_DATA:                                         CANMeta(idx=False, len=4),
        CarCANID.VOLTAGE_DATA_ARRAY:                                   CANMeta(idx=True,  len=5),
        CarCANID.TEMPERATURE_DATA_ARRAY:                               CANMeta(idx=True,  len=5),
        CarCANID.STATE_OF_CHARGE_DATA:                                 CANMeta(idx=False, len=4),
        CarCANID.WDOG_TRIGGERED:                                       CANMeta(idx=False, len=1),
        CarCANID.CAN_ERROR:                                            CANMeta(idx=False, len=1),
        CarCANID.BPS_COMMAND_MSG:                                      CANMeta(idx=False, len=8),
        CarCANID.SUPPLEMENTAL_VOLTAGE:                                 CANMeta(idx=False, len=2),
        CarCANID.CHARGING_ENABLED:                                     CANMeta(idx=False, len=1),
        CarCANID.CAR_STATE:                                            CANMeta(idx=False, len=1),
        CarCANID.IO_STATE:                                             CANMeta(idx=False, len=8),
        CarCANID.MOTOR_CONTROLLER_BUS:                                 CANMeta(idx=False, len=8),
        CarCANID.VELOCITY:                                             CANMeta(idx=False, len=8),
        CarCANID.MOTOR_CONTROLLER_PHASE_CURRENT:                       CANMeta(idx=False, len=8),
        CarCANID.MOTOR_VOLTAGE_VECTOR:                                 CANMeta(idx=False, len=8),
        CarCANID.MOTOR_CURRENT_VECTOR:                                 CANMeta(idx=False, len=8),
        CarCANID.MOTOR_BACKEMF:                                        CANMeta(idx=False, len=8),
        CarCANID.MOTOR_TEMPERATURE:                                    CANMeta(idx=False, len=8),
        CarCANID.ODOMETER__BUS_AMP_HOURS:                              CANMeta(idx=False, len=8),
        CarCANID.ARRAY_CONTACTOR_STATE_CHANGE:                         CANMeta(idx=False, len=1),
        CarCANID.SUNSCATTER_A_MPPT_1_ARRAY_VOLTAGE_SETPOINT:           CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_A_ARRAY_VOLTAGE_MEASUREMENT:               CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_A_ARRAY_CURRENT_MEASUREMENT:               CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_A_BATTERY_VOLTAGE_MEASUREMENT:             CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_A_BATTERY_CURRENT_MEASUREMENT:             CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_A_OVERRIDE_ENABLEDISABLE_COMMAND:          CANMeta(idx=False, len=1),
        CarCANID.SUNSCATTER_A_FAULT:                                   CANMeta(idx=False, len=1),
        CarCANID.SUNSCATTER_B_MPPT_2_ARRAY_VOLTAGE_SETPOINT:           CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_B_ARRAY_VOLTAGE_MEASUREMENT:               CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_B_ARRAY_CURRENT_MEASUREMENT:               CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_B_BATTERY_VOLTAGE_MEASUREMENT:             CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_B_BATTERY_CURRENT_MEASUREMENT:             CANMeta(idx=False, len=4),
        CarCANID.SUNSCATTER_B_OVERRIDE_ENABLEDISABLE_COMMAND:          CANMeta(idx=False, len=1),
        CarCANID.SUNSCATTER_B_FAULT:                                   CANMeta(idx=False, len=1),
        CarCANID.BLACKBODY_RTD_SENSOR_MEASUREMENT:                     CANMeta(idx=False, len=5),
        CarCANID.BLACKBODY_IRRADIANCE_SENSOR_1_MEASUREMENT:            CANMeta(idx=False, len=4),
        CarCANID.BLACKBODY_IRRADIANCE_SENSOR_2_MEASUREMENT:            CANMeta(idx=False, len=4),
        CarCANID.BLACKBODY_IRRADIANCERTD_BOARD_ENABLEDISABLE_COMMAND:  CANMeta(idx=False, len=1),
        CarCANID.BLACKBODY_IRRADIANCERTD_BOARD_FAULT:                  CANMeta(idx=False, len=1),
        CarCANID.PV_CURVE_TRACER_PROFILE:                              CANMeta(idx=False, len=5)
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__, 
                                     epilog='\n\r'.join([str(s) for s in serial.tools.list_ports.comports()]))
    parser.add_argument('-p', '--port', type=str, default='',
                        help='serial port. if unspecified, will automatically search for one')
    parser.add_argument('-b', '--baudrate', type=int, default=115200,
                        help='serial port baudrate. default is 115200')
    parser.add_argument('-l', '--log', type=str, default=None,
                        help='log file to write to. default is None')
    args = parser.parse_args()

    ser = serial.Serial(args.port, args.baudrate)
    while True:
        incoming = ser.readline().strip()
        print(CarCANMsg.from_human_readable_bytes(incoming))

if __name__ == '__main__':
    main()
