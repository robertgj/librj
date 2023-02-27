/** 
 * \file redblackCache_private.h 
 *
 * Private definition for a Red-Black cache tree type. 
 */

#if !defined(REDBLACK_CACHE_PRIVATE_H)
#define REDBLACK_CACHE_PRIVATE_H

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
#include "redblackTree.h"
#include "redblackCache.h"

/**
 * \c redblackCache_t internal entry type. 
 *
 * Internal representation of a \c redblackCache_t entry.
 */
typedef struct redblackCacheEntry_t
{
  void *entry;
  /**< Pointer to caller's entry data. 
     This memory was allocated by the caller. */

  struct redblackCacheEntry_t *next;
  /**< Pointer to the next entry */

  struct redblackCacheEntry_t *prev;
  /**< Pointer to the previous entry */
} redblackCacheEntry_t;

/**
 * \c redblackCache_t structure.
 * 
 * Private implementation of \c redblackCache_t. Uses a linked list and a
 * red-black tree.  New entries are allocated up to \e space after
 * which the oldest entry is re-used as the newest cache entry.
 *
 * \see \c list_t \c redblackTree_t
 */
struct redblackCache_t
{
  redblackCacheAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c redblackCache_t. */

  redblackCacheDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function for \c redblackCache_t. */

  redblackCacheDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  redblackCacheDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  redblackCacheDebugFunc_t debug;
  /**< Debugging message callback function. */

  redblackCacheCompFunc_t compare;
  /**< Callback function to compare two entries in the splay tree.  */

  redblackCacheEntry_t head;
  /**< First entry. This is a dummy list entry. It never stores a list
     entry data entry pointer. It simplifies list manipulations. The
     \e head.prev member is always \c NULL. The \e head.next member
     points to the first real list entry. If the list is empty then 
     \e head.next points to \e tail.prev . */

  redblackCacheEntry_t tail;
  /**< Last entry. Also a dummy list entry. The \e tail.next member
     is always \c NULL. The \e tail.prev member points to the last
     real list entry. If the list is empty then \e tail.prev
     points to \e head.next .*/

  redblackTree_t *tree;
  /**< Cache red-black tree. */

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
