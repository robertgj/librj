/** 
 * \file swTree_private.h 
 *
 * Private definition for a Stout/Warren tree type. 
 */

#if !defined(STOUT_WARREN_TREE_PRIVATE_H)
#define STOUT_WARREN_TREE_PRIVATE_H

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
#include "swTree.h"

/**
 * swTree_t internal node. 
 *
 * Internal representation of a swTree_t node.
 */
typedef struct swTreeNode_t
{
  struct swTreeNode_t *left;
  /**< Pointer to left-hand child of node. */

  struct swTreeNode_t *right;
  /**< Pointer to right-hand child of node. */

  void *entry;
  /**< Pointer to caller's data. */

} swTreeNode_t;

/**
 * swTree_t structure.
 * 
 * Private implementation of swTree_t. 
 */

static const size_t swTree_depth_factor = 6;
/**< Constant threshold on binary tree depth imbalance */
  
struct swTree_t 
{
  swTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for swTree_t. */

  swTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for swTree_t. */

  swTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  swTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  swTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  swTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the tree.  */

  swTreeNode_t *root;
  /**< Root entry in the tree. */

  size_t size;
  /**< Number of entries in the swTree. */

  size_t depth_factor;
  /**< Rebalance when current_depth > depth_factor+floor(lg(size)) */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
