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
Right now the simulator can be compiled with ``make simulator``. 

Running the Simulator
=====================
The simulator can be run with ``./bps-simulator.out <path to JSON input file>``

Checking Tests
==============
The python script ``Validation/verify_test.py`` can be used to verify if the test passes or fails. It takes the name
of the test as an argument. For example, if you want to run a test called ``footest`` specified in
``BSP/Simulator/Data/footest.json`` and ``BSP/Simulator/Data/footest-out.json``, you should run
``python3 Validation/verify_test.py``. This script assumes the ``<testname>.json`` and ``<testname>-out.json`` will
be in the ``BSP/Simulator/Data/`` directory and that ``./bps-simulator.out`` generated a log called
``bps-sim-<testname>.json.log`` in the current directory (usually the root of the BPS repo).

Creating Tests
==============
To add a test to regressions, create a files called ``<testname>.json`` and ``<testname>-out.json`` in 
``BSP/Simulator/Data/`` to specify the test states and requirements. Then modify the appropriate GitHub
workflow (in ``.github/workflows/``) to add your test. The shell script ``Validation/test.sh`` is used to
run the simulator on GitHub's servers with the proper conditions and must be run with sudo. For example, 
this is how the footest test is added to the simple-tests workflow in ``.github/workflows/simple-tests.yml``::

    - name: footest 
      run: |
        sudo ./Validation/test.sh footest

