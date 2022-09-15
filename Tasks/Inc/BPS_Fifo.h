/**
 *  Copyright Longhorn Racing Solar 2022-2023 
 *  Implements a FIFO structure on Micrium's Heap
 * 
*/
#ifndef __BPS_FIFO_H
#define __BPS_FIFO_H

// Includes
#include "os.h"
#include "lib_mem.h"
#include <stdint.h>
#include <stdbool.h>

// Flags for the Fifo
typedef enum {
    FIFO_INITIALIZED,
    FIFO_EMPTY,
    FIFO_FULL,
    FIFO_AVAILABLE,
    FIFO_ERR
} fifo_flags;

// Fifo struct
typedef struct {
    size_t fifoSize;
    size_t elementSize;
    intptr_t* putptr;
    intptr_t* getptr;
    intptr_t* fifo; // range of fifo base address -> fifo base address[fifoSize - 1]
    unsigned int flags;
} BPS_Fifo_t;

// An invalid fifo - returned on errors
const BPS_Fifo_t INVALID_FIFO = {0, 0, NULL, NULL, NULL, FIFO_ERR};

/**
 * @brief Returns size of an element in the FIFO
 * 
 * @param fifo 
 * @return size_t 
 */
inline size_t fifoTypeSize(BPS_Fifo_t* fifo) {
    return fifo->elementSize;
}

/**
 * @brief Determines if fifo is empty or non empty
 * 
 * @param fifo - pointer to a fifo struct (should be created with Fifo_Init)
 * @return true if empty
 * @return false if not empty
 */
inline bool fifoEmpty(BPS_Fifo_t* fifo) {
    return fifo->putptr == fifo->getptr;
}

/**
 * @brief Determines if fifo is full or not full
 * 
 * @param fifo - pointer to a fifo struct (should be created with Fifo_Init)
 * @return true if full
 * @return false if not full
 */
inline bool fifoFull(BPS_Fifo_t* fifo) {
    return (fifo->putptr + fifo->elementSize) == (fifo->fifo + (fifo->elementSize * fifo->fifoSize));
}

/**
 * @brief Initializes a Fifo structure of the specified size, and sets all variables accordingly.
 * @param elementCount - # of elements in the fifo
 * @param elementSize - size (in bytes) of each element in the fifo
 * @return None
 */
inline BPS_Fifo_t Fifo_Init(size_t elementCount, size_t elementSize) {
    BPS_Fifo_t returnedFifo;
    LIB_ERR err;
    intptr_t* base_addr = (intptr_t*)Mem_HeapAlloc(elementCount, 4, NULL, &err);
    if (err != LIB_MEM_ERR_NONE) {
        return INVALID_FIFO;
    }
    returnedFifo = BPS_Fifo_t {
        .fifoSize = elementCount,
        .elementSize = elementSize,
        .fifo = base_addr,
        .putptr = base_addr,
        .getptr = base_addr,
        .flags = FIFO_INITIALIZED | FIFO_AVAILABLE | FIFO_EMPTY
    };
    // zero out the newly created fifo memory
    // this third parameter should be looked at - lib_mem.c specifies "octets", I believe they mean # of bytes to set
    Mem_Set(returnedFifo.fifo, 0, elementCount * elementSize);
    return returnedFifo;
}

/**
 * @brief Returns a pointer to the next element in the fifo
 * 
 * @param fifo* - pointer to a fifo object 
 * @return void* - pointer to the returned element in the fifo. Return value can be casted with *(type *)Fifo_Get().
 */
inline void* Fifo_Get(BPS_Fifo_t* fifo) {
    void* returnPtr = NULL;
    if (!fifoEmpty(fifo)) {
        returnPtr = fifo->getptr;
        fifo->getptr += fifo->elementSize;
        // handle wraparound
        if (fifo->getptr >= fifo->fifo + (fifo->elementSize * fifo->fifoSize)) {
            fifo->getptr = fifo->fifo;
        }
    } else {
        fifo->flags |= FIFO_EMPTY;
    }
    return returnPtr; // Callee MUST check if this is NULL return or not
}

/**
 * @brief Writes an object into the fifo
 * 
 * @param fifo* - pointer to a fifo object
 * @param toPut* - void pointer to the element to insert into the fifo 
 * @return true if successful
 * @return false if unsuccessful
 */
inline bool Fifo_Put(BPS_Fifo_t* fifo, void* toPut) {
    bool success = false;
    if (!fifoFull(fifo)) {
        success = true;
        void* writeAddr = fifo->putptr;
        Mem_Copy(writeAddr, toPut, fifo->elementSize);
        fifo->putptr += fifo->elementSize;
        // handle wraparound
        if (fifo->putptr >= fifo->fifo + (fifo->elementSize * fifo->fifoSize)) {
            fifo->putptr = fifo->fifo;
        }
    } else {
        fifo->flags |= FIFO_FULL;
    }
    return success;
}

/**
 * @brief Similar to Fifo_Get, but doesn't modify the pointers.
 * 
 * @param fifo - pointer to a fifo object
 * @return void* - of the element peeked at by getptr.
 */
inline void* Fifo_Peek(BPS_Fifo_t* fifo) {
    void* returnPtr = NULL;
    if (!fifoEmpty) {
        returnPtr = fifo->getptr;
    }
    return returnPtr;
}

/**
 * @brief Take the most recent element of the fifo out. The returned pointer can be casted, just like Fifo_Get
 * 
 * @param fifo - pointer to a fifo object
 * @return void* - 
 */
inline void* Fifo_Pop(BPS_Fifo_t* fifo) {
    void* returnPtr = NULL;
    if (!fifoEmpty(fifo)) {
        fifo->putptr -= fifo->elementSize;
        // handle wraparound
        if (fifo->putptr < fifo->fifo) 
            fifo->putptr = (fifo->fifo + fifo->elementSize*fifo->fifoSize);
        returnPtr = fifo->putptr;
    }
    return returnPtr;
}

#endif