/**
 * \file splayCache.h
 *
 * Public interface for a cache type.
 */

#if !defined(SPLAY_CACHE_H)
#define SPLAY_CACHE_H

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
 * \e splayCache_t structure. An opaque type for a cache.
 */
typedef struct splayCache_t splayCache_t;

/**
 * \e splayCache_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*splayCacheAllocFunc_t)(const size_t amount, void * const user);

/**
 * \e splayCache_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*splayCacheDeallocFunc_t)(void * const pointer, void * const user);

/**
 * \e splayCache_t entry data memory duplicator callback function.
 *
 * Red-black tree entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void *(*splayCacheDuplicateEntryFunc_t)(void * const entry,
                                                void * const user);

/**
 * \e splayCache_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*splayCacheDeleteEntryFunc_t)(void * const entry, 
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
typedef void (*splayCacheDebugFunc_t)(const char *function,
                                      const unsigned int line,
                                      void * const user,
                                      const char *format, 
                                      ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries when searching in the cache. 
 * The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*splayCacheCompFunc_t)( const void * const a, 
                                           const void * const b, 
                                           void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e splayCacheWalkByAge().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*splayCacheWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty cache.
 * 
 * Creates and initialises an empty \e splayCache_t instance
 *
 * \param space initial number of available splayCache elements
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e splayCache_t. \e NULL indicates failure
 */
splayCache_t *splayCacheCreate
(const size_t space,
 const splayCacheAllocFunc_t alloc, 
 const splayCacheDeallocFunc_t dealloc,
 const splayCacheDuplicateEntryFunc_t duplicateEntry,    
 const splayCacheDeleteEntryFunc_t deleteEntry,
 const splayCacheDebugFunc_t debug,
 const splayCacheCompFunc_t comp, 
 void * const user);

/**
 * Find an entry in the cache.
 *
 * Searches the cache for an entry. Typically, the entry passed will be
 * a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param cache \e splayCache_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the cache.
 * \e NULL indicates failure to find the entry.
 */
void *splayCacheFind(splayCache_t * const cache,  void * const entry);

/**
 * Insert an entry in the cache.
 *
 * Inserts an entry in the cache. The memory used by entry is assumed
 * to have been allocated by the caller. If the entry already exists
 * then it will be replaced. If the \e deleteEntry() callback has been
 * defined then that function will be called to deallocate the
 * caller's entry memory when the entry is replaced.
 *
 * \param cache \e splayCache_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data installed in the cache. 
 * \e NULL indicates failure.
 */
void *splayCacheInsert(splayCache_t * const cache, void * const entry);

/**
 * Clear the cache.
 *
 * Clears all entries from the cache. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param cache \e splayCache_t pointer
 */
void splayCacheClear(splayCache_t * const cache);

/** 
 * Destroy the splay-tree cache.
 *
 * Detaches and deallocates all entries from the splay-tree cache. If the 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param cache pointer to \e splayCache_t
 */
void splayCacheDestroy(splayCache_t * const cache);

/** 
 * Get the depth of the cache.
 *
 * For an implementation of the cache using a tree recursively
 * descends the tree and returns the depth of the tree.
 *
 * \param cache pointer to \e splayCache_t
 * \return the depth of the cache
 */
size_t splayCacheGetDepth(const splayCache_t * const cache);

/** 
 * Get the size of the cache.
 *
 * Returns the number of entries in the cache.
 *
 * \param cache pointer to \e splayCache_t
 * \return number of entries in the cache
 */
size_t splayCacheGetSize(const splayCache_t * const cache);

/** 
 * Walk the cache in increasing age order.
 *
 * Walks the cache in increasing age order calling \e walk on each 
 * entry. 
 *
 * \param cache pointer to \e splayCache_t
 * \param walk pointer to function called for each entry
 * \return \e bool indicating success
 */
bool splayCacheWalk(splayCache_t * const cache, const splayCacheWalkFunc_t walk);

/** 
 * Check the cache tree.
 *
 * Traverses the cache list checking that tree node order is consistent.
 *
 * \param cache pointer to \e splayCache_t
 * \return \e bool indicating success. 
 */
bool splayCacheCheck(splayCache_t * const cache);

/** 
 * Rebalance the cache.
 *
 * First converts the cache to look like a linked list by accessing nodes 
 * in order. Next balances the resulting list-like cache by multiple passes
 * moving alternate nodes to the root. The number of passes is about
 * log base 2 of the number of entries.
 *
 * \param cache pointer to \e splayCache_t
 * \return \e bool indicating success. 
 */
bool splayCacheBalance(splayCache_t * const cache);
  
#ifdef __cplusplus
}
#endif

#endif
