/** 
 * \file splayTree.h 
 *
 * Public interface for a splay tree type.
 *
 * A splay tree is a self-organizing data structure.  Every operation
 * on the tree causes a splay to happen.  The splay moves the requested
 * node to the root of the tree and partly rebalances it. This has the 
 * benefit that request locality causes faster lookups as the requested 
 * nodes move to the top of the tree.  On the other hand, every lookup 
 * causes memory writes.
 *
 * The Balance Theorem bounds the expected total access time for m 
 * operations and n inserts on an initially empty tree as O((m + n)lg n)
 * so the amortized cost for a sequence of m accesses to a splay tree is 
 * O(lg n). However, if the tree is accessed in order (by calls to 
 * \e splayTreeNext() for example) then the tree will be reordered to look 
 * much like a linked list. Consequently, the worst case performance of 
 * the splay tree is O(n).
 * 
 *  Reference: "Self-Adjusting Binary Search Trees", Sleator, D.D., and 
 *  Tarjan, R.E., Journal of the Association for Computing Machinery
 *  Vol. 32, No. 3, July 1985, pp. 652-686.
 */

#if !defined(SPLAY_TREE_H)
#define SPLAY_TREE_H


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
/**
 * \e splayTree_t structure. An opaque type for a splay tree.
 */
typedef struct splayTree_t splayTree_t;

/**
 * \e splayTree_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*splayTreeAllocFunc_t)(const size_t amount, void * const user);

/**
 * \e splayTree_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*splayTreeDeallocFunc_t)(void *pointer, void * const user);

/**
 * \e splayTree_t entry data memory duplicator callback function.
 *
 * Splay tree entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void *(*splayTreeDuplicateEntryFunc_t)(void * const entry, 
                                               void * const user);

/**
 * \e splayTree_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data. 
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatility with \e splayTreeWalk().
 */
