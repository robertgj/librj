/** 
 * \file sgTree_private.h 
 *
 * Private definition for a scapegoat tree type. 
 */

#if !defined(SCAPEGOAT_TREE_PRIVATE_H)
#define SCAPEGOAT_TREE_PRIVATE_H

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
#include "sgTree.h"

/**
 * sgTree_t internal node. 
 *
 * Internal representation of a sgTree_t node.
 */
typedef struct sgTreeNode_t
{
  struct sgTreeNode_t *left;
  /**< Pointer to left-hand child of node. */

  struct sgTreeNode_t *right;
  /**< Pointer to right-hand child of node. */

  void *entry;
  /**< Pointer to caller's data. */

} sgTreeNode_t;

/**
 * sgTree_t structure.
 * 
 * Private implementation of sgTree_t.
 */
  
static const double sgTree_alpha = 0.4;
/**< Scapegoat tree weight imbalance threshold */

struct sgTree_t 
{
  sgTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for sgTree_t. */

  sgTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for sgTree_t. */

  sgTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  sgTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  sgTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  sgTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the tree.  */

  sgTreeNode_t *root;
  /**< Root entry in the tree. */

  stack_t *stack;
  /**< Stack for non-recursive implementation of sgTreeSizeNode(). */

  size_t size;
  /**< Number of entries in the sgTree. */

  size_t max_size;
  /**< Maximum size of the sgTree since the last time the tree was completely
   *    rebuilt.
   */

  bool rebalance_done;
  /**< If \c true , then the sub-tree has been rebalanced. */

  size_t subtree_size;
  /**< Size of the sub-tree at the current \c node. */

  size_t subtree_depth;
  /**< Depth of the sub-tree at the current \c node. */

  double alpha;
  /**< Factor triggering height or weight rebalancing. */
  
  double ln_alpha;
  /**< Natural log of alpha, for convenience . */
  
  bool use_alpha_weight_balance;
  /**< Use weight rebalancing if true, otherwise use height rebalancing. */
  
  bool use_recursize_size_node;
  /**< If true, use a recursive algorithm to to find the size of a node. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
