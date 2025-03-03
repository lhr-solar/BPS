/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/*
 * This file implements a EMA filter. 
 * This performs the operation y_t = α * x_t + (1 - α) * y_{t-1}
 * 
 * In order to use it in another file, you must import it in 
 * a particular way.
 * 
 * 1. Define your data type, like so
 *    #define EMA_FILTER_TYPE int
 * 2. Define your filter cofficient, like so
 *    #define EMA_FILTER_ALPHA_NUMERATOR 1
 *    #define EMA_FILTER_ALPHA_DEMONINATOR 8
 * 3. Define the number of channels in your filter, like so
 *    #define EMA_FILTER_CHANNELS (31)
 * 3. Name your EMA filter
 *    #define EMA_FILTER_NAME my_filter
 * 4. Import this file
 *    #include "EMAFilter.h"
 * 
 * This file includes some defaults, but they might not work for
 * your case!
 * 
 * Also, this file undef's everything at the end, so you can import
 * multiple times if you need.
 * 
 * If EMA_FILTER_NAME == my_filter, then your new data structure will be
 * called my_filter_t.
 * 
 * NOTE: importantly, this does not currently support usage from
 * header files. That is, all these types/functions are statically
 * declared, so there cannot be a non-static fifo at the moment.
 */

// The header guard only guard the import,
// since this file can be imported multiple times 
#ifndef EMA_FILTER_H
#define EMA_FILTER_H
#include <stdint.h>
#include <string.h>
#endif

// The type of the EMA filter
#ifndef EMA_FILTER_TYPE
#define EMA_FILTER_TYPE int32_t
#endif

// The alpha coefficient of the EMA filter, as a fraction
#ifndef EMA_FILTER_ALPHA_NUMERATOR
#define EMA_FILTER_ALPHA_NUMERATOR 1
#endif

#ifndef EMA_FILTER_ALPHA_DEMONINATOR
#define EMA_FILTER_ALPHA_DEMONINATOR 8
#endif

// The number of channels in the EMA filter
#ifndef EMA_FILTER_CHANNELS
#define EMA_FILTER_CHANNELS 1
#endif

// The name of the EMA filter (minus the _t)
#ifndef EMA_FILTER_NAME
#define EMA_FILTER_NAME define_your_filter_type
#endif

// Utility definitions
#define _CONCAT(A, B) A ## B
#define CONCAT(A, B) _CONCAT(A, B)

// Type names
#define EMA_FILTER_STRUCT_NAME CONCAT(EMA_FILTER_NAME, _s)
#define EMA_FILTER_TYPE_NAME CONCAT(EMA_FILTER_NAME, _t)

// The actual structure
typedef struct EMA_FILTER_STRUCT_NAME {
    EMA_FILTER_TYPE prev[EMA_FILTER_CHANNELS];
} EMA_FILTER_TYPE_NAME;

// Define some names for our functions
#define INIT        CONCAT(EMA_FILTER_NAME, _init)
#define GET         CONCAT(EMA_FILTER_NAME, _get)
#define PUT         CONCAT(EMA_FILTER_NAME, _put)

/**
 * @brief Initialize a new EMA filter
 * 
 * If the type of the filter is myfilter_t, then this function
 * will be called myfilter_init().
 * 
 * @param filter    a pointer to the EMA filter to initialize
 * @param init      initial value
 */
static inline void __attribute__((unused))
INIT (EMA_FILTER_TYPE_NAME *filter, EMA_FILTER_TYPE init) {
    for (uint32_t ch = 0; ch < EMA_FILTER_CHANNELS; ch++) {
        filter->prev[ch] = init;
    }
}

/**
 * @brief update the EMA filter by giving it a new set of values for all channels
 * 
 * @param filter    a pointer to the EMA filter
 * @param values    a complete set of new values for all channels to add to the EMA filter
 * 
 */
static inline void __attribute__((unused))
PUT (EMA_FILTER_TYPE_NAME *filter, EMA_FILTER_TYPE *values) {
    for (uint32_t ch = 0; ch < EMA_FILTER_CHANNELS; ch++) {
        filter->prev[ch] = 
            ( EMA_FILTER_ALPHA_NUMERATOR * values[ch]
                + (EMA_FILTER_ALPHA_DEMONINATOR - EMA_FILTER_ALPHA_NUMERATOR) * filter->prev[ch] )
            / EMA_FILTER_ALPHA_DEMONINATOR;
    }
}

/**
 * @brief get a complete set of filtered values for all channels
 * 
 * @param filter    a pointer to the EMA filter
 * @param dest      a pointer to a buffer to store all of the filtered values
 */
static inline void __attribute__((unused))
GET (EMA_FILTER_TYPE_NAME *filter, EMA_FILTER_TYPE *dest) {
    memcpy(dest, filter->prev, sizeof(EMA_FILTER_TYPE) * EMA_FILTER_CHANNELS);
}


// undef everything, so this file can be included multiple times
#undef EMA_FILTER_TYPE
#undef EMA_FILTER_DEPTH
#undef EMA_FILTER_CHANNELS
#undef EMA_FILTER_NAME
#undef _CONCAT
#undef CONCAT
#undef EMA_FILTER_STRUCT_NAME
#undef EMA_FILTER_TYPE_NAME
#undef EMA
#undef INIT
#undef GET
#undef PUT

