/**
 * \file sgTree.h
 *
 * Public interface for a scapegoat tree type.
 *
 * From https://en.wikipedia.org/wiki/Scapegoat_tree :
\verbatim
A scapegoat tree is a self-balancing binary search tree, invented by Arne
Andersson[2] in 1989 and again by Igal Galperin and Ronald L. Rivest in 1993.[1]
It provides worst-case O(log n) lookup time (with n as the number of entries)
and O(log n) amortized insertion and deletion time.

Unlike most other self-balancing binary search trees which also provide worst
case O(lg n) lookup time, scapegoat trees have no additional
per-node memory overhead compared to a regular binary search tree: besides key
and value, a node stores only two pointers to the child nodes. This makes
scapegoat trees easier to implement and, due to data structure alignment, can
reduce node overhead by up to one-third.

Instead of the small incremental rebalancing operations used by most balanced
tree algorithms, scapegoat trees rarely but expensively choose a "scapegoat" and
completely rebuild the subtree rooted at the scapegoat into a complete binary
tree. Thus, scapegoat trees have O(n) worst-case update performance.
\endverbatim
 *
 * For \f$1/2<\alpha<1\f$, \e Galperin and \e Rivest [1, Section 3] define:
 *   1. \e max_size as the maximal size of the tree since the last
 *      time the tree was rebuilt.
 *   2. The \e height of a \e node is the length of longest path from
 *      that \e node to a leaf.
 *   3. The \e depth of a \e node is the length of the path from the
 *      \e root to that \e node.
 *   4. An \f$\alpha\f$-weight balanced sub-tree at \e node has
 *      \f$ size(node->left) \le \alpha size(node)\f$ and
 *      \f$ size(node->right) \le \alpha size(node)\f$.
 *   5. If \f$n=size(root)\f$, then
 *      \f$ h_{\alpha}(n) = \lfloor\log_{1/\alpha}(n)\rfloor\f$ and
 *      an \f$\alpha\f$-height balanced tree has \f$ h(n) \le h_{\alpha}(n)\f$.
 *
 * \e Galperin and \e Rivest [1, Section 4.2] propose two methods for finding
 * the \e scapegoat \e node after an insertion:
 *   1. Climb the tree until we find a \e node for which the sub-tree
 *      is not \f$\alpha\f$-weight balanced. Rebalance that sub-tree.
 *   2. Find the deepest ancestor sub-tree, \f$x_{i}\f$, of the
 *      \e node, for which \f$ i>h_{\alpha} size(x_{i})\f$.
 *      Rebalance that sub-tree.
 *
 * \e Galperin and \e Rivest [1, Section 4.3] suggest that after deleting
 * a \e node, the entire tree should be rebalanced if
 * \f$size(root) < \alpha  maxsize\f$.
 *
 * From [1], in Section 8, referring to Tables 4 and 5:
\verbatim
For uniformly distributed sequences our experi-
ments show that one can choose an alpha so that scapegoat
trees outperform red-black trees and splay trees on all
three operations. However, for the insertion of sorted se-
quences scapegoat trees are noticeably slower than the
other two data structures. Hence, in practical applica-
tions, it would be advisable to use scapegoat trees when
the inserted keys are expected to be roughly randomly
distributed, or when the application is search intensive.
\endverbatim
 *
 * Reference:
 *
 *   [1] "ScapegoatTrees", Igal Galperin and Ronald L. Rivest,
 *       https://people.csail.mit.edu/rivest/pubs/GR93.pdf
 */
