# Battery Protection System Code
This repo contains all code related to LHR Solar's Battery Protection System (BPS)

## Setup
The BPS is designed be built and deployed from a linux terminal, allowing you to use your choice of text editor/IDE.
1. Ensure that you have some type of linux machine such as Ubuntu, Windows Subsystem for Linux, etc. Windows will not work.
2. The BPS code supports multiple microcontrollers and different architectures. Depending on which on you're using, make sure you have the correct toolchain.
    - STM32F413: Follow the instructions in this [repo](https://github.com/SijWoo/ARM-Toolchain-Setup) to setup the toolchains for the current BPS.
    - Simulator: Install gcc and gdb using the following lines
        ```
        sudo apt install gcc gdb
        ```
3. (Optional) Download [VSCode](https://code.visualstudio.com/)
4. (Optional) In VSCode, download the following extensions (Click on the block icon on the left column):
    - C/C++
    - ARM
    - cortex-debug

## Building
When calling any of the following commands, make sure you are in the top most level of the directory.

Call ```make bsp_type``` to compile the release version of the code. ```bsp_type``` is the system you want to compile for.

Call ```make help``` for more information on how to build the project.

Call ```make clean``` if the build fails for any reason other than syntax related.

For testing, please read the Testing section.

## Flashing
When calling any of the following commands, make sure you are in the top most level of the directory.

1. Install `st-util`
2. Connect your computer to the BPS using a nucleo programmer
3. Call `make flash` to flash the most recently built BPS code. You may need to run this with `sudo`.

## Testing
The following testing information is speficially for terminal development.

### Rules for making a new test src file
1. Test names: The formatting of the file names is crucial to have the makefile work properly. "Test_" should be the prefix and the src file name that is to be tested must follow i.e. if you want to test x.c, the test src file must be named Test_x.c or the makefile will not be able to find the file.
    E.g. A test for Voltage.c should be Test_Voltage.c file, a test for BSP_SPI.c should be named Test_BSP_SPI.c
2. All test files should be placed into the Tests folder.

### How to build a test
To build a new test, you need to use the following command:
```make bsp_type TEST=x```

- ```bsp_type``` specifies which system you want to compile the code for: ```stm32f413```, ```simulator```
- ```x``` specifies which test you want to compile. TEST= is optional and only required if a test is to be compiled. Set TEST equal to the suffix of the Test_ src files i.e. if the test you want to run is in Test_x.c, set TEST=x.
    E.g. Call ```make stm32f413 TEST=Voltage``` if you want to test Voltage.c with the Test_Voltage.c src file

## Rules
Make sure to have fun!

Commit frequently into your own branches. Create a Pull Request whenever you are ready to add you working code to the master branch. Make sure that your code compiles without any errors or warnings before you open a pull request. You must select 1 reviewer for approval. Follow the coding guidelines in the Solar Google Drive. The reviewers will make sure everything is up to par with the coding standards.

Reviewers:
1. Sijin Woo
2. Chase Block
3. Clark Poon
4. Rishi Ponnekanti
5. Manthan Upadhyaya
6. Sugam Arora
7. Matthew Yu
