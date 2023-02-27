/**
 * \file skipList.h
 *
 * Public interface for a skip list type based on the files jsw_slib.[ch]
 * written by Julienne Walker. See :
 * [1] "Skip Lists: A Probabilistic Alternative to Balanced Trees", William
 *     Pugh, Commun. ACM, June 1990, Vol. 33, No. 6, pp 668-676
 * [2] http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
 */

#if !defined(SKIP_LIST_H)
#define SKIP_LIST_H


#ifdef __cplusplus
#include <cstdarg>
#include <cstdlib>
#include <cstdbool>
#include <cstddef>
using std::size_t;
extern "C" {
#else
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"

/**
 * \e skipList_t structure. An opaque type for a skip list.
 */
typedef struct skipList_t skipList_t; 

/**
 * \e skipList_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param size amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*skipListAllocFunc_t)(const size_t size, void * const user);

/**
 * \e skipList_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param ptr pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*skipListDeallocFunc_t)(void * const ptr, void * const user);

/**
 * \e skipList_t entry data memory duplicator callback function.
 *
 * Skip list entry duplicator call-back for caller's entry data.
 *
 * \param entry to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e void pointer to the new entry.
 */
typedef void * (*skipListDuplicateEntryFunc_t)(void *const entry,
                                               void * const user);

/**
 * \e skipList_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*skipListDeleteEntryFunc_t)(void * const entry, 
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
typedef void (*skipListDebugFunc_t)(const char *function,
                                    const unsigned int line,
                                    void * const user,
                                    const char *format, 
                                    ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries when searching in the skip list. 
 * The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*skipListCompFunc_t)(const void * const a,
                                        const void * const b,
                                        void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e skipListWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*skipListWalkFunc_t)(void *entry, void * const user);

/**
 * Create an empty skip list.
 * 
 * Creates and initialises an empty \e skipList_t instance
 *
 * \param space maximum available skipList entries
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e skipList_t. \e NULL indicates failure
 */
skipList_t *skipListCreate(const size_t space,
                           const skipListAllocFunc_t alloc, 
                           const skipListDeallocFunc_t dealloc,
                           const skipListDuplicateEntryFunc_t duplicateEntry,
                           const skipListDeleteEntryFunc_t deleteEntry,
                           const skipListDebugFunc_t debug,
                           const skipListCompFunc_t comp,
                           void * const user);

/**
 * Find an entry in the skip list.
 *
 * Searches the skip list for an entry. Typically, the entry passed 
 * will be a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param list \e skipList_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the skip list.
 * \e NULL indicates failure to find the entry.
 */
void *skipListFind(skipList_t * const list, void * const entry);

/**
 * Insert an entry in the skip list.
 *
 * Inserts an entry in the skip list. The memory used by the entry is
 * assumed to have been allocated by the caller. If the entry already
 * exists and the \e deleteEntry() callback exists then the
 * existing entry is deleted. If the entry already exists and the 
 * \e deleteEntry callback has not been defined then the existing entry
 * is simply replaced.  To replace an entry the caller must first
 * remove the entry from the skip list. The entry data pointer inserted is
 * returned.
 *
 * \param list pointer to \e skipList_t 
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data in the skip list. 
 * \e NULL indicates  failure.
 */
void *skipListInsert(skipList_t * const list, void * const entry);

/**
 * Remove an entry from the skip list.
 *
 * Removes an entry in the skip list. The memory used by entry is 
 * assumed to have been allocated by the caller. If the entry is
 * found and the \e deleteEntry()callback has been defined then that 
 * function will be called to deallocate the entry memory when the 
 * entry is removed.
 *
 * \param list pointer to \e skipList_t
 * \param entry \e void pointer to the caller's entry data
 * \return \e void pointer to the entry found in the skip list. If the entry 
 * is found and \e deleteEntry() exists then NULL is returned. Otherwise
 * \e NULL is returned.
 */
void *skipListRemove(skipList_t * const list, void * const entry);

/**
 * Clear the skip list.
 *
 * Clears all entries from the skip list. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param list \e skipList_t pointer
 */
void skipListClear(skipList_t * const list);

/** 
 * Destroy the skip list.
 *
 * Removes and deallocates all entries from the skip list. If the 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param list pointer to \e skipList_t
 */
void skipListDestroy(skipList_t * const list);

/** 
 * Get the size of the skip list.
 *
 * Returns the number of entries in the skip list
 *
 * \param list pointer to \e skipList_t
 * \return number of entries in the skip list
 */
size_t skipListGetSize(const skipList_t * const list);

/** 
 * Get the minimum entry in the skip list.
 *
 * Returns a pointer to the minimum (leftmost) entry in the skip list
 *
 * \param list pointer to \e skipList_t
 * \return \e void pointer to the minimum entry in the skip list. 
 *         \e NULL if the skip list is empty.
 */
void *skipListGetMin(skipList_t * const list);

/** 
 * Get the maximum entry in the skip list  skip list.
 *
 * Returns a pointer to the maximum (rightmost) entry in the skip list.
 *
 * \param list pointer to \e skipList_t
 * \return \e void pointer to the maximum entry in the skip list.
 *         \e NULL if the skip list is empty.
 */
void *skipListGetMax(skipList_t * const list);

/** 
 * Get the next entry in the skip list.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * next (rightwards) entry in the skip list.
 *
 * \param list pointer to \e skipList_t
 * \param entry pointer to caller's data
 * \return \e void pointer to the next entry in the skip list.
 *         \e NULL if the skip list is empty or entry is the rightmost.
 */
void *skipListGetNext(skipList_t * const list, const void * const entry);

/** 
 * Get the previous entry in the skip list.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * previous (leftwards) entry in the skip list.
 *
 * \param list pointer to \e skipList_t
 * \param entry \e void pointer to a callers entry data
 * \return \e void pointer to the previous entry in the skip list.
 *         \e NULL if the skip list is empty or entry is the leftmost.
 */
void *skipListGetPrevious(skipList_t * const list, const void * const entry);

/** 
 * Operate on each skip list entry.
 *
 * Traverses the skip list in increasing order calling a function for each entry.
 *
 * \param list pointer to \e skipList_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating sucess
 */
bool skipListWalk(skipList_t * const list, const skipListWalkFunc_t walk);

/** 
 * Check the skip list.
 *
 * Traverses the skip list checking that nodes are consistent.
 *
 * \param list pointer to \e skipList_t
 * \return \e bool indicating success. 
 */
bool skipListCheck(skipList_t * const list);

#ifdef __cplusplus
}
#endif

#endif
