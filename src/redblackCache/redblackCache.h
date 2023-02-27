/**
 * \file redblackCache.h
 *
 * Public interface for a red-black cache type.
 */

#if !defined(REDBLACK_CACHE_H)
#define REDBLACK_CACHE_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdlib>
#include <cstdbool>
#include <cstddef>
using std::size_t;
extern "C" {
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"

/**
 * \e redblackCache_t structure. An opaque type for a red-black cache.
 */
typedef struct redblackCache_t redblackCache_t;

/**
 * \e redblackCache_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*redblackCacheAllocFunc_t)(const size_t amount,
                                          void * const user);

/**
 * \e redblackCache_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*redblackCacheDeallocFunc_t)(void * const pointer, 
                                           void * const user);

/**
 * \e redblackCache_t entry data memory duplicator callback function.
 *
 * Red-black cache entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void * (*redblackCacheDuplicateEntryFunc_t)(void * const entry, void * const user);

/**
 * \e redblackCache_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*redblackCacheDeleteEntryFunc_t)(void * const entry, void * const user);

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
typedef void (*redblackCacheDebugFunc_t)(const char *function,
                                         const unsigned int line,
                                         void * const user,
                                         const char *format, 
                                         ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries when searching in the
 * red-black cache.  The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*redblackCacheCompFunc_t)(const void * const a,
                                             const void * const b, 
                                             void * const user);

/**
 * Operate on an entry.
 * 
 * Callback function run on an entry by \e redblackCacheWalkByAge().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*redblackCacheWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty red-black cache.
 * 
 * Creates and initialises an empty \e redblackCache_t instance
 *
 * \param space initial number of available redblackCache elements
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e redblackCache_t. \e NULL indicates failure
 */
redblackCache_t *redblackCacheCreate
(const size_t space,
 const redblackCacheAllocFunc_t alloc, 
 const redblackCacheDeallocFunc_t dealloc,
 const redblackCacheDuplicateEntryFunc_t duplicateEntry,
 const redblackCacheDeleteEntryFunc_t deleteEntry,
 const redblackCacheDebugFunc_t debug,
 const redblackCacheCompFunc_t comp, 
 void * const user);

/**
 * Find an entry in the red-black cache.
 *
 * Searches the red-black cache for an entry. Typically, the entry
 * passed will be a pointer to a dummy entry declared automatic by the
 * caller and containing the requested entry. This entry might
 * correspond to a simple value or to the key of a key/value entry in
 * a dictionary.
 *
 * \param cache \e redblackCache_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the red-black cache.
 * \e NULL indicates failure to find the entry.
 */
void *redblackCacheFind(redblackCache_t * const cache, void * const entry);

/**
 * Insert an entry in the red-black cache.
 *
 * Inserts an entry in the red-black cache. The memory used by entry
 * is assumed to have been allocated by the caller. If the entry
 * already exists then it will be replaced. If the \e deleteEntry()
 * callback has been defined then that function will be called to
 * deallocate the caller's entry memory when the entry is replaced.
 *
 * \param cache \e redblackCache_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data installed in the red-black cache. 
 * \e NULL indicates failure.
 */
void *redblackCacheInsert(redblackCache_t * const cache, void * const entry);

/**
 * Clear the red-black cache.
 *
 * Clears all entries from the red-black cache. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param cache \e redblackCache_t pointer
 */
void redblackCacheClear(redblackCache_t * const cache);

/** 
 * Destroy the redblackCache_t.
 *
 * Detaches and deallocates all entries from the splay red-black
 * cache. If the \e deleteEntry() member exists then deallocates the
 * caller's entry data.
 *
 * \param cache pointer to \e redblackCache_t
 */
void redblackCacheDestroy(redblackCache_t * const cache);

/** 
 * Get the depth of the red-black cache.
 *
 * For an implementation of the red-black cache using a tree recursively
 * descends the tree and returns the depth of the tree.
 *
 * \param cache pointer to \e redblackCache_t
 * \return the depth of the red-black cache
 */
size_t redblackCacheGetDepth(const redblackCache_t * const cache);

/** 
 * Get the size of the red-black cache.
 *
 * Returns the number of entries in the red-black cache.
 *
 * \param cache pointer to \e redblackCache_t
 * \return number of entries in the red-black cache
 */
size_t redblackCacheGetSize(const redblackCache_t * const cache);

/** 
 * Walk the red-black cache in increasing age order.
 *
 * Walks the red-black cache in increasing age order calling \e walk on each 
 * entry. 
 *
 * \param cache pointer to \e redblackCache_t
 * \param walk pointer to function called for each entry
 * \return \e bool indicating success
 */
bool redblackCacheWalk(redblackCache_t * const cache, 
                       const redblackCacheWalkFunc_t walk);

/** 
 * Check the red-black cache tree.
 *
 * Traverses the red-black cache list checking that tree node order is 
 * consistent.
 *
 * \param cache pointer to \e redblackCache_t
 * \return \e bool indicating success. 
 */
bool redblackCacheCheck(redblackCache_t * const cache);

#ifdef __cplusplus
}
#endif

#endif
