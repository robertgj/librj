/** 
 * \file stack_private.h 
 *
 * Private definition for a stack type. 
 */

#if !defined(STACK_PRIVATE_H)
#define STACK_PRIVATE_H

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
#include "stack.h"
 
/** \c stackEntry_t stack entry type */
typedef void* stackEntry_t;

/**
 * \c stack_t structure.
 *
 *  A stack type. The stack top grows upward from \e bottom.
 */
struct stack_t
{
  stackAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c stack_t. */

  stackDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function for \c stack_t. */

  stackDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  stackDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  stackDebugFunc_t debug;
  /**< Debugging message callback function. */

  void *user;
  /**< Placeholder for user data in callbacks. */

  size_t space;
  /**< Maximum number of entries in the stack. */

  stackEntry_t *bottom;
  /**< Pointer to the bottom of the stack of \c stackEntry_t pointers. */ 

  size_t size;
  /**< Number of entries in the stack. */
};

#ifdef __cplusplus
}
#endif

#endif
