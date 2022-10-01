********
Overview
********

New version of the simulator. This one was designed with the intention that it would be easier to maintain than the old one, 
easier to create, and designed for running unit tests. It was not designed to be a completely faithful simulation of the BPS. 
I made a conscious decision to trade off fidelity to get better development time and implementation simplicity. It runs on the 
version of Micrium that we used for the previous simulator. A quick overview of it is:

Simulating Output
=================
Whenever the BPS sends output to a peripheral, it will be simulated by logging a record to a log file. This log file can be 
parsed after the test is complete to determine if it was successful.

Simulating Input
================
JSON files can be used to define test cases. At startup, the simulator will read the JSON file and create a linked list of 
states that it will use to return fake data to the BSP layer whenever the BPS tries to read inputs. Each state has an associated 
time and will transition to the next state when the current state's time has run out. After the last state has completed, the 
simulator will shut down. Right now, state transitions only happen when a call into the simulator is made. I chose to use JSON 
because it is a common file format and many languages have libraries for it, so it should be relatively straightforward to create 
scripts/programs for generating test cases. For example JSON files, see https://github.com/lhr-solar/BPS/tree/simulator_v2/BSP/Simulator_V2/Data

What to Simulate
================
In the previous simulator, the well-defined boundary between the simulator and the non-simulated code was that the BSP was part 
of the simulator and the Driver layer and above was (mainly) unmodified. This forced the simulator to maintain compatibility with 
the Driver layer, which was expensive (especially for the LTC6811). To solve this problem, the new simulator implements it's own 
version of the BSP layer, but it also is allowed to modify the layers above it. This can be done safely by using the preprocessor 
to check if SIMULATION is defined to decide which parts of the upper layers are used, which allows the simulator to bypass the LTC6811 
driver adn deal with a higher layer of abstraction (voltage and temperature readings) instead of a more complex lower layer (LTC6811 configuration)

Compiling the Simulator
=======================
Right now the simulator can be compiled with ``make simulator_v2``. The simulator requires an additional ``INPUT=`` command line argument to 
specific the path to the JSON file for the test case to run. I chose to define the test at compile time instead of run time because I wanted 
to avoid making changes to main() to add command line arguemnts when I started this, but I am open to revisiting this if we decide a run-time 
option would be better. The ``TEST=`` option is also supported for running custom test cases.

Running the Simulator
=====================
The simulator can be run with ``./bps-simulator.out``

Checking tests
==============
This is not implemented yet, but I would like to create a script/program for parsing the simulator output files to determine the ending state 
of the simulator's outputs. We can then compare the final state with the expected state (define by the creator of the test case) to determine 
if the test passed or failed. This would allow us to script the testing process and integrate it with GitHub actions to run regression tests on 
all of our pull requests.

Some things (like current, CAN, and automated test checking) are not supported yet, so maybe we want to wait a little bit before merging with 
master? I also haven't written any serious test cases yet.
