/**
 * \file jsw_rand.h
 *
 * Public interface for a random number generator(RNG) based on the file 
 * \e jsw_rand.c written by Julienne Walker. See :
 * http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
 *
 * This file has been modified by:
 *  - definition of \e jsw_rand_t
 *  - the addition of \e doxygen comments
 */

#ifndef JSW_RAND_H
#define JSW_RAND_H

#ifdef __cplusplus
extern "C" {
#endif

/** Definition of the type returned by \e jsw_rand() */
typedef unsigned long jsw_rand_t;

/** Seed the random number generator. 
 *
 * Seed the random number generator. Must be called first. 
 *
 * \param seed RNG seed
 */
void jsw_seed ( jsw_rand_t seed );

/** Return a random number.
 *
 * Return a random number.
 * \return a random number
 */
jsw_rand_t jsw_rand ( void );

/** Return a seed derived from the current system time.
 *
 * Return a seed derived from the current system time.
 * \return A seed derived from the current system time.
 */
jsw_rand_t jsw_time_seed( void );

#ifdef __cplusplus
}
#endif

#endif
