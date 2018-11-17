/** FIFO.c
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Sijin Woo
 * @lastRevised 10/2/2018
 */

#include "FIFO.h"

// Global variables

// Public Function Definitions
/** FIFO_Init
 * Initializes empty FIFO
 */
void FIFO_Init(void){
	
}

/** FIFO_IsEmpty_Tx
 * Checks if Fifo is empty
 * @return 1 if empty, 0 if not empty
 */
uint8_t FIFO_IsEmpty_Tx(void){
	
}

/** FIFO_IsFull_Tx
 * Checks if Fifo is full
 * @return 1 if full, 0 if empty
 */
uint8_t FIFO_IsFull_Tx(void){
	
}

/** FIFO_Get_Tx
 * Gets the next content in tx fifo
 * @Precondition Fifo is not empty
 * @return unsigned 8-bit data in tx Fifo
 */
uint8_t FIFO_Get_Tx(void){
	
}

/** FIFO_Put_Tx
 * Puts the next content in tx fifo
 * @Precondition Fifo is not full
 * @param unsigned 8-bit data in Fifo
 */
void FIFO_Put_Tx(uint8_t data){
	
}

/** FIFO_IsEmpty_Rx
 * Checks if rx Fifo is empty
 * @return 1 if empty, 0 if not empty
 */
uint8_t FIFO_IsEmpty_Rx(void){
	
}

/** FIFO_IsFull_Tx
 * Checks if Fifo is full
 * @return 1 if full, 0 if empty
 */
uint8_t FIFO_IsFull_Rx(void){
	
}

/** FIFO_Get_Rx
 * Gets the next content in rx fifo
 * @Precondition Fifo is not empty
 * @return unsigned 8-bit data in rx Fifo
 */
uint8_t FIFO_Get_Rx(void){
	
}

/** FIFO_Put_Rx
 * Puts the next content in rx fifo
 * @Precondition Fifo is not full
 * @param unsigned 8-bit data in Fifo
 */
void FIFO_Put_Rx(uint8_t data){
	
}

