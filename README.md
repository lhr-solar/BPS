# Battery Protection System Code
This repo contains all code related to UTSVT's Battery Protection System

## Setup
There are two options for developing the BPS system. One option is to use the
Keil IDE, which includes some basic debugging tools. The other option is to
use a terminal in a linux environment to build and flash the program.

### Setup for Keil Development
There is a tool called Keil that ARM produces for development on its microcontrollers.
Download the most recent version of the Keil IDE: https://www.keil.com/demo/eval/arm.htm

### Setup for Terminal Development
The system can also be built and deployed from a terminal, allowing you to use different
development tools than Keil.

1. Ensure that you either have Ubuntu installed or have the windows subsystem for linux installed.
2. Install the `gcc-arm-none-eabi` package via `sudo apt install gcc-arm-none-eabi`.
3. Go the the directory with the `Makefile` via your terminal.
4. Type `make`. If the thing does not compile, try a `make clean` and then repeat `make`.
5. Celebrate your independence from Keil.

In order to compile a specific test case, you can run the `solar-build` script with the name of the
test case. For example, `./solar-build CAN_TEST_2`.

## Rules
Commit frequently into your own branches. Create a Pull Request whenever you are ready to add 
your working code to the master branch. You must select at least 1 reviewer for approval. 
Follow the coding guidelines in the Solar Google Drive. The reviewers will make sure everything 
is up to par with the coding standards. Expect them to ask you about parts of your code if they
need clarification on anything.

Reviewers:
1. Sijin Woo
2. Chase Block
3. Clark Poon
