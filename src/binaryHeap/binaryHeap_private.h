/** 
 * \file binaryHeap_private.h 
 *
 * Private definition for a binary heap type. 
 */

#if !defined(BINARY_HEAP_PRIVATE_H)
#define BINARY_HEAP_PRIVATE_H

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
#include "binaryHeap.h"

/**
 * \e binaryHeap_t structure.
 *
 *  A binary heap type.
 */
struct binaryHeap_t
{
  binaryHeapAllocFunc_t alloc; 
  /**< Memory allocator callback function for \e binaryHeap_t. */

  binaryHeapDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function for \e binaryHeap_t. */

  binaryHeapDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  binaryHeapDeleteEntryFunc_t deleteEntry;
  /**< Memory de-allocator callback function for the caller's entry data. */

  binaryHeapDebugFunc_t debug;
  /**< Debugging message callback function. */

  binaryHeapCompFunc_t compare;
  /**< Callback function to compare two entries in the binary heap.  */

  void **heap;
  /**< Array of pointers to the heap entries */

  size_t size;
  /**< Number of entries in the binary heap. */

  size_t space;
  /**< Number of possible entries in the binary heap. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};


#ifdef __cplusplus
}
#endif

#endif
