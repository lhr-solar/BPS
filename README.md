# Battery Protection System Code
This repo contains all code related to UTSVT's Battery Protection System

## Setup
There are two options for developing the BPS system. One option is to use the Keil IDE, which includes some basic debugging tools. The other option is to use a terminal in a linux environment to build and flash the program.

### Setup for Keil Development
There is a tool called Keil that ARM produces for development on its microcontrollers. Download the most recent version of the Keil IDE: https://www.keil.com/demo/eval/arm.htm

### Setup for Terminal Development
The system can also be built and deployed from a terminal, allowing you to use different development tools than Keil.
1. Ensure that you have some type of linux machine such as Ubuntu, Windows Subsystem for Linux, etc. Windows will not work.
2. The BPS code supports multiple microcontrollers and different architectures. Depending on which on you're using, make sure you have the correct toolchain.
    - STM32F413: Follow the instructions in this [repo](https://github.com/SijWoo/ARM-Toolchain-Setup) to setup the toolchains for the current BPS.
    - Simulator: Install gcc and gdb using the following lines
        ```
        sudo apt install gcc gdb
        ```
3. Download [VSCode](https://code.visualstudio.com/)
4. In VSCode, download the following extensions (Click on the block icon on the left column):
    - C/C++
    - ARM
    - cortex-debug
5. Go the the directory with the Makefile via your terminal.
6. Type make. If the thing does not compile, try a make clean and then repeat make.
7. Celebrate your independence from Keil.

## Rules
Commit frequently into your own branches. Create a Pull Request whenever you are ready to add you working code to the master branch. You must select 1 reviewer for approval. Follow the coding guidelines in the Solar Google Drive. The reviewers will make sure everything is up to par with the coding standards.

Reviewers: (More will be added)
1. Sijin Woo
2. Chase Block
3. Clark Poon
4. Rishi Ponnekanti
