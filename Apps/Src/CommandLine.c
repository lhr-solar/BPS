// #include <stdbool.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>

// #include "BSP_UART.h"
// #include "CANbus.h"
// #include "Contactor.h"
// // #include "Minions.h"
// // #include "Pedals.h"
// #include "Tasks.h"
// #include "os.h"

// #define MAX_BUFFER_SIZE 128  // defined from BSP_UART_Read function

// #define CAN_DIAG_ID 0x0582

// #define TAB 0x09
// #define LF 0x0A
// #define FF 0x0C
// #define CR 0x0D
// #define SPACE 0x20

// // Represents a command that the command line understands
// struct Command {
//     const char *name;
//     bool (*action)(void);
// };

// static bool cmdHelp(void);
// static bool cmdCanBusSend(void);
// static bool cmdCanBusRead(void);
// static bool cmdContactorsGet(void);
// static bool cmdContactorsSet(void);


// const struct Command kCmdlineCommands[] = {
//     {.name = "help", .action = cmdHelp},
//     {.name = "CANbus_Send", .action = cmdCanBusSend},
//     {.name = "CANbus_Read", .action = cmdCanBusRead},
//     {.name = "Contactors_Get", .action = cmdContactorsGet},
//     {.name = "Contactors_Set", .action = cmdContactorsSet},
//     {.name = NULL, .action = NULL}};

// static char input[MAX_BUFFER_SIZE];
// char *save;  // Save pointer for strtok_r

// char *help = {
//     "LHRS Controls Command Line:\n\r"
//     "	For help, enter [help]\n\r"
//     "	Format is: cmd [param, ...]\n\r"
//     "	Commands and their params are as follows:\n\r"
//     "	CANbus_Send (non)blocking motor/car 'string' - Sends a CAN\n\r"
//     "message with the string data as is on the determined line\n\r"
//     "	CANbus_Read (non)blocking motor/car - Reads a CAN message\n\r"
//     "on the detemined line\n\r"
//     "	Contactors_Get array_c/array_p/motor_c - Gets the status of\n\r"
//     "determined contactor\n\r"
//     "	Contactors_Set array_c/array_p/motor_c on/off (non)blocking -\n\r"
//     "Sets the determined contactor\n\r"
//     "	Contactors_Enable  array_c/array_p/motor_c - Enables the determined\n\r"
//     "contactor\n\r"
//     "	Contactors_Disable  array_c/array_p/motor_c - Disables the "
//     "determined\n\r"
//     "contactor\n\r"};

// static inline bool isWhiteSpace(char character) {
//     switch (character) {
//         case TAB:
//         case LF:
//         case FF:
//         case CR:
//         case SPACE:
//             return true;
//         default:
//             return false;
//     }
// }

// static bool executeCommand(char *input) {
//     // The first word in the input should be a valid command
//     char *command = strtok_r(input, " ", &save);
//     // Iterate through all valid commands and check if the input matches (exits
//     // if the action is NULL (acts as a sentinal))
//     for (int i = 0; kCmdlineCommands[i].action; i++) {
//         if (!strcmp(command, kCmdlineCommands[i].name)) {
//             return kCmdlineCommands[i].action();  // Execute the command
//         }
//     }
//     return false;  // Didn't find a valid command!
// }

// // *********** Command line (shell) ************
// void TaskCommandLine(void *p_arg) {
//     OS_ERR err = 0;

//     // output welcome/help screen
//     printf("%s", help);

//     while (1) {
//         printf("> ");
//         BSP_UART_ReadLine(input, UART_USB);
//         printf("\n\r");

//         if (!executeCommand(input)) {  // If command failed, error
//             printf("Bad cmd. Please try again\n\r");
//         }
//     }

//     // Delay of 1 seconds
//     OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);
//     if (err != OS_ERR_NONE) {
//         assertOSError(err);
//     }
// }

// // Function Implementations
// // ------------------------------------------------------------------------

// static inline bool cmdHelp(void) {
//     printf("%s", help);
//     return true;
// }

// /* This has not been tested and not sure if this implementation is correct (not
// sure if we want to just transmit string data from serial) */
// static bool cmdCanBusSend(void) {
//     char *data = strtok_r(NULL, " ", &save);
//     CANPayload_t msg = {
//         .id = CAN_DIAG_ID,
//         .idx =
//             0};  // this would change in the future (don't assume char as data)
//     for (int i = 0; i < BSP_CAN_DATA_LENGTH && i < strlen(data); i++) {
//         msg.data[i] = data[i];
//     }