typedef bool (*splayTreeDeleteEntryFunc_t)(void * const entry, 
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
typedef void (*splayTreeDebugFunc_t)(const char *function,
                                     const unsigned int line,
                                     void * const user,
                                     const char *format, 
                                     ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries in the splay tree. Used to
 * choose between left and right nodes in the tree. The entry type is
 * defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*splayTreeCompFunc_t)(const void * const a,
                                         const void * const b,
                                         void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e splayTreeWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*splayTreeWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty splay tree.
 * 
 * Creates and initialises an empty \e splayTree_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e splayTree_t. \e NULL indicates failure.
 */
splayTree_t *splayTreeCreate(const splayTreeAllocFunc_t alloc, 
                             const splayTreeDeallocFunc_t dealloc,
                             const splayTreeDuplicateEntryFunc_t duplicateEntry,
                             const splayTreeDeleteEntryFunc_t deleteEntry,
                             const splayTreeDebugFunc_t debug,
                             const splayTreeCompFunc_t comp,
                             void * const user);

/**
 * Find an entry in the tree.
 *
 * Searches the tree for an entry. Typically, the entry passed will be
 * a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param tree pointer to \e splayTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the tree. 
 * \e NULL indicates failure to find the entry.
 */
void *splayTreeFind(splayTree_t * const tree, void * const entry);

/**
 * Insert an entry in the tree.
 *
 * Inserts an entry in the tree. The memory used by the entry pointer
 * is assumed to have been allocated by the caller. If the entry
 * already exists then a pointer to the existing entry is returned and
 * that entry is not replaced. To replace an entry the caller must
 * first remove the entry from the tree.
 *
 * \param tree pointer to \e splayTree_t 
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data inserted in the tree. 
 * \e NULL indicates  failure.
 */
void *splayTreeInsert(splayTree_t * const tree, void * const entry);

/**
 * Remove an entry from the tree.
 *
 * Splays to then removes an entry in the tree. The memory used by the
 * entry is assumed to have been allocated by the caller. If the entry
 * is found and the \e deleteEntry()callback has been defined then
 * that function will be called to deallocate the entry memory when
 * the entry is removed.
 *
 * \param tree pointer to \e splayTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the tree. If entry 
 * is found and \e deleteEntry() exists then NULL is returned. Otherwise
 * \e NULL.
 */
void *splayTreeRemove(splayTree_t * const tree, void * const entry);

/**
 * Clear the splay tree.
 *
 * Clears all entries from the splay tree. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param tree \e splayTree_t pointer
 */
void splayTreeClear(splayTree_t * const tree);

/** 
 * Recursively destroy the splay tree.
 *
 * Detaches and deallocates all entries in the splay tree. If the 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param tree pointer to \e splayTree_t
 */
void splayTreeDestroy(splayTree_t * const tree);

/** 
 * Get the depth of the tree.
 *
 * Traverses the splay tree from the root to find the maximum depth.
 *
 * \param tree pointer to \e splayTree_t
 * \return depth of the tree
 */
size_t splayTreeGetDepth(const splayTree_t * const tree);

/** 
 * Get the size of the splay tree.
 *
 * Returns the number of entries in the tree
 *
 * \param tree pointer to \e splayTree_t
 * \return number of entries in the tree
 */
size_t splayTreeGetSize(const splayTree_t * const tree);

/** 
 * Get the minimum entry in the splay tree.
 *
 * Returns a pointer to the minimum (leftmost) entry 
 * in the tree. Doesn't splay to this entry.
 *
 * \param tree pointer to \e splayTree_t
 * \return \e void pointer to the minimum entry in the tree. 
 *         \e NULL if the tree is empty.
 */
void *splayTreeGetMin(splayTree_t * const tree);

/** 
 * Get the maximum entry in the splay  tree.
 *
 * Returns a pointer to the maximum (rightmost) entry 
 * in the tree. Doesn't splay to this entry.
 *
 * \param tree pointer to \e splayTree_t
 * \return \e void pointer to the maximum entry in the tree.
 *         \e NULL if the tree is empty.
 */
void *splayTreeGetMax(splayTree_t * const tree);

/** 
 * Get the next entry in the splay tree.
 *
 * Given an entry splays to that entry then splays to and returns a
 * pointer to the next (rightwards) entry in the tree.
 *
 * \param tree pointer to \e splayTree_t
 * \param entry \e void pointer to an entry
 * \return \e void pointer to the next entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *splayTreeGetNext(splayTree_t * const tree, const void * const entry);

/** 
 * Get the previous entry in the splay tree.
 *
 * Given an entry splays to that entry then splays to and returns a
 * pointer to the previous (leftwards) entry in the tree.
 *
 * \param tree pointer to \e splayTree_t
 * \param entry \e void pointer to an entry
 * \return \e void pointer to the previous entry in the tree.
 *         \e NULL if the tree is empty or entry is the leftmost.
 */
void *splayTreeGetPrevious(splayTree_t * const tree, const void * const entry);

/** 
 * Recrsively walk the tree from the root operating on each tree entry.
 *
 * Traverses the tree from the root downwards to each node calling a 
 * function for each entry. Does not splay to alter tree structure.
 *
 * \param tree pointer to \e splayTree_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating success
 */
bool splayTreeWalk(splayTree_t * const tree, const splayTreeWalkFunc_t walk);

/** 
 * Recursively walk the tree checking the node order.
 *
 * Traverses the tree from the root downwards to each node checking that 
 * node order is consistent. Does not splay to alter tree structure.
 *
 * \param tree pointer to \e splayTree_t
 * \return \e bool indicating success. 
 */
bool splayTreeCheck(splayTree_t * const tree);

/** 
 * Rebalance the tree.
 *
 * First converts the tree to look like a linked list by accessing nodes 
 * in order. Next balances the resulting list-like tree by multiple passes
 * moving alternate nodes to the root. The number of passes is about
 * log base 2 of the number of entries.
 *
 * \param tree pointer to \e splayTree_t
 * \return \e bool indicating success. 
 */
bool splayTreeBalance(splayTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
