/** 
 * \file list_private.h 
 *
 * Private definition for a doubly linked list type. Required for the KLEE
 * analyser.
 */

#if !defined(LIST_PRIVATE_H)
#define LIST_PRIVATE_H

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
#include "list.h"

/**
 * \c list_t entry type. Internal representation of a \c list_t entry.
 */
typedef struct listEntry_t
{
  struct listEntry_t *next; 
  /**< Pointer to the next entry. */

  struct listEntry_t *prev; 
  /**< Pointer to the previous entry. */

  void *entry; 
  /**< Pointer to the caller's entry data. 
     This memory was allocated by the caller. */
} listEntry_t;

/**
 * \c list_t structure.
 *
 *  A doubly linked list type.
 */
struct list_t
{
  listAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c list_t. */

  listDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function for \c list_t. */

  listDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  listDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  listDebugFunc_t debug;
  /**< Debugging message callback function. */

  listCompFunc_t compare;
  /**< Callback function to compare two entries in the list.  */

  listEntry_t head;
  /**< First entry. This is a dummy list entry. It never stores a
     list entry data entry pointer. It simplifies list
     manipulations. The \e head.prev member is always \c NULL. The 
     \e head.next member points to the first real list entry. If the
     list is empty then \e head.next points to \e tail.prev . */

  listEntry_t tail;
  /**< Last entry. Also a dummy list entry. The \e tail.next member
     is always \c NULL. The \e tail.prev member points to the last
     real list entry. If the list is empty then \e tail.prev
     points to \e head.next .*/

  listEntry_t *current;
  /**< Pointer to the last list entry accessed. 
     Used to speed up next/previous operations. */

  size_t size;
  /**< Number of entries in the list. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
