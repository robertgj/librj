/** 
 * \file splayCache_private.h 
 *
 * Private definition for a splay-tree cache type. 
 */

#if !defined(SPLAY_CACHE_PRIVATE_H)
#define SPLAY_CACHE_PRIVATE_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
using std::size_t;
extern "C" {
#else
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"
#include "splayCache.h"

/**
 * \c splayCache_t internal entry type. 
 *
 * Internal representation of a splayCache_t entry.
 */
typedef struct splayCacheEntry_t
{
  void *entry;
  /**< Pointer to caller's entry data. 
     This memory was allocated by the caller. */

  struct splayCacheEntry_t *next;
  /**< Pointer to the next entry */

  struct splayCacheEntry_t *prev;
  /**< Pointer to the previous entry */

  /* Splay tree stuff */
  struct splayCacheEntry_t *left;
  /**< Pointer to the left child of the node */

  struct splayCacheEntry_t *right;
  /**< Pointer to the right child of the node */
}
  splayCacheEntry_t;

/**
 * \c splayCache_t structure.
 * 
 * Private implementation of \c splayCache_t. Uses a linked list
 * threading a splay tree in age order. New entries are allocated up
 * to \e maxSize after which the oldest entry is re-used as the newest
 * cache entry.
 *
 * \see \c list_t \c splayTree_t
 */
struct splayCache_t
{
  splayCacheAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c splayCache_t. */

  splayCacheDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function for \c splayCache_t. */

  splayCacheDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  splayCacheDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  splayCacheDebugFunc_t debug;
  /**< Debugging message callback function. */

  splayCacheCompFunc_t compare;
  /**< Callback function to compare two entries in the splay tree.  */

  splayCacheEntry_t head;
  /**< First entry. This is a dummy list entry. It never stores a list
     entry data entry pointer. It simplifies list manipulations. The
     \e head.prev member is always \c NULL. The \e head.next member
     points to the first real list entry. If the list is empty then \e
     head.next points to \e tail.prev . */

  splayCacheEntry_t tail;
  /**< Last entry. Also a dummy list entry. The \e tail.next member
     is always \c NULL. The \e tail.prev member points to the last
     real list entry. If the list is empty then \e tail.prev
     points to \e head.next . */

  splayCacheEntry_t *root;
  /**< Root entry in the cache splay tree. */

  size_t size;
  /**< Number of entries in the cache. */

  size_t space;
  /**< Maximum number of entries in the cache. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