#if !defined(SCAPEGOAT_TREE_H)
#define SCAPEGOAT_TREE_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdlib>
#include <cstdbool>
#include <cstddef>
extern "C" {
  using std::size_t;
#else
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"

  /**
   * \e sgTree_t structure. An opaque type for a scapegoat tree.
   */
  typedef struct sgTree_t sgTree_t; 

  /**
   * \e sgTree_t memory allocator.
   *
   * Memory allocation call-back.
   *
   * \param size amount of memory requested
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return pointer to the memory allocated. \e NULL indicates failure.
   */
  typedef void *(*sgTreeAllocFunc_t)(const size_t size, void * const user);

  /**
   * \e sgTree_t memory de-allocator.
   *
   * Memory deallocation call-back.
   *
   * \param ptr pointer to memory to be deallocated
   * \param user \e void pointer to user data to be echoed by callbacks
   */
  typedef void (*sgTreeDeallocFunc_t)(void * const ptr, void * const user);

  /**
   * \e sgTree_t entry data memory duplicator callback function.
   *
   * Scapegoat tree entry duplicator call-back for caller's entry data.
   *
   * \param entry pointer to memory to be duplicated
   * \param user \e void pointer to user data to be echoed by callbacks
   */
  typedef void *(*sgTreeDuplicateEntryFunc_t)(void * const entry,
                                              void * const user);

  /**
   * \e sgTree_t entry data memory de-allocator.
   *
   * Memory deallocation call-back for caller's entry data
   *
   * \param entry pointer to caller's memory to be deallocated
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e bool for compatibility with walk.
   */
  typedef bool (*sgTreeDeleteEntryFunc_t)(void * const entry, 
                                          void * const user);

  /**
   * Debugging message.
   *
   * Callback function to output a debugging message. Accepts a 
   * variable argument list like \e printf(). 
   *
   * \param function name
   * \param line number
   * \param user \e void pointer to user data to be echoed by callbacks
   * \param format string
   * \param ... variable argument list
   */
  typedef void (*sgTreeDebugFunc_t)(const char *function,
                                    const unsigned int line,
                                    void * const user,
                                    const char *format, 
                                    ...);

  /**
   * Compare two entries.
   * 
   * Callback function to compare two entries when searching in the tree. 
   * The entry type is defined by the caller.
   *
   * \param a pointer to an entry defined by the caller
   * \param b pointer to an entry defined by the caller
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e compare_e value
   */
  typedef compare_e (*sgTreeCompFunc_t)(const void * const a,
                                        const void * const b,
                                        void * const user);

  /**
   * Operate on entry.
   * 
   * Callback function run on an entry by \e sgTreeWalk().
   *
   * \param entry pointer to an entry defined by the caller
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return \e bool indicating success.
   */
  typedef bool (*sgTreeWalkFunc_t)(void * const entry, void * const user);

  /**
   * Create an empty scapegoat tree.
   * 
   * Creates and initialises an empty \e sgTree_t instance
   *
   * \param alloc memory allocator callback
   * \param dealloc memory deallocator callback
   * \param duplicateEntry entry duplication callback for caller's entry data
   * \param deleteEntry memory deallocator callback for callers entry data
   * \param debug message function callback
   * \param comp entry key comparison function callback
   * \param user \e void pointer to user data to be echoed by callbacks
   * \return pointer to a \e sgTree_t. \e NULL indicates failure
   */
  sgTree_t *sgTreeCreate
  (const sgTreeAllocFunc_t alloc, 
   const sgTreeDeallocFunc_t dealloc,
   const sgTreeDuplicateEntryFunc_t duplicateEntry,
   const sgTreeDeleteEntryFunc_t deleteEntry,
   const sgTreeDebugFunc_t debug,
   const sgTreeCompFunc_t comp,
   void * const user);

  /**
   * Find an entry in the scapegoat tree.
   *
   * Searches the scapegoat tree for an entry. Typically, the entry passed 
   * will be a pointer to a dummy entry declared automatic by the caller and
   * containing the requested entry. This entry might correspond to a simple
   * value or to the key of a key/value entry in a dictionary.
   *
   * \param tree \e sgTree_t pointer
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the entry found in the tree.
   * \e NULL indicates failure to find the entry.
   */
  void *sgTreeFind(sgTree_t * const tree, void * const entry);

  /**
   * Insert an entry in the tree.
   *
   * Inserts an entry in the tree. The memory used by the entry is
   * assumed to have been allocated by the caller. If the entry already
   * exists and the \e deleteEntry() callback exists then the
   * existing entry is deleted. If the entry already exists and the 
   * \e deleteEntry callback has not been defined then the existing entry
   * is simply replaced.  To replace an entry the caller must first
   * remove the entry from the tree. The entry data pointer inserted is
   * returned.
   *
   * \param tree pointer to \e sgTree_t 
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the entry data in the tree. 
   * \e NULL indicates  failure.
   */
  void *sgTreeInsert(sgTree_t * const tree, void * const entry);

  /**
   * Remove an entry from the tree.
   *
   * Removes an entry in the tree. The memory used by entry is 
   * assumed to have been allocated by the caller. If the entry is
   * found and the \e deleteEntry()callback has been defined then that 
   * function will be called to deallocate the entry memory when the 
   * entry is removed.
   *
   * \param tree pointer to \e sgTree_t
   * \param entry \e void pointer to the caller's entry data
   * \return \e void pointer to the entry found in the tree. If the entry 
   * is found and \e deleteEntry() exists then NULL is returned. Otherwise
   * \e NULL is returned.
   */
  void *sgTreeRemove(sgTree_t * const tree, void * const entry);

  /**
   * Clear the scapegoat tree.
   *
   * Clears all entries from the scapegoat tree. The memory used by entry is 
   * assumed to have been allocated by the caller. If the \e deleteEntry()
   * callback has been defined then that function will be called to 
   * deallocate the caller's entry memory when the entries are removed.
   *
   * \param tree \e sgTree_t pointer
   */
  void sgTreeClear(sgTree_t * const tree);

  /** 
   * Destroy the scapegoat tree.
   *
   * Removes and deallocates all entries from the scapegoat tree. If the 
   * \e deleteEntry() member exists then deallocates the caller's entry data.
   *
   * \param tree pointer to \e sgTree_t
   */
  void sgTreeDestroy(sgTree_t * const tree);

  /** 
   * Get the depth of the tree.
   *
   * Traverses the scapegoat tree to find the maximum depth.
   *
   * \param tree pointer to \e sgTree_t
   * \return depth of the tree
   */
  size_t sgTreeGetDepth(const sgTree_t * const tree);

  /** 
   * Get the size of the scapegoat tree.
   *
   * Returns the number of entries in the tree
   *
   * \param tree pointer to \e sgTree_t
   * \return number of entries in the tree
   */
  size_t sgTreeGetSize(const sgTree_t * const tree);

  /** 
   * Get the minimum entry in the scapegoat tree.
   *
   * Returns a pointer to the minimum (leftmost) entry in the tree
   *
   * \param tree pointer to \e sgTree_t
   * \return \e void pointer to the minimum entry in the tree. 
   *         \e NULL if the tree is empty.
   */
  void *sgTreeGetMin(sgTree_t * const tree);

  /** 
   * Get the maximum entry in the scapegoat tree.
   *
   * Returns a pointer to the maximum (rightmost) entry in the tree.
   *
   * \param tree pointer to \e sgTree_t
   * \return \e void pointer to the maximum entry in the tree.
   *         \e NULL if the tree is empty.
   */
  void *sgTreeGetMax(sgTree_t * const tree);

  /** 
   * Get the next entry in the scapegoat tree.
   *
   * Given an entry pointer returns a pointer to the caller's data in the
   * next (rightwards) entry in the tree.
   *
   * \param tree pointer to \e sgTree_t
   * \param entry pointer to caller's data
   * \return \e void pointer to the next entry in the tree.
   *         \e NULL if the tree is empty or entry is the rightmost.
   */
  void *sgTreeGetNext(sgTree_t * const tree, 
                      const void * const entry);

  /** 
   * Get the previous entry in the scapegoat tree.
   *
   * Given an entry pointer returns a pointer to the caller's data in the
   * previous (leftwards) entry in the tree.
   *
   * \param tree pointer to \e sgTree_t
   * \param entry \e void pointer to a callers entry data
   * \return \e void pointer to the previous entry in the tree.
   *         \e NULL if the tree is empty or entry is the leftmost.
   */
  void *sgTreeGetPrevious(sgTree_t * const tree,
                          const void * const entry);

  /** 
   * Given an entry, get the next entry in the scapegoat tree.
   *
   * Given an entry pointer returns a pointer to the next (rightwards) entry 
   * in the tree. The entry passed need not refer to an entry actually in the
   * tree.
   *
   * \param tree pointer to \e sgTree_t
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the next entry in the tree.
   *         \e NULL if the tree is empty or entry is the rightmost.
   */
  void *sgTreeGetUpper(sgTree_t * const tree, 
                       const void * const entry);

  /** 
   * Given an entry, get the previous entry in the scapegoat tree.
   *
   * Given an entry pointer returns a pointer to the previous (leftwards) entry 
   * in the tree. The entry passed need not refer to an entry actually in the
   * tree.
   *
   * \param tree pointer to \e sgTree_t
   * \param entry \e void pointer to caller's entry data
   * \return \e void pointer to the previous entry in the tree.
   *         \e NULL if the tree is empty or entry is the rightmost.
   */
  void *sgTreeGetLower(sgTree_t * const tree,
                       const void * const entry);

  /** 
   * Operate on each tree entry.
   *
   * Traverses the tree calling a function for each entry.
   *
   * \param tree pointer to \e sgTree_t
   * \param walk pointer to a callback function to be called for each entry
   * \return \e bool indicating sucess
   */
  bool sgTreeWalk(sgTree_t * const tree, 
                  const sgTreeWalkFunc_t walk);

  /** 
   * Check the consistency of the tree.
   *
   * Traverses the tree checking that nodes are consistent.
   *
   * \param tree pointer to \e sgTree_t
   * \return \e bool indicating success. 
   */
  bool sgTreeCheck(sgTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
