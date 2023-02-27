/** 
 * \file skipList_private.h 
 *
 * Private definition for a skip-list type. 
 */

#if !defined(SKIP_LIST_PRIVATE_H)
#define SKIP_LIST_PRIVATE_H

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
#include "skipList.h"

/**
 * \e skipList_t structure.
 * 
 * Private implementation of \e skipList_t. Refers to an instance of the
 * \e jsw_skip_t skip list.
 */
struct skipList_t 
{
  skipListAllocFunc_t alloc; 
  /**< Memory allocator callback function for skipList_t. */

  skipListDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for skipList_t. */

  skipListDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  skipListDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  skipListDebugFunc_t debug;
  /**< Debugging message callback function. */

  skipListCompFunc_t compare;
  /**< Callback function to compare two entries in the skip list.  */

  jsw_skip_t *skip;
  /**< A pointer to a instance Julienne Walker's skip list data structure */

  size_t space;
  /**< Maximum size of skip list */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
