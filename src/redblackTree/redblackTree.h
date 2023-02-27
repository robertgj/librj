/**
 * \file redblackTree.h
 *
 * Public interface for a red-black tree type.
 *
 * From the <a href="http://en.wikipedia.org/wiki/Red-black_tree">Wikipedia</a>
 * entry:
 *
 * <BLOCKQUOTE>A red-black tree is a binary search tree where each node has 
 * a color attribute, the value of which is either red or black. 
 * In addition to the ordinary requirements imposed on binary search 
 * trees, we make the following additional requirements of any valid 
 * red-black tree:
 *
 *  1. A node is either red or black.\n
 *  2. The root is black.\n
 *  3. All leaves are black.\n
 *  4. Both children of every red node are black.\n
 *  5. All paths from any given node to its leaf nodes contain the 
 *     same number of black nodes.
 *
 * These constraints enforce a critical property of red-black trees: 
 * that the longest possible path from the root to a leaf is no more 
 * than twice as long as the shortest possible path. The result is that 
 * the tree is roughly balanced. Since operations such as inserting,
 * deleting, and finding values requires worst-case time proportional 
 * to the height of the tree, this theoretical upper bound on the 
 * height allows red-black trees to be efficient in the worst-case, 
 * unlike ordinary binary search trees. 
 *
 * To see why these properties guarantee this, it suffices to note
 * that no path can have two red nodes in a row, due to property 4. 
 * The shortest possible path has all black nodes, and the longest
 * possible path alternates between red and black nodes. Since all
 * maximal paths have the same number of black nodes, by property 5,
 * this shows that no path is more than twice as long as any other
 * path.
 *
 * In many presentations of tree data structures, it is possible for
 * a node to have only one child, and leaf nodes contain data. It is
 * possible to present red-black trees in this paradigm, but it
 * changes several of the properties and complicates the algorithms.
 * For this reason, we use "null leaves", which contain no data and
 * merely serve to indicate where the tree ends.  A consequence of
 * this is that all internal (non-leaf) nodes have two children,
 * although one or more of those children may be a null leaf. A 
 * threaded red-black tree uses the pointer space in the leaf nodes
 * to build an ordered linked list of internal nodes. An extra tag
 * is required for each node to distinguish between internal and 
 * thread nodes.</BLOCKQUOTE>
 *
 * <a href="http://en.wikipedia.org/wiki/Wikipedia:Text_of_the_GNU_Free_Documentation_License">GNU Free Documentation License</a>
 */

#if !defined(REDBLACK_TREE_H)
#define REDBLACK_TREE_H


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
 * \e redblackTree_t structure. An opaque type for a red-black tree.
 */
typedef struct redblackTree_t redblackTree_t; 

/**
 * \e redblackTree_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param size amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*redblackTreeAllocFunc_t)(const size_t size, void * const user);

/**
 * \e redblackTree_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param ptr pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*redblackTreeDeallocFunc_t)(void * const ptr, void * const user);

/**
 * \e redblackTree_t entry data memory duplicator callback function.
 *
 * Red-black tree entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void *(*redblackTreeDuplicateEntryFunc_t)(void * const entry,
                                                  void * const user);

/**
 * \e redblackTree_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*redblackTreeDeleteEntryFunc_t)(void * const entry, 
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
typedef void (*redblackTreeDebugFunc_t)(const char *function,
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
typedef compare_e (*redblackTreeCompFunc_t)(const void * const a,
                                            const void * const b,
                                            void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e redblackTreeWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*redblackTreeWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty red-black tree.
 * 
 * Creates and initialises an empty \e redblackTree_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e redblackTree_t. \e NULL indicates failure
 */
redblackTree_t *redblackTreeCreate
(const redblackTreeAllocFunc_t alloc, 
 const redblackTreeDeallocFunc_t dealloc,
 const redblackTreeDuplicateEntryFunc_t duplicateEntry,
 const redblackTreeDeleteEntryFunc_t deleteEntry,
 const redblackTreeDebugFunc_t debug,
 const redblackTreeCompFunc_t comp,
 void * const user);

/**
 * Find an entry in the red-black tree.
 *
 * Searches the red-black tree for an entry. Typically, the entry passed 
 * will be a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param tree \e redblackTree_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the tree.
 * \e NULL indicates failure to find the entry.
 */
void *redblackTreeFind(redblackTree_t * const tree, void * const entry);

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
 * \param tree pointer to \e redblackTree_t 
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data in the tree. 
 * \e NULL indicates  failure.
 */
