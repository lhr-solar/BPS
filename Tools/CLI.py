#!/usr/bin/env python3

import serial
import prompt_toolkit as pr
import multiprocessing as mp

"""
TODO: separate threads/processes for
        serial io
        recv parsing/graphing
        user cli
"""

class CommandLineInterface():

    # base ASCII value we will encode numbers from; range of encodable numbers is 0 to 94; 0 maps to 33 and 94 maps to 127
    # TODO: put these constant tables in json to be more humanly readable
    OUTPUT_BASE_VALUE = 33
    OUTPUT_DICT = { # note: try to avoid changing these values as the C code will also need to be changed
        'volts':0,
        'amps':1,
        'temperature':2,
        'charge':3,
        'display':4,
        'watchdog':5,
        'eeprom':6,
        'adc':7,
        'openwire':8,
        'fault':9,
        'all':10,
        'critical':11,
        'can':12,
        'ltc':13,
        'contactor':14,
        'ping':15,
        'error':16,
        'exit':17
    }
    OTHER_COMMANDS = [
        'help'
    ]

    def __init__(self, loopback:bool, port:str):
        self.port = port
        self.auto_complete = pr.completion.WordCompleter(
                list(self.OUTPUT_DICT.keys())+self.OTHER_COMMANDS, ignore_case=True)
        self.serial = serial.serial_for_url('loop://', timeout=1) if loopback else (
                      serial.Serial(port=self.port, 
                                    baudrate=115200, 
                                    bytesize=8, 
                                    parity='N', 
                                    stopbits=1, 
                                    timeout=None, 
                                    xonxoff=0, 
                                    rtscts=0))

        output_command_handler = {key:self._default_output_handler for key in self.OUTPUT_DICT.keys()}
        misc_command_handler = {'help':self.help}
        self.command_handler = {**output_command_handler, **misc_command_handler}
        
    def run(self):
        while True:
            try:
                input = pr.prompt('☀️🚗🔍 # ',
                                history=pr.history.FileHistory('cli_history.log'),
                                auto_suggest=pr.auto_suggest.AutoSuggestFromHistory(),
                                completer=self.auto_complete)
                self.command_handler[input.split(' ')[0]](input.split(' '))
            except EOFError:
                break
            except self.InvalidArgCError as e:
                print(e)

    def help(self, *args):
        print(self)

    def _write(self, data:bytearray):
        self.serial.write(data)

    def _read(self):
        return self.serial.readline()

    def _default_output_handler(self, arguments:list) -> None:
        if len(arguments) != 2: raise self.InvalidArgCError(len(arguments), 2)
        outputarray = bytearray([
                self.OUTPUT_DICT.get(arguments[0]) | 
                {'on':(1 << 8), 'off':0}.get(arguments[1]) ])
        self._write(outputarray)

    def __str__(self):
        num_rows = 3
        cmd_list = list(self.OUTPUT_DICT.keys())
        max_len = max(map(len, cmd_list))+1
        formatted_cmd_list_str = '\r\n'.join(
                [''.join(
                    map(lambda s: s.ljust(max_len, ' '), cmd_list[i:i+num_rows])
                ) for i in range(0, len(cmd_list), num_rows)])

        output = [
            '-------------------------Help Menu-------------------------',
            'The available commands are:',
            formatted_cmd_list_str,
            '-----------------------------------------------------------']
        
        return '\r\n'.join(output)

    class InvalidArgCError(Exception):
        def __init__(self, args_given:int, args_expected:int):
            self.args_given = args_given
            self.args_expected = args_expected
        def __str__(self):
            return f'Invalid Arguments: {self.args_given} argument(s) supplied; {self.args_expected} argument(s) expected.'


def main():
    # TODO: add multiprocessing
    
    cli = CommandLineInterface(True, '/dev/ttyUSB0')
    cli.run()

if __name__ == '__main__':
    main()
