/**
 * \file swTree.h
 *
 * Public interface for a Stout/Warren tree type.
 *
 * See: "Tree Rebalancing in Optimal Time and Space", Q. F. Stout and
 * B. L. Warren, Communications of the ACM, September 1986, Volume 29,
 * Number 9, pp. 902-908
 */

#if !defined(STOUT_WARREN_TREE_H)
#define STOUT_WARREN_TREE_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdlib>
#include <cstdbool>
#include <cstddef>
extern "C" {
  using std::size_t;
#else
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"

  /**
   * \e swTree_t structure. An opaque type for a Stout/Warren tree.
   */
  typedef struct swTree_t swTree_t; 

  /**
   * \e swTree_t memory allocator.
   *
   * Memory allocation call-back.
   *
   * \param size amount of memory requested
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return pointer to the memory allocated. \e NULL indicates failure.
   */
  typedef void *(*swTreeAllocFunc_t)(const size_t size, void * const user);

  /**
   * \e swTree_t memory de-allocator.
   *
   * Memory deallocation call-back.
   *
   * \param ptr pointer to memory to be deallocated
   * \param user \e void pointer to user data to be echoed by callbacks
   */
  typedef void (*swTreeDeallocFunc_t)(void * const ptr, void * const user);

  /**
   * \e swTree_t entry data memory duplicator callback function.
   *
   * Stout/Warren tree entry duplicator call-back for caller's entry data.
   *
   * \param entry pointer to memory to be duplicated
   * \param user \e void pointer to user data to be echoed by callbacks
   */
  typedef void *(*swTreeDuplicateEntryFunc_t)(void * const entry,
                                              void * const user);

  /**
   * \e swTree_t entry data memory de-allocator.
   *
   * Memory deallocation call-back for caller's entry data
   *
   * \param entry pointer to caller's memory to be deallocated
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e bool for compatibility with walk.
   */
  typedef bool (*swTreeDeleteEntryFunc_t)(void * const entry, 
                                          void * const user);

  /**
   * Debugging message.
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
  typedef void (*swTreeDebugFunc_t)(const char *function,
                                    const unsigned int line,
                                    void * const user,
                                    const char *format, 
                                    ...);

  /**
   * Compare two entries.
   * 
   * Callback function to compare two entries when searching in the tree. 
   * The entry type is defined by the caller.
   *
   * \param a pointer to an entry defined by the caller
   * \param b pointer to an entry defined by the caller
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e compare_e value
   */
  typedef compare_e (*swTreeCompFunc_t)(const void * const a,
                                        const void * const b,
                                        void * const user);

  /**
   * Operate on entry.
   * 
   * Callback function run on an entry by \e swTreeWalk().
   *
   * \param entry pointer to an entry defined by the caller
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e bool indicating success.
   */
  typedef bool (*swTreeWalkFunc_t)(void * const entry, void * const user);

  /**
   * Create an empty Stout/Warren tree.
   * 
   * Creates and initialises an empty \e swTree_t instance
   *
   * \param alloc memory allocator callback
   * \param dealloc memory deallocator callback
   * \param duplicateEntry entry duplication callback for caller's entry data
   * \param deleteEntry memory deallocator callback for callers entry data
   * \param debug message function callback
   * \param comp entry key comparison function callback
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return pointer to a \e swTree_t. \e NULL indicates failure
   */
  swTree_t *swTreeCreate
  (const swTreeAllocFunc_t alloc, 
   const swTreeDeallocFunc_t dealloc,
   const swTreeDuplicateEntryFunc_t duplicateEntry,
   const swTreeDeleteEntryFunc_t deleteEntry,
   const swTreeDebugFunc_t debug,
   const swTreeCompFunc_t comp,
   void * const user);

  /**
   * Find an entry in the Stout/Warren tree.
   *
   * Searches the Stout/Warren tree for an entry. Typically, the entry passed 
   * will be a pointer to a dummy entry declared automatic by the caller and
   * containing the requested entry. This entry might correspond to a simple
   * value or to the key of a key/value entry in a dictionary.
   *
   * \param tree \e swTree_t pointer
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the entry found in the tree.
   * \e NULL indicates failure to find the entry.
   */
  void *swTreeFind(swTree_t * const tree, void * const entry);

  /**
   * Insert an entry in the tree.
   *
   * Inserts an entry in the tree. The memory used by the entry is
   * assumed to have been allocated by the caller. If the entry already
   * exists and the \e deleteEntry() callback exists then the
   * existing entry is deleted. If the entry already exists and the 
   * \e deleteEntry callback has not been defined then the existing entry
   * is overwritten and to replace such an entry the caller must first
   * remove the entry from the tree. The entry data pointer inserted is
   * returned.
   *
   * \param tree pointer to \e swTree_t 
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the entry data in the tree. 
   * \e NULL indicates  failure.
   */
  void *swTreeInsert(swTree_t * const tree, void * const entry);

  /**
   * Remove an entry from the tree.
   *
   * Removes an entry in the tree. The memory used by entry is 
   * assumed to have been allocated by the caller. If the entry is
   * found and the \e deleteEntry()callback has been defined then that 
   * function will be called to deallocate the entry memory when the 
   * entry is removed.
   *
   * \param tree pointer to \e swTree_t
   * \param entry \e void pointer to the caller's entry data
   * \return \e void pointer to the entry found in the tree. If the entry 
   * is found and \e deleteEntry() exists then NULL is returned. Otherwise
   * \e NULL is returned.
   */
  void *swTreeRemove(swTree_t * const tree, void * const entry);

  /**
   * Clear the Stout/Warren tree.
   *
   * Clears all entries from the Stout/Warren tree. The memory used by entry is 
   * assumed to have been allocated by the caller. If the \e deleteEntry()
   * callback has been defined then that function will be called to 
   * deallocate the caller's entry memory when the entries are removed.
   *
   * \param tree \e swTree_t pointer
   */
  void swTreeClear(swTree_t * const tree);

  /** 
   * Destroy the Stout/Warren tree.
   *
   * Removes and deallocates all entries from the Stout/Warren tree. If the 
   * \e deleteEntry() member exists then deallocates the caller's entry data.
   *
   * \param tree pointer to \e swTree_t
   */
  void swTreeDestroy(swTree_t * const tree);

  /** 
   * Get the depth of the tree.
   *
   * Traverses the Stout/Warren tree to find the maximum depth.
   *
   * \param tree pointer to \e swTree_t
   * \return depth of the tree
   */
  size_t swTreeGetDepth(const swTree_t * const tree);

  /** 
   * Get the size of the Stout/Warren tree.
   *
   * Returns the number of entries in the tree
   *
   * \param tree pointer to \e swTree_t
   * \return number of entries in the tree
   */
  size_t swTreeGetSize(const swTree_t * const tree);

  /** 
   * Get the minimum entry in the Stout/Warren tree.
   *
   * Returns a pointer to the minimum (leftmost) entry in the tree
   *
   * \param tree pointer to \e swTree_t
   * \return \e void pointer to the minimum entry in the tree. 
   *         \e NULL if the tree is empty.
   */
  void *swTreeGetMin(swTree_t * const tree);

  /** 
   * Get the maximum entry in the Stout/Warren tree.
   *
   * Returns a pointer to the maximum (rightmost) entry in the tree.
   *
   * \param tree pointer to \e swTree_t
   * \return \e void pointer to the maximum entry in the tree.
   *         \e NULL if the tree is empty.
   */
  void *swTreeGetMax(swTree_t * const tree);

  /** 
   * Operate on each tree entry.
   *
   * Traverses the tree in increasing order calling a function for each entry.
   *
   * \param tree pointer to \e swTree_t
   * \param walk pointer to a callback function to be called for each entry
   * \return \e bool indicating sucess
   */
  bool swTreeWalk(swTree_t * const tree, 
                  const swTreeWalkFunc_t walk);

  /** 
   * Check the consistency of the tree.
   *
   * Traverses the tree checking that nodes are consistent.
   *
   * \param tree pointer to \e swTree_t
   * \return \e bool indicating success. 
   */
  bool swTreeCheck(swTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