void *redblackTreeInsert(redblackTree_t * const tree, void * const entry);

/**
 * Remove an entry from the tree.
 *
 * Removes an entry in the tree. The memory used by entry is 
 * assumed to have been allocated by the caller. If the entry is
 * found and the \e deleteEntry()callback has been defined then that 
 * function will be called to deallocate the entry memory when the 
 * entry is removed.
 *
 * \param tree pointer to \e redblackTree_t
 * \param entry \e void pointer to the caller's entry data
 * \return \e void pointer to the entry found in the tree. If the entry 
 * is found and \e deleteEntry() exists then NULL is returned. Otherwise
 * \e NULL is returned.
 */
void *redblackTreeRemove(redblackTree_t * const tree, void * const entry);

/**
 * Clear the red-black tree.
 *
 * Clears all entries from the red-black tree. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param tree \e redblackTree_t pointer
 */
void redblackTreeClear(redblackTree_t * const tree);

/** 
 * Destroy the red-black tree.
 *
 * Removes and deallocates all entries from the red-black tree. If the 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param tree pointer to \e redblackTree_t
 */
void redblackTreeDestroy(redblackTree_t * const tree);

/** 
 * Get the depth of the tree.
 *
 * Traverses the red-black tree to find the maximum depth.
 *
 * \param tree pointer to \e redblackTree_t
 * \return depth of the tree
 */
size_t redblackTreeGetDepth(const redblackTree_t * const tree);

/** 
 * Get the size of the red-black tree.
 *
 * Returns the number of entries in the tree
 *
 * \param tree pointer to \e redblackTree_t
 * \return number of entries in the tree
 */
size_t redblackTreeGetSize(const redblackTree_t * const tree);

/** 
 * Get the minimum entry in the red-black tree.
 *
 * Returns a pointer to the minimum (leftmost) entry in the tree
 *
 * \param tree pointer to \e redblackTree_t
 * \return \e void pointer to the minimum entry in the tree. 
 *         \e NULL if the tree is empty.
 */
void *redblackTreeGetMin(redblackTree_t * const tree);

/** 
 * Get the maximum entry in the red-black  tree.
 *
 * Returns a pointer to the maximum (rightmost) entry in the tree.
 *
 * \param tree pointer to \e redblackTree_t
 * \return \e void pointer to the maximum entry in the tree.
 *         \e NULL if the tree is empty.
 */
void *redblackTreeGetMax(redblackTree_t * const tree);

/** 
 * Get the next entry in the red-black tree.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * next (rightwards) entry in the tree.
 *
 * \param tree pointer to \e redblackTree_t
 * \param entry pointer to caller's data
 * \return \e void pointer to the next entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *redblackTreeGetNext(redblackTree_t * const tree, 
                          const void * const entry);

/** 
 * Get the previous entry in the red-black tree.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * previous (leftwards) entry in the tree.
 *
 * \param tree pointer to \e redblackTree_t
 * \param entry \e void pointer to a callers entry data
 * \return \e void pointer to the previous entry in the tree.
 *         \e NULL if the tree is empty or entry is the leftmost.
 */
void *redblackTreeGetPrevious(redblackTree_t * const tree,
                              const void * const entry);

/** 
 * Given an entry, get the next entry in the red-black tree.
 *
 * Given an entry pointer returns a pointer to the next (rightwards) entry 
 * in the tree. The entry passed need not refer to an entry actually in the
 * tree.
 *
 * \param tree pointer to \e redblackTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the next entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *redblackTreeGetUpper(redblackTree_t * const tree, 
                           const void * const entry);

/** 
 * Given an entry, get the previous entry in the red-black tree.
 *
 * Given an entry pointer returns a pointer to the previous (leftwards) entry 
 * in the tree. The entry passed need not refer to an entry actually in the
 * tree.
 *
 * \param tree pointer to \e redblackTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the previous entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *redblackTreeGetLower(redblackTree_t * const tree,
                           const void * const entry);

/** 
 * Operate on each tree entry.
 *
 * Traverses the tree in increasing order calling a function for each entry.
 *
 * \param tree pointer to \e redblackTree_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating sucess
 */
bool redblackTreeWalk(redblackTree_t * const tree, 
                      const redblackTreeWalkFunc_t walk);

/** 
 * Check the colour property of the tree.
 *
 * Traverses the tree checking that node colours are consistent ensuring
 * that the tree is in fact balanced.
 *
 * \param tree pointer to \e redblackTree_t
 * \return \e bool indicating success. 
 */
bool redblackTreeCheck(redblackTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
