/**
 * \file interp_utility.h
 */

#if !defined(INTERP_UTILITY_H)
#define INTERP_UTILITY_H

#ifdef __cplusplus
using std::size_t;
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/** @name Interpreter messages
 */
/** @{ */

/** Print a message from the interpreter. Use this rather than printf(). 
 * \param format string defining printf format
 * \param ... variable argument list to printf
 */
void interpMessage(const char *format, ...);

/** @} */

/** @name Interpreter error reporting 
 */
/** @{ */

/**
 * Interpreter error reporting function.
 *
 * Report intepreter error and exit.
 *
 * \param function name
 * \param line in function
 * \param format of error report passed to printf
 */
void interpError(const char *function, 
                 const unsigned int line, 
                 const char *format, 
                 ...);
/** @} */

/** @name Interpreter utility functions
 */
/** @{ */

/** A simple random number generator from p.279 of 
 * "Numerical Recipes in C" 2nd Edn. OS independent!
 * \param range Value returned is a random integer between \e 0 and \e range-1
 * \return Random integer between \e 0 and \e range-1
 */
long interpRand(const long range);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
