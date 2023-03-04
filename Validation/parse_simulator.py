"""parse_simulator.py

This module will be used to parse the output log files
from the BPS simulator
"""
import argparse
import os
import re

from typing import Dict, List

LOG_FILEPATH: str = "BSP/Simulator/Simulator-Out/"
PERIPHERALS: List[str] = [
    "ADC",
    "CAN",
    "Contactor",
    "Fan",
    "I2C",
    "Light",
    "PLL",
    "SPI",
    "SPI3",
    "Timer",
    "UART",
    "WDTimer",
]

bps_state: Dict[str, List[List[str]]] = {peripheral: [] for peripheral in PERIPHERALS}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("log_file", help="Name of the log file to parse")
    return parser.parse_args()


def parse_line(line: str):
    if "[OUTPUT]" in line:
        for peripheral in PERIPHERALS:
            if (peripheral in line) and ('{' + peripheral + '}' not in line): # the surrounding by {} is an edge case for CAN with Lights
                values = [val[1:-1] for val in re.findall("{[^{}]*}", line)]
                bps_state[peripheral].append(values)
    elif "initialized" in line.lower():
        for peripheral in PERIPHERALS:
            if peripheral in line:
                bps_state[peripheral].append(["init"])


def print_state():
    for peripheral in PERIPHERALS:
        print(peripheral)
        for line in bps_state[peripheral]:
            print(str(line) + "->", end="")
        print("END")
        print()


def parse(log_file: str = None) -> Dict[str, List[List[str]]]:
    if log_file is None:
        args = parse_args()
        log_file = args.log_file
    with open(log_file, "r") as f:
        for line in f:
            parse_line(line)
    return bps_state
    

if __name__ == '__main__':
    _ = parse()
    print_state()
