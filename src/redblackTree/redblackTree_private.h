/** 
 * \file redblackTree_private.h 
 *
 * Private definition for a Red-Black tree type. 
 */

#if !defined(REDBLACK_TREE_PRIVATE_H)
#define REDBLACK_TREE_PRIVATE_H

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

/**
 * red-black tree node colour. 
 *
 * Enumeration of red-black tree node colour.
 */
typedef enum redblackTreeColour_e 
  {
    redblackTreeBlack = 0, redblackTreeRed
  }
  redblackTreeColour_e;

/**
 * redblackTree_t internal node. 
 *
 * Internal representation of a redblackTree_t node.
 */
typedef struct redblackTreeNode_t
{
  void *entry;
  /**< Pointer to caller's data. */

  struct redblackTreeNode_t *left;
  /**< Pointer to left-hand child of node. */

  struct redblackTreeNode_t *right;
  /**< Pointer to right-hand child of node. */

  struct redblackTreeNode_t *parent;
  /**< Pointer to parent of node. */

  redblackTreeColour_e colour;
  /**< Colour of node. */
} redblackTreeNode_t;

/**
 * redblackTree_t structure.
 * 
 * Private implementation of redblackTree_t. This implementation
 * uses a parent pointer in redblackTreeNode_t rather than a
 * stack of parent nodes in redblackTree_t. This has the advantage
 * that there is no need to re-allocate the parent stack of O(logN) if the 
 * maximum size is exceeded but it uses an extra pointer in each node
 * so will use O(N) extra memory. The \e current entry is used to avoid 
 * repeated searches by next/previous operations. 
 */
struct redblackTree_t 
{
  redblackTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for redblackTree_t. */

  redblackTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for redblackTree_t. */

  redblackTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  redblackTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  redblackTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  redblackTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the tree.  */

  redblackTreeNode_t *root;
  /**< Root entry in the tree. */

  redblackTreeNode_t *current;
  /**< Last entry accessed in the tree. */

  size_t size;
  /**< Number of entries in the redblackTree. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
