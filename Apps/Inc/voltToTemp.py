# Copyright (c) 2018-2022 UT Longhorn Racing Solar
import pandas as pd

# script for generating VoltageToTemp.h
# LMT87_LookUpTable.csv can be downloaded from https://www.ti.com/lit/zip/snic009

UNKNOWN = 10**9

def interpolate(idx, predecessor, successor):
    delta = idx - predecessor[0]
    slope = (successor[1] - predecessor[1]) / (successor[0] - predecessor[0])
    return int(delta * slope + predecessor[1])

def main():
    # read in TI's lookup table
    df = pd.read_csv("LMT87_LookUpTable.csv")
    partial_table = {}
    for row in range(len(df)):
        partial_table[int(df["millivolts"][row])] = int(df["temperature"][row])
    # populate our table with the values TI gives us
    table = [UNKNOWN] * 3301
    for val in range(0, 3301):
        if val in partial_table.keys():
            table[val] = partial_table[val] * 1000
    # interpolate the rest of the values
    predecessor = (UNKNOWN, UNKNOWN)
    successor = (UNKNOWN, UNKNOWN)
    vals_to_calc = []
    for idx, val in enumerate(table):
        if val == UNKNOWN:
            vals_to_calc.append(idx)
        else:
            if successor[0] != UNKNOWN:
                predecessor = successor
            successor = (idx, val)
            if predecessor[0] != UNKNOWN:
                while len(vals_to_calc) > 0:
                    update_idx = vals_to_calc.pop()
                    print("interpolating mV = " + str(update_idx))
                    table[update_idx] = interpolate(update_idx, predecessor, successor)
    # interpolate ending values
    while len(vals_to_calc) > 0:
                    update_idx = vals_to_calc.pop()
                    print("interpolating mV = " + str(update_idx))
                    table[update_idx] = interpolate(update_idx, predecessor, successor)
    # write the lookup table to a file as a C array
    with open("VoltageToTemp.h", "w") as file:
        file.write("const int32_t voltToTemp[] = {\n")
        for val in table:
            file.write("    " + str(int(val)) + ",\n")
        file.write("};\n")

if __name__ == "__main__":
    main()