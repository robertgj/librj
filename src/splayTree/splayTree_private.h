/** 
 * \file splayTree_private.h 
 *
 * Private definition for a splay-tree type. 
 */

#if !defined(SPLAY_TREE_PRIVATE_H)
#define SPLAY_TREE_PRIVATE_H

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
#include "splayTree.h"
  
/**
 * \c splayTree_t internal node type. 
 *
 * Internal representation of a splayTree_t entry.
 */
typedef struct splayTreeNode_t
{
  void *entry; 
  /**<Pointer to caller's entry data. 
     This memory was allocated by the caller. */

  struct splayTreeNode_t *left; 
  /**< Pointer to the right child of the node */

  struct splayTreeNode_t *right; 
  /**< Pointer to the left child of the node */
}
  splayTreeNode_t;

/**
 * \c splayTree_t structure.
 * 
 * A splay tree type.
 */
struct splayTree_t 
{
  splayTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c splayTree_t. */

  splayTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for \c splayTree_t. */

  splayTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  splayTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory allocator callback function for caller's entry data. */

  splayTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  splayTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the splay tree.  */

  splayTreeNode_t *root;
  /**< Tree root entry. */

  size_t size;
  /**< Number of entries in the tree. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
