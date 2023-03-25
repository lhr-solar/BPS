# Battery Protection System Code
This repo contains all code related to LHR Solar's Battery Protection System (BPS)
The [Documentation](https://bps.readthedocs.io/en/latest/) for the BPS contains other useful software and an in depth explanation of our architecture.

NOTE: As of 2023, the simulator for the BPS repository is deprecated.

## Setup
Development is done in a Linux environment to build and flash the BPS program.

### Cloning the Repo
Cloning repos onto your machine is usually straightforward, but our RTOS project depend on uCOS-III-Simulator and uCOS-III-STM32F4 repos. Extra steps must be taken to correctly pull submodules. [Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) are repos that are inside a repo.

The most common way to clone repos is to use the HTTPS link, but the most secure way is to clone with SSH. The submodules use SSH, so you must generate and add an SSH key to your GitHub account. Follow the instructions in this tutorial. If this is your first time doing this, you can start at the [Generating a new SSH key and adding it to the ssh-agent](https://docs.github.com/en/github/authenticating-to-github/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) step, but reading everything is beneficial.

Once you're able to use Git with SSH, go to the green "Code" button of the repo, and click "Use SSH" to get the URL. Copy the `git@hithub.com:repo-name` link. Then, enter the following command to the terminal but with the correct URL:

```
git clone --recurse-submodules git@github.com:repo-name
```
If you already cloned the repo but don't have the submodules, enter the following commands:

```
git submodule init
git submodule update --remote
```

### Setup for Terminal Development
The BPS is designed be built and deployed from a Linux terminal, allowing you to use your choice of text editor/IDE.
1. Ensure that you have some type of Linux machine such as Ubuntu, Windows Subsystem for Linux, etc. Windows will not work.
    - While you can compile the code on WSL, you will not be able to flash the code unless it is through a Linux machine (VM, dual boot, etc.)
2. The BPS code supports multiple microcontrollers and different architectures. Depending on which on you're using, make sure you have the correct toolchain.
    - STM32F413: Follow the instructions in this [repo](https://github.com/SijWoo/ARM-Toolchain-Setup) to setup the toolchains for the current BPS.
    - Simulator: Install GCC and GDB using the following lines
        ```
        sudo apt install gcc gdb
        ```
3. (Optional) Download [VSCode](https://code.visualstudio.com/)
4. (Optional) In VSCode, download the following extensions (Click on the block icon on the left column):
    - C/C++
    - ARM
    - cortex-debug
5. OPTIONAL: If you are testing on the simulator, you must add the following line in `/etc/security/limits.conf` then restart your machine:
    ```
    <username>             -      rtprio          unlimited
    ```
    The tabbing MUST MATCH the other dashes at the bottom of the file, otherwise this will not work!

## Building
When calling any of the following commands, make sure you are in the top level of the repo.

Call ```make bsp_type``` to compile the release version of the code. ```bsp_type``` is the system you want to compile for. It can either be
```stm32f4132``` or ```simulator```. The stm32f413 version is selected by default.

Call ```make help``` for more information on how to build the project.

Call ```make clean``` if the build fails for any reason other than syntax related.

The BPS has an RTOS version and a bare metal version for its BSP files. To select which one to build, pass either `RTOS` or `BAREMETAL` to the `OS` argument. For example
    ```
    make simulator OS=BAREMETAL
    ```
will build the bare metal version, and  
    ```
    make simulator OS=RTOS
    ```
will build the RTOS version. The RTOS version is selected by default.

For testing, please read the Testing section.

### How to build a test
To build a new test, you need to use the following command:
```make bsp_type TEST=x```

- ```x``` specifies which test you want to compile. TEST= is optional and only required if a test is to be compiled. Set TEST equal to the suffix of the Test_ Src files i.e. if the test you want to run is in Test_x.c, set TEST=x.
    E.g. Call ```make stm32f413 TEST=Voltage``` if you want to test Voltage.c with the Test_Voltage.c src file

## Flashing
When calling any of the following commands, make sure you are in the top most level of the directory.
You must be on a Linux VM or dual boot machine. Consider using [VMWARE](https://kb.vmware.com/s/article/2057907) 
using this free license [key](https://gist.github.com/williamgh2019/cc2ad94cc18cb930a0aab42ed8d39e6f)

1. Install `st-util`
2. Connect your computer to the BPS using a Nucleo programmer
3. Call `make flash` to flash the most recently built BPS code. You may need to run this with `sudo`.

## Testing
The following testing information is specifically for terminal development.

### Rules for making a new test Src file
1. Test names: The formatting of the file names is crucial to have the Makefile work properly. "Test_" should be the prefix and the src file name that is to be tested must follow i.e. if you want to test x.c, the test Src file must be named Test_x.c or the Makefile will not be able to find the file.
    E.g. A test for Voltage.c should be Test_Voltage.c file, a test for BSP_SPI.c should be named Test_BSP_SPI.c
2. All test files should be placed into the Tests folder.

### Using GDB with OPENOCD
OpenOCD is a debugger program that is open source and compatible with the STM32F413. 
GDB is a debugger program that can be used to step through a program as it is being run on the board. To use, you need two terminals open, as well as a USB connection to the ST-Link programmer (as if you were going to flash the program to the board). 
1. Run ```./openocd-debug.sh``` in one terminal.
2. In the other terminal, start gdb with the command ```gdb-multiarch ./Objects/bps-leader.elf``` (assuming that you are doing this in the root of the project directory.
3. This will launch GDB and read in all of the symbols from the program that you are running on the board. In order to actually connect gdb to the board, execute the command ```target extended-remote localhost:4242```, which will connect to the st-util command from earlier.

**Note:** If you get an error message for Permission denied, try giving openocd read/write permissions using chmod: ```chmod 764 openocd```

https://linuxcommand.org/lc3_lts0090.php

### Using the Simulator
The simulator reads input from a JSON file to simulate dummy values and dumps output to a log file to simulate controlling the BPS's peripherals. The goal is to create repeatable tests that can be checked automatically by using the same test file, and by parsing the log file.

The simulator can be compiled with `make simulator`. The simulator also has an optional command line argument `TEST=` to specify the name of the test file to run. 

Note: when typing name it, don't type the `Test_` part or the `.c` part.

Example: `make simulator TEST=SimulatorInit`.

The simulator can be run with `./bps-simulator.out <path to JSON test>`. However, in order to run properly, `ulimit -r` must be set from within a script. In order to debug, edit the `test.sh` file
to either debug, or just run a specific test.

You can check the log file by doing `cat bps-sim-<logged time>.log`.

## Rules
Make sure to have fun!

Commit frequently into your own branches. Create a Pull Request whenever you are ready to add you working code to the master branch. Make sure that your code compiles without any errors or warnings before you open a pull request. At least one approver must approve your pull request before it can be merged. The reviewers will make sure everything is up to par with the coding standards.

### Code Reviews
You can review a pull request even if you are not an approver. In addition to helping us catch bugs, code reviews are a good opportunity to learn more about the BPS codebase. Some examples on what you can include in a code review are:
- Ask why the author wrote a certain part of code in a way they did
- Point out potential bugs in the code
- Point out that part of the code is not well documented or hard to read

You can either approve, comment, or request changes at the end of your pull request. These are the scenarios in which I would use each option:
- **Approve:** You are signing off that this pull request is ready to merge. You and the pull request author "own" this part of the code now.
- **Request Changes:** You have found problems with the code that must be addressed before it can be merged. Note that this will block the pull request from merging until you re-review the code.
- **Comment:** Do this if you do not want to explicitly approve or request changes.

Approvers:
1. Manthan Upadhyaya
2. Clark Poon
3. Tianda Huang
