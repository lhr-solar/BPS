"""verify_test.py

Verifies that the output of the simulator matches expected
behavior and does not reach any forbidden states
"""
import argparse
import json
import os

from typing import Dict, List
from parse_simulator import parse, LOG_FILEPATH, PERIPHERALS

LOG_PREFIX: str = "BSP/Simulator/Simulator-Out/bps-sim-"
PERIPHERALS_MULTIPLE: List[str] = [
    "Light",
    "Fan",
    "Contactor"
]

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("test_name", help="Name of the test to verify")
    return parser.parse_args()

def parse_output(out_file: str) -> str:
    # Read JSON from out_file
    with open(out_file, "r") as f:
        out_json = json.load(f)
    return out_json

def verify_forbidden_states(forbidden_states: Dict[str, object], bps_state) -> bool:
    # Check if any forbidden states were reached
    for peripheral in PERIPHERALS:
        if peripheral in PERIPHERALS_MULTIPLE:
            for state in bps_state[peripheral]:
                if peripheral in forbidden_states:
                    for forbidden_state in forbidden_states[peripheral]:
                        if forbidden_state == state:
                            print("ERROR: " + peripheral + " reached forbidden state " + str(forbidden_state))
                            return False
        else:
            if peripheral in forbidden_states:
                for state in bps_state[peripheral]:
                    if forbidden_states[peripheral] in state:
                        print("ERROR: " + peripheral + " reached forbidden state " + forbidden_states[peripheral])
                        return False
    return True

def verify_end_state(end_state: Dict[str, object], bps_state) -> bool:
    # Check if the end state matches the expected end state
    verified = True
    for peripheral in PERIPHERALS:
        if peripheral in PERIPHERALS_MULTIPLE and peripheral in end_state:
            for end_sub_state in end_state[peripheral]:
                for state in reversed(bps_state[peripheral]):
                    if end_sub_state[0] == state[0]:
                        if end_sub_state == state:
                            break
                        else:
                            print("ERROR: " + peripheral + " did not reach expected end state " + str(end_sub_state))
                            verified = False
        else:
            if peripheral in end_state:
                if end_state[peripheral] not in bps_state[peripheral][-1]:
                    print("ERROR: " + peripheral + " did not reach end state " + str(end_state[peripheral]))
                    verified = False
    return verified

def verify_init_before_use(bps_state: Dict[str, List[List[str]]]) -> bool:
    # Check that all peripherals were initialized before use
    for peripheral in PERIPHERALS:
        if len(bps_state[peripheral]) > 0 and bps_state[peripheral][0] != ["init"]:
            print("ERROR: " + peripheral + " used before initialization")
            return False
    return True

def verify_output(out_json: str, bps_state: Dict[str, List[List[str]]]) -> bool:
    # Check that the output matches the expected behavior
    verified: bool = True
    verified = verify_forbidden_states(out_json["forbidden_states"], bps_state) and verified
    verified = verify_init_before_use(bps_state) and verified
    verified = verify_end_state(out_json["end_state"], bps_state) and verified
    return verified

def main():
    args = parse_args()
    test_name = args.test_name
    out_filepath = test_name[:test_name.rfind('.')]
    out_filepath = out_filepath + "-out.json"

    #seperate out the actual file name
    log_filepath = test_name[test_name.rfind('/')+1:]
    log_filepath = LOG_FILEPATH + log_filepath + ".log"
    
    #run the simulator 
    print("Running test " + test_name)
    if(os.WEXITSTATUS(os.system("./bps-simulator.out "+ test_name)) != 0):
        print("Your code broke the simulator. Test Failed")
        exit(-1)

    #run the verification
    bps_state = parse(log_filepath)
    out_json = parse_output(out_filepath)
    if verify_output(out_json, bps_state):
        print("Test passed")
        exit(0)
    else:
        print("Test failed")
        exit(-1)
    
if __name__ == '__main__':
    main()
