/**
 * \file jsw_slib.h
 *
 * Public interface for a random number generator(RNG) based on the file 
 * \e jsw_slib.c written by Julienne Walker. See :
 * http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
 *
 * This file has been modified by:
 *  - addition of a pointer to user private data
 *  - addition of \e const qualifiers
 *  - the addition of \e doxygen comments
 * 
 * The original file comments follow :
 *
 * Classic skip list library
 *
 *   Created (Julienne Walker): April 11, 2004
 *
 *   Updated (Julienne Walker): August 19, 2005
 *
 * This code is in the public domain. Anyone may
 * use it or change it in any way that they see
 * fit. The author assumes no responsibility for 
 * damages incurred through use of the original
 * code or any variations thereof.
 *
 * It is requested, but not required, that due
 * credit is given to the original author and
 * anyone who has modified the code through
 * a header comment, such as this one.
 */

#ifndef JSW_SLIB_H
#define JSW_SLIB_H

#ifdef __cplusplus
#include <cstddef>
using std::size_t;
extern "C" {
#else
#include <stddef.h>
#endif

  /** \e jsw_skip_t structure. An opaque type for a skip list. */
  typedef struct jsw_skip jsw_skip_t;

  /** Compare two items in the \e jsw_skip_t.
   * 
   * Callback function to compare two items in the jsw_skip_t skip list. 
   * The item type is defined by the caller.
   *
   * \param a pointer to an item defined by the caller
   * \param b pointer to an item defined by the caller
   * \param priv \e void pointer to private data to be echoed by callbacks
   * \return \e -1 indicates \e a<b \n
   *         \e 0 indicates \e a==b \n
   *         \e 1 indicates \e a>b
   */
  typedef int (*cmp_f) ( const void * const a,
                         const void * const b, 
                         void * const priv );

  /** Duplicate an item to be inserted in the \e jsw_skip_t skip list.
   *
   * Skip list item duplicator call-back for caller's item data.
   *
   * \param item \e void pointer to memory to be duplicated
   * \param priv \e void pointer to private data to be echoed by callbacks
   * \return \e void pointer to the memory allocated. \e NULL indicates failure.
   */
  typedef void *(*dup_f) ( void * const item, void * const priv );

  /** Delete an item to be removed from the \e jsw_skip_t skip list.
   *
   * Skip list item deletion call-back for caller's item data.
   *
   * \param item \e void pointer to caller's memory to be deallocated
   * \param priv \e void pointer to private data to be echoed by callbacks
   */
  typedef void (*rel_f) ( void * const item, void * const priv );

  /** Create a new \e jsw_skip_t skip list.
   *
   * Create a new skip list with a maximum height of \e max.
   * \param max maximum height of the skip list
   * \param cmp item key comparison callback function 
   * \param dup item duplication callback function 
   * \param rel item deletion callback function 
   * \param priv \e void pointer to private data to be echoed by callbacks
   * \return Pointer to the new, empty, skip list. \e NULL indicates failure.
   */
  jsw_skip_t *jsw_snew ( size_t max, 
                         const cmp_f cmp, 
                         const dup_f dup, 
                         const rel_f rel, 
                         void * const priv );

  /** Delete a \e jsw_skip_t skip list
   *
   * Release all memory used by the skip list 
   * \param skip pointer to a \e jsw_skip_t instance
   */
  void jsw_sdelete ( jsw_skip_t *skip );

  /** Find an item in a \e jsw_skip_t skip list
   *
   * Given a pointer to an item representing the key to be found, search
   * the skip list for an item representing the key-value pair.
   * \param skip pointer to a \e jsw_skip_t instance
   * \param item \e void pointer to a key rpresenting the item to be found.
   * \return A pointer to the item found. \e NULL if not found.
   */
  void *jsw_sfind ( jsw_skip_t * const skip, const void * const item );

  /** Insert an item with the selected key in a \e jsw_skip_t skip list
   *
   * Given a pointer to an item, insert a copy of the item into the skip list.
   * \param skip pointer to a \e jsw_skip_t instance
   * \param item \e void pointer to a key rpresenting the item to be inserted.
   * \return Non-zero for success, zero for failure.
   */
  int jsw_sinsert ( jsw_skip_t * const skip, void * const item );

  /** Remove an item with the selected key from a \e jsw_skip_t skip list
   *
   * Given a pointer to an item, remove the item from the skip list and 
   * deallocate the memory for the item.
   * \param skip pointer to a \e jsw_skip_t instance of a skip list
   * \param item \e void  pointer to the item to be removed from the skip list
   * \return Non-zero for success, zero for failure.
  */
  int jsw_serase ( jsw_skip_t * const skip, void *item );

  /** Get the size of the \e jsw_skip_t skip list.
   *
   * Returns the number of entries in the skip list at height 0
   * \param skip pointer to \e jsw_skip_t instance
   * \return number of entries in the skip list at height 0
   */
  size_t jsw_ssize ( const jsw_skip_t * const skip );

  /** Reset the internal position of the \e jsw_skip_t skip list
   *
   * Reset the traversal markers in the internal state of the \e jsw_skip_t
   * skip list to the beginning of the list.
   * \param skip pointer to a \e jsw_skip_t instance
   */
  void jsw_sreset ( jsw_skip_t * const skip );

  /** Get the current item in the \e jsw_skip_t skip list
   *
   * Return the current item in the \e jsw_skip_t skip list.
   * \param skip pointer to a \e jsw_skip_t instance
   * \return A pointer to the item. \e NULL indicates end-of-list.
  */
  void *jsw_sitem ( const jsw_skip_t * const skip );

  /** Traverse forward by one key in the \e jsw_skip_t skip list
   *
   * Traverse forward by one key in the \e jsw_skip_t skip list.
   * \param skip pointer to a \e jsw_skip_t instance
   * \return 0 if end-of-list, 1 otherwise.
   */
  int jsw_snext ( jsw_skip_t * const skip );

#ifdef __cplusplus
}
#endif

#endif
