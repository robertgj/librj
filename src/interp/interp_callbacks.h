/**
 * \file interp_callbacks.h
 */

#if !defined(INTERP_CALLBACKS_H)
#define INTERP_CALLBACKS_H

#ifdef __cplusplus
using std::size_t;
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "compare.h"
  
/** @name Interpreter callback functions installed by interpCreate()
 */
/** @{ */

/**
 * Interpreter memory allocator callback function.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
void *interpAlloc(const size_t amount, void * const user);

/**
 * Interpreter memory re-allocator callback function.
 *
 * Memory re-allocation call-back.
 *
 * \param entry pointer to memory to be reallocated
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
void *interpRealloc(void * const entry,
                    const size_t amount,
                    void * const user);

/**
 * Interpreter memory de-allocator callback function.
 *
 * Memory deallocation call-back.
 *
 * \param entry pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
void interpDealloc(void * const entry, void * const user);

/**
 * Interpreter entry duplicator callback function.
 *
 * Data structure entry duplicator call-back.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
void *interpDuplicateEntry(void * const entry, void * const user);
  
/**
 * Interpreter entry memory de-allocator callback function.
 *
 * Data structure entry memory deallocation call-back.
 *
 * \param entry pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
bool interpDeleteEntry(void * const entry, void * const user);

/**
 * Interpreter debugging message callback function.
 *
 * Callback function to output a debugging message. Accepts a 
 * variable argument list like \e printf(). 
 *
 * \param function name
 * \param line number
 * \param user \e void pointer to user data to be echoed by callbacks
 * \param format string
 * \param ... variable argument list
 */
void interpDebug(const char *function, 
                 const unsigned int line, 
                 void * const user,
                 const char *format, 
                 ...);

/**
 * Interpreter entry compare callback function.
 *
 * Data structure entry compare callback function.
 *
 * \param a pointer to first entry to be compared
 * \param b pointer to second entry to be compared
 * \param user \e void pointer to user data to be echoed by callbacks
 */
compare_e
interpComp(const void * const a, const void * const b, void * const user);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