//     char *block_input = strtok_r(NULL, " ", &save);
//     bool blocking = false;
//     if (strcmp(block_input, "blocking") == 0) {
//         blocking = CAN_BLOCKING;
//     } else if (strcmp(block_input, "nonblocking") == 0) {
//         blocking = CAN_NON_BLOCKING;
//     } else {
//         return false;
//     }

//     char *bus_input = strtok_r(NULL, " ", &save);
//     Can bus = 0;
//     if (strcmp(bus_input, "motor") == 0) {
//         bus = kCan1;
//     } else if (strcmp(bus_input, "car") == 0) {
//         bus = kCan3;
//     } else {
//         return false;
//     }

//     if (CanBusSend(msg, blocking, bus)) {
//         printf("msg sent on %s (%s)\n\r", "can", block_input);
//     } else {
//         printf("msg sent failed\n\r");
//     }
//     return true;
// }

// static bool cmdCanBusRead(void) {
//     CanData msg;

//     char *block_input = strtok_r(NULL, " ", &save);
//     bool blocking = false;
//     if (strcmp(block_input, "blocking") == 0) {
//         blocking = CAN_BLOCKING;
//     } else if (strcmp(block_input, "nonblocking") == 0) {
//         blocking = CAN_NON_BLOCKING;
//     } else {
//         return false;
//     }

//     char *bus_input = strtok_r(NULL, " ", &save);
//     Can bus = 0;
//     if (strcmp(bus_input, "motor") == 0) {
//         bus = kCan1;
//     } else if (strcmp(bus_input, "car") == 0) {
//         bus = kCan3;
//     } else {
//         return false;
//     }

//     if (CanBusRead(&msg, blocking, bus) == SUCCESS) {
//         printf("msg received on %s (%s)\n\r", bus_input, block_input);
//         printf("ID: %d, Data: ", msg.id);
//         for (int i = 0; i < CAN_DIAG_ID; i++) {
//             printf("[%d] %x \n\r", i, msg.data[i]);
//         }
//     } else {
//         printf("read failed on %s (%s)\n\r", bus_input, block_input);
//     }
//     return true;
// }

// static bool cmdContactorsGet(void) {
//     char *contactor_input = strtok_r(NULL, " ", &save);
//     Contactor contactor = 0;
//     if (strcmp(contactor_input, "array_p") == 0) {
//         contactor = kArrayPrechargeBypassContactor;
//     } else if (strcmp(contactor_input, "motor_p") == 0) {
//         contactor = kMotorControllerPrechargeBypassContactor;
//     } else {
//         return false;
//     }

//     printf("%s state: %s\n\r", contactor_input,
//            ContactorsGet(contactor) == ON ? "on" : "off");
//     return true;
// }

// static bool cmdContactorsSet(void) {
//     char *contactor_input = strtok_r(NULL, " ", &save);
//     Contactor contactor = 0;
//     if (strcmp(contactor_input, "array_p") == 0) {
//         contactor = kArrayPrechargeBypassContactor;
//     } else if (strcmp(contactor_input, "motor_p") == 0) {
//         contactor = kMotorControllerPrechargeBypassContactor;
//     } else {
//         return false;
//     }

//     char *state_input = strtok_r(NULL, " ", &save);
//     bool state = false;
//     if (strcmp(state_input, "on") == 0) {
//         state = true;
//     } else if (strcmp(state_input, "off") == 0) {
//         state = false;
//     } else {
//         return false;
//     }

//     char *blocking_input = strtok_r(NULL, " ", &save);
//     bool blocking = false;
//     if (strcmp(blocking_input, "blocking") == 0) {
//         blocking = true;
//     } else if (strcmp(blocking_input, "nonblocking") == 0) {
//         blocking = false;
//     } else {
//         return false;
//     }

//     if (ContactorsSet(contactor, state, blocking)) {
//         printf("%s set to %s (%s)\n\r", contactor_input, state_input,
//                blocking_input);
//     } else {
//         printf("set failed\n\r");
//     }
//     return true;
// }




