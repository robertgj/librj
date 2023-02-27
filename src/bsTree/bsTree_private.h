/** 
 * \file bsTree_private.h 
 *
 * Private definition for a balanced search tree type. 
 */

#if !defined(BALANCED_SEARCH_TREE_PRIVATE_H)
#define BALANCED_SEARCH_TREE_PRIVATE_H

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
#include "bsTree.h"

/**
 * bsTree_t internal node. 
 *
 * Internal representation of a bsTree_t node.
 */
typedef struct bsTreeNode_t
{
  struct bsTreeNode_t *left;
  /**< Pointer to left-hand child of node. */

  struct bsTreeNode_t *right;
  /**< Pointer to right-hand child of node. */

  size_t level;
  /**< Nodes at the bottom of the tree are at level 1 */

  void *entry;
  /**< Pointer to caller's data. */

} bsTreeNode_t;

/**
 * bsTree_t structure.
 * 
 * Private implementation of bsTree_t. 
 */
struct bsTree_t 
{
  bsTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for bsTree_t. */

  bsTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for bsTree_t. */

  bsTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  bsTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  bsTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  bsTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the tree.  */

  bsTreeNode_t *root;
  /**< Root entry in the tree. */

  bsTreeNode_t *bottom;
  /**< Sentinel at level 0 of the tree. */

  bsTreeNode_t *deleted, *last;
  /**< Global variables. */

  size_t size;
  /**< Number of entries in the bsTree. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
