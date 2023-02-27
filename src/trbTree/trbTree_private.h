/** 
 * \file trbTree_private.h 
 *
 * Private definition for a threaded Red-Black tree type. 
 */

#if !defined(TRB_TREE_PRIVATE_H)
#define TRB_TREE_PRIVATE_H

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
#include "trbTree.h"

/**
 * threaded red-black tree node colour. 
 *
 * Enumeration of threaded red-black tree node colour.
 */
typedef enum trbTreeColour_e 
  {
    trbTreeBlack = 0, trbTreeRed
  }
  trbTreeColour_e;

/**
 * Threaded red-black tree node tag.
 *
 * Enumeration of threaded red-black tree node tag for a node left or right
 * pointer to a child or a leaf. A "child" has a "real" entry. A "leaf" is a
 * pointer to the next or previous node in the thread. The "least" left leaf
 * and the "greatest" right leaf are NULL, indicating the start and end of
 * the thread, respectively.
 */
typedef enum trbTreeTag_e 
  {
    trbTreeLeaf = 0, trbTreeChild
  }
  trbTreeTag_e;

/**
 * \c trbTree_t internal node. 
 *
 * Internal representation of a \c trbTree_t node.
 */
typedef struct trbTreeNode_t
{
  void *entry;
  /**< Pointer to caller's data. */

  struct trbTreeNode_t *left;
  /**< Pointer to left-hand child of node. */

  struct trbTreeNode_t *right;
  /**< Pointer to right-hand child of node. */

  struct trbTreeNode_t *parent;
  /**< Pointer to parent of node. */

  trbTreeColour_e colour;
  /**< Colour of node. */

  trbTreeTag_e leftTag;
  /**< Tag of left-hand child of node. */

  trbTreeTag_e rightTag;
  /**< Tag of right-hand child of node. */
} trbTreeNode_t;

/**
 * \c trbTree_t structure.
 * 
 * Private implementation of \c trbTree_t. This implementation
 * uses a parent pointer in \c trbTreeNode_t rather than a
 * stack of parent nodes in \c trbTree_t. This has the advantage
 * that there is no need to re-allocate the parent stack of O(logN) if the 
 * maximum size is exceeded but it uses an extra pointer in each node
 * so will use O(N) extra memory. The \c current entry is used to avoid 
 * repeated searches by next/previous operations. 
 */
struct trbTree_t 
{
  trbTreeAllocFunc_t alloc; 
  /**< Memory allocator callback function for \c trbTree_t. */

  trbTreeDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for \c trbTree_t. */

  trbTreeDuplicateEntryFunc_t duplicateEntry;
  /**< Entry duplication callback function for the caller's entry data. */

  trbTreeDeleteEntryFunc_t deleteEntry;
  /**< Memory deallocator callback function for the caller's entry data. */

  trbTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  trbTreeCompFunc_t compare;
  /**< Callback function to compare two entries in the tree.  */

  trbTreeNode_t *root;
  /**< Root entry in the tree. */

  trbTreeNode_t *current;
  /**< Last entry accessed in the tree. */

  size_t size;
  /**< Number of entries in the threaded red-black tree. */

  void *user;
  /**< Placeholder for user data in callbacks. */
};

#ifdef __cplusplus
}
#endif

#endif
