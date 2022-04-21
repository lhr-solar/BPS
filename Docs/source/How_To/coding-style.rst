*************
Coding Style
*************

Macros and Constants
====================

Should be written in snakecase with all caps. If a macro utilizes arithmetic, it should be covered in parentheses to prevent compiler
from assuming priority. Ex

.. code-block:: c

    #define ALL_CAPS_WITH_UNDERSCORE
    #define math (4 + x)
    const uint8_t FOUR_KG = 4;

Enums
=====

Enum name should be in CamelCase and all element in enum should be all caps. Ex:

.. code-block:: c

    enum ExampleEnum {
        ELEMENT_ONE;
        ELEMENT_TWO;
        ELEMENT_THREE;
        };

Libraries
=========

All libraries are separated into source files and header files. The source file consists of the source code and the header file
holds the function declarations for the library. 

=============
Source Files
=============

Source files should be formatted in the following method.

    1. Trademark/Description of file
    2. Includes
    3. Defines
    4. Globals
    5. Static Functions
    6. Public Functions
    7. Newline

Included 

Functions
=========

============
Descriptions
============

All functions should have a comment paragraph description that follows the specified format.::

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

These are the same as public functions but do not need to have the layer section in their name. 
Their description should be included in the source file, not the header file.

.. code-block:: c

    static void ADC_InitDMA(void);

Variables
=========

================
Local Variables
================

If variables have the same type, declare them on the same line if possible. 
Meaningful names for variables that aren't used for iterations in loop. Should be written in camelCase.
Unecessary variables should be avoided. i.e

.. code-block:: c
    :linenos:

    //NO BUENO
    int x = function();
    if (x) return;
    //YES BUENO
    if (function()) return;

If functionality of code is affected by this (calling same function multiple time), then
it is fine to have a variable for the return value.

================
Global Variables
================

Variable names should be meaningful and written in CamelCase. Efforts must be made to prevent global variables from
being used in multiple libraries. All global variables must be static variables unless the logic requires global variables
being shared. All global variables should have comments describing their purpose (what they hold). Ex.

.. code-block:: c
    :linenos:

    static OS_MUTEX Voltage_Mutex;
    static uint16_t VoltageVal[NUM_BATTERY_MODULES]; //Voltage values gathered

Miscellaneous
=============

============
Punctuation
============

**Curly Braces:** Opening braces should be on same line as condition or function that contains them. Curly braces should always 
be used in coditional even if it is one line
**Parentheses:** Should be used for clarifying oder of precedence.

===============
Common Practice
===============

**Pointers:** Members to pointers should be accessed through ``p->member`` operator instead of ``*(p).member``.
**Indentation:** Tabs should be 4 spaces. If a pull request is made and changes are made to files you did not edit, check to see
if your editor is editing whitespace when opening files (for MAC users).
