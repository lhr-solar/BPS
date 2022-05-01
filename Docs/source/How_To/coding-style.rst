*************
Coding Style
*************

.. note:: 

    | CamelCase - FirstWordSecondWord
    | camelCase - firstWordSecondWord

Macros and Constants
====================

Macros and constants should be written in snakecase with all caps. If a macro utilizes arithmetic, it should be covered in parentheses to prevent the compiler
from assuming priority. Ex

.. code-block:: c

    #define ALL_CAPS_WITH_UNDERSCORE
    #define MATH (4 + x)
    const uint8_t FOUR_KG = 4;
    #ifdef TASKS_H //No leading underscores

Enums
=====

Enum names should be in CamelCase and all elements in enums should be in all caps. They should be located in the header file. Ex:

.. code-block:: c

    enum ExampleEnum {
        ELEMENT_ONE;
        ELEMENT_TWO;
        ELEMENT_THREE;
        };

Structs
=======

All struct typedef names should have a descriptive name written in CamelCase and an appended ``_t``. The name of the struct should be
in CamelCase and its members should be written in camelCase to make it easier to understand gcc error messages. 
They should be located in the header file. Ex:

.. code-block:: c

    typedef struct StructName {
        uint8_t element;
        uint16_t elementTwo;
    } StructName_t;

Unions
======

Unions should only be used when absolutely necessary (normally for communication protocols). They should be located in the
header file, written in CamelCase, and appended with an ``_t``. Members of the union should be in camelCase.

.. code-block:: c

    typedef union {
	    uint8_t b;
	    uint16_t h;
	    uint32_t w;
	    float f;
    } CANData_t;

Libraries
=========

All libraries are separated into source files and header files. The source file consists of the source code and the header file
holds the function declarations for the library. 

=============
Library Names
=============

The name of the library depends on the layer that the library is part of. 

Board Support Package (BSP)
---------------------------

BSP library file names should describe the feature of the microcontroller that is being used. They should all be in 
snakecase with the first section being ``BSP_``. If the BSP library is for specific functionality in the system, the name should
describe the functionality. Ex:

.. code-block:: c

    BSP_ADC.c //Microcontroller Feature
    BSP_ADC.h
    BSP_Fans.c //Interface with system
    BSP_Fans.h

Drivers
-------

Driver library file names should be the name of the component the library is for with the appropriate suffix. Ex:

.. code-block:: c

    Partname.c
    Partname.h
    LTC6811.c
    LTC6811.h

Application
-----------

Application level library names should describe the functionality of the application. If the application library is for a task,
it should be prefaced with the ``Task_`` prefix. the tasks name should be in CamelCase. Ex:

.. code-block:: c 

    EEProm.c //interface with EEPROM
    Temperature.c
    Task_TaskName.c

============
Source Files
============

Source files should be formatted in the following method.

    1. Trademark/Description of file
    2. Includes
    3. Defines
    4. Globals
    5. Static Functions
    6. Public Functions
    7. Newline

The trademark/description should adhere to the following format:

.. code-block:: c

    /* Copyright (c) <YEAR> UT Longhorn Racing Solar */
    /** DRIVER.c
    * DESCRIPTION
    */

Included files should only be what is necessary for the source code to run. Defines should follow Macro coding style.
If there is debugging functionality in the source file (that is not essential for the system to run), ``#ifdef`` and ``endif``
should surround all code necessary for that functionality. This consists of included files, ``printf`` statements, and other code
inside functions that run at regular runtime. Debugging functions do not have to be surrounded. Static functions must be declare
before all public functions. It is the prerogative of the programmer as to have the static function definition at the start of the 
source file or at the end.

============
Header Files
============

Header files should be formatted in the following method.

    1. Trademark/Description of file
    2. Defines
    3. Includes
    4. Typedefs
    5. Function Declarations
    6. Endifs
    7. Newline

All header files should be surrounded with ``#ifndef`` and ``#endif``.
Header files should only have amount of includes necessary for header file to work. For example, if a function returns an 
``int``, then the header file should ``#include <stdint.h>``. However, if the source file calls ``memcpy`` in a function, it is not
necessary for the header file to ``#include <stdlib.h>``.

Functions
=========

============
Descriptions
============

All function declarations and definitions should have a comment paragraph description that follows the specified format.::

    /**
     * NOTE: Include anything important someone else will need to know
     * @brief Give Description of Function
     * @param variable Describe input parameters
     * @return What function returns
     */

================
Public Functions
================

The first section should be the layer that the function resides in. The second section should
be separated by snakecase and have the library the function is a part of. The third section should
also be separated by snakecase and describe the functions purpose. If there are multiple words in
the third section they should be written in CamelCase. Ex

.. code-block:: c

    Layer_Library_Function();
    BSP_Contactor_GetState();

Descriptions for public functions should be included above the function definition in the source
file and above the declaration in the header file.

=================
Private Functions
=================

Code duplication should be avoided by using private functions instead. Their format is the same as public functions but 
do not need to have the layer section in their name. Their description should be included in the source file, not the header file.

.. code-block:: c

    /**
     * NOTE: Include anything important someone else will need to know
     * @brief Give Description of Function
     * @param variable Describe input parameters
     * @return What function returns
     */
    static void ADC_InitDMA(void);

Variables
=========

================
Local Variables
================

If variables have the same type, declare them on the same line if possible. 
Meaningful names for variables that aren't used for iterations in loop. Should be written in camelCase.
Unnecessary variables should be avoided. i.e

.. code-block:: c
    :linenos:

    //NO BUENO
    int x = function();
    if (x) return;
    //YES BUENO
    if (function()) return;

If the functionality of the code is affected by this (calling the same function multiple times), then
it is fine to have a variable for the return value.

================
Global Variables
================

Variable names should be meaningful and written in CamelCase. Efforts must be made to prevent global variables from
being used in multiple libraries. All global variables must be static variables unless the logic requires global variables
being shared. All global variables should have comments describing their purpose (what they hold). Ex.

.. code-block:: c
    :linenos:

    static OS_MUTEX VoltageMutex;
    static uint16_t VoltageVal[NUM_BATTERY_MODULES]; //Voltage values gathered

Miscellaneous
=============

============
Punctuation
============

**Curly Braces:** Opening braces should be on the same line as the condition or function that contains them. Curly braces should always 
be used in conditionals even if it is one line
**Parentheses:** Should be used for clarifying order of precedence.
**Indentation:** Indentation should be used in all conditionals, iteratives, and functions. Indentation should be 4 spaces (NOT TABS).
**Line Endings:** All End Of Line sequences should be LF(line feed). This prevents files from looking like they are changed when
nothing was changed at all.

===============
Common Practice
===============

**Pointers:** Members to pointers should be accessed through ``p->member`` operator instead of ``(p).member``.
**Indentation:** Tabs should be 4 spaces. If a pull request is made and changes are made to files you did not edit, check to see
if your editor is editing whitespace when opening files (for MAC users). If these issues are not fixed, your PR WILL NOT BE MERGED
