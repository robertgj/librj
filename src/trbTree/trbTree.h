/**
 * \file trbTree.h
 *
 * Public interface for a threaded red-black tree type.
 */

#if !defined(TRB_TREE_H)
#define TRB_TREE_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdlib>
#include <cstdbool>
#include <cstddef>
using std::size_t;
extern "C" {
#else
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "compare.h"

/**
 * \e trbTree_t structure. An opaque type for a threaded red-black tree.
 */
typedef struct trbTree_t trbTree_t; 

/**
 * \e trbTree_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param size amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*trbTreeAllocFunc_t)(const size_t size, void * const user);

/**
 * \e trbTree_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param ptr pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*trbTreeDeallocFunc_t)(void * const ptr, void * const user);

/**
 * \e trbTree_t entry data memory duplicator callback function.
 *
 * Threaded red-black tree entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void *(*trbTreeDuplicateEntryFunc_t)(void * const entry, void * const user);

/**
 * \e trbTree_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*trbTreeDeleteEntryFunc_t)(void * const entry, void * const user);

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
typedef void (*trbTreeDebugFunc_t)(const char *function,
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
typedef compare_e (*trbTreeCompFunc_t)(const void * const a,
                                       const void * const b, 
                                       void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e trbTreeWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*trbTreeWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty threaded red-black tree.
 * 
 * Creates and initialises an empty \e trbTree_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e trbTree_t. \e NULL indicates failure
 */
trbTree_t *trbTreeCreate
(const trbTreeAllocFunc_t alloc, 
 const trbTreeDeallocFunc_t dealloc,
 const trbTreeDuplicateEntryFunc_t duplicateEntry,
 const trbTreeDeleteEntryFunc_t deleteEntry,
 const trbTreeDebugFunc_t debug,
 const trbTreeCompFunc_t comp,
 void * const user);

/**
 * Find an entry in the threaded red-black tree.
 *
 * Searches the threaded red-black tree for an entry. Typically, the entry passed 
 * will be a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param tree \e trbTree_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the tree.
 * \e NULL indicates failure to find the entry.
 */
void *trbTreeFind(trbTree_t * const tree, void * const entry);

/**
 * Insert an entry in the tree.
 *
 * Inserts an entry in the tree. The memory used by the entry is 
 * assumed to have been allocated by the caller. If the entry already 
 * exists and the \e deleteEntry() callback exists then the 
 * existing entry is deleted. If the entry already exists and the 
 * \e deleteEntry callback has not been defined then the existing entry 
 * is simply replaced. To replace an entry the caller must first 
 * remove the entry from the tree. The entry data pointer inserted is
 * returned.
 *
 * \param tree pointer to \e trbTree_t 
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry data in the tree. 
 * \e NULL indicates  failure.
 */
void *trbTreeInsert(trbTree_t * const tree, void * const entry);

/**
 * Remove an entry from the tree.
 *
 * Removes an entry in the tree. The memory used by entry is 
 * assumed to have been allocated by the caller. If the entry is
 * found and the \e deleteEntry()callback has been defined then that 
 * function will be called to deallocate the entry memory when the 
 * entry is removed.
 *
 * \param tree pointer to \e trbTree_t
 * \param entry \e void pointer to the caller's entry data
 * \return \e void pointer to the entry found in the tree. If the entry 
 * is found and \e deleteEntry() exists then NULL is returned. Otherwise
 * \e NULL is returned.
 */
void *trbTreeRemove(trbTree_t * const tree, void * const entry);

/**
 * Clear the threaded red-black tree.
 *
 * Clears all entries from the threaded red-black tree. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param tree \e trbTree_t pointer
 */
void trbTreeClear(trbTree_t * const tree);

/** 
 * Destroy the threaded red-black tree.
 *
 * Removes and deallocates all entries from the threaded red-black tree. If the 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param tree pointer to \e trbTree_t
 */
void trbTreeDestroy(trbTree_t * const tree);

/** 
 * Get the depth of the tree.
 *
 * Traverses the threaded red-black tree to find the maximum depth.
 *
 * \param tree pointer to \e trbTree_t
 * \return depth of the tree
 */
size_t trbTreeGetDepth(const trbTree_t * const tree);

/** 
 * Get the size of the threaded red-black tree.
 *
 * Returns the number of entries in the tree
 *
 * \param tree pointer to \e trbTree_t
 * \return number of entries in the tree
 */
size_t trbTreeGetSize(const trbTree_t * const tree);

/** 
 * Get the minimum entry in the threaded red-black tree.
 *
 * Returns a pointer to the minimum (leftmost) entry in the tree
 *
 * \param tree pointer to \e trbTree_t
 * \return \e void pointer to the minimum entry in the tree. 
 *         \e NULL if the tree is empty.
 */
void *trbTreeGetMin(trbTree_t * const tree);

/** 
 * Get the maximum entry in the threaded red-black  tree.
 *
 * Returns a pointer to the maximum (rightmost) entry in the tree.
 *
 * \param tree pointer to \e trbTree_t
 * \return \e void pointer to the maximum entry in the tree.
 *         \e NULL if the tree is empty.
 */
void *trbTreeGetMax(trbTree_t * const tree);

/** 
 * Get the next entry in the threaded red-black tree.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * next (rightwards) entry in the tree.
 *
 * \param tree pointer to \e trbTree_t
 * \param entry pointer to caller's data
 * \return \e void pointer to the next entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *trbTreeGetNext(trbTree_t * const tree, const void * const entry);

/** 
 * Get the previous entry in the threaded red-black tree.
 *
 * Given an entry pointer returns a pointer to the caller's data in the
 * previous (leftwards) entry in the tree.
 *
 * \param tree pointer to \e trbTree_t
 * \param entry \e void pointer to a callers entry data
 * \return \e void pointer to the previous entry in the tree.
 *         \e NULL if the tree is empty or entry is the leftmost.
 */
void *trbTreeGetPrevious(trbTree_t * const tree, const void * const entry);

/** 
 * Given an entry, get the next entry in the threaded red-black tree that is
 * higher or equal to the entry.
 *
 * Given an entry pointer returns a pointer to the higher or equal entry 
 * in the tree. The entry passed need not refer to an entry actually in the
 * tree.
 *
 * \param tree pointer to \e trbTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the higher or equal entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *trbTreeGetUpper(trbTree_t * const tree, const void * const entry);

/** 
 * Given an entry, get the previous entry in the threaded red-black tree
 * that is lesser or equal to the entry.
 *
 * Given an entry pointer returns a pointer to the lower or equal entry 
 * in the tree. The entry passed need not refer to an entry actually in the
 * tree.
 *
 * \param tree pointer to \e trbTree_t
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the lower or equal entry in the tree.
 *         \e NULL if the tree is empty or entry is the rightmost.
 */
void *trbTreeGetLower(trbTree_t * const tree, const void * const entry);

/** 
 * Operate on each tree entry.
 *
 * Traverses the tree in increasing order calling a function for each entry.
 *
 * \param tree pointer to \e trbTree_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating sucess
 */
bool trbTreeWalk(trbTree_t * const tree, const trbTreeWalkFunc_t walk);

/** 
 * Check the colour property of the tree.
 *
 * Traverses the tree checking that node colours are consistent ensuring
 * that the tree is in fact balanced.
 *
 * \param tree pointer to \e trbTree_t
 * \return \e bool indicating success. 
 */
bool trbTreeCheck(trbTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
