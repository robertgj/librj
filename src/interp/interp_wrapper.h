/**
 * \file interp_wrapper.h
 * 
 * Definition of "shim" wrapper functions between the test interpreter
 * and the data structure implementation and the utility functions provided
 * by the interpreter implementation. Each data structure implements the
 * appropriate subset of these wrapper functions.
 */

#if !defined(INTERP_WRAPPER_H)
#define INTERP_WRAPPER_H

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


/** @name Interpreter wrapper or shim functions
 */
/** @{ */

/**
 * Interpreter data structure create function.
 * 
 * Interpreter function to create a data structure. The data structure wrapper
 * function implementations of \e interpCreate() pass the interpreter callback
 * functions to the data structure implementation of the \e create function.
 * For some data structures (eg: redblackTreeCache) the \e create() function
 * accepts an argument specifying the initial size or number of entries. In 
 * these cases the \e interpCreate() wrapper function passes a fixed size of 128.
 * \return \e void pointer to the data structure created. NULL indicates failure.
 */
void *interpCreate(void);

/**
 * Interpreter data structure push function.
 * 
 * Interpreter function to push an entry to the data structure (eg: a stack). 
 * The interpreter allocates space for an integer entry and passes the address
 * to the data structure implementation.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to the entry
 * \return \e void pointer to the entry pushed
 */
void *interpPush(void * const pointer, void * const entry);

/**
 * Interpreter data structure pop function.
 * 
 * Interpreter function to pop an entry from the data structure (eg: a stack). 
 * The data structure implementation calls the interpreter \e deleteEntry 
 * callback function to deallocate the entry memory.
 * \param pointer \e void pointer to the data structure
 */
void interpPop(void * const pointer);

/**
 * Interpreter data structure peek function.
 * 
 * Interpreter function to peek at the entry at the top of the data structure
 * (eg: a stack). 
 * \param pointer \e void pointer to the data structure
 * \return \e void pointer to the entry at the top of the data structure
 */
void *interpPeek(void * const pointer);

/**
 * Interpreter data structure insert function.
 * 
 * Interpreter function to insert an entry into the data structure
 * (eg: a list). 
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to the entry to be inserted 
 * \return \e void pointer to the entry inserted into the data structure
 */
void *interpInsert(void * const pointer, void * const entry);

/**
 * Interpreter data structure remove function.
 * 
 * Interpreter function to remove an entry from the data structure
 * (eg: a list). The data structure implementation calls the interpreter
 * \e deleteEntry callback function to deallocate the entry memory.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to the entry to be removed
 * \return \e void pointer to the entry deleted (and free'd !)
 */
void * interpRemove(void * const pointer, void * const entry);

/**
 * Interpreter data structure clear function.
 * 
 * Interpreter function to remove all entries from the data structure.
 * The data structure implementation calls the interpreter
 * \e deleteEntry callback function to deallocate the memory allocated for
 * each entry.
 * \param pointer \e void pointer to the data structure
 */
void interpClear(void * const pointer);

/**
 * Interpreter data structure destroy function.
 * 
 * Interpreter function to remove all entries from the data structure and
 * then free the memory allocated for the data structure. The data structure
 * implementation calls the interpreter \e deleteEntry callback function to
 * deallocate the memory allocated for each entry.
 * \param pointer \e void pointer to the data structure
 */
void interpDestroy(void * const pointer);

/**
 * Interpreter data structure depth function.
 * 
 * Interpreter function to return the depth of a tree-like data structure
 * (eg: redblackTree_t).
 * \param pointer \e void pointer to the data structure
 * \return \e size_t depth of the tree
 */
size_t interpGetDepth(const void * const pointer);

/**
 * Interpreter data structure size function.
 * 
 * Interpreter function to return the number of entries in the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e size_t size of the data structure
 */
size_t interpGetSize(const void * const pointer);

/**
 * Interpreter data structure minimum function.
 * 
 * Interpreter function to return the minimum entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e void pointer to the minimum entry in the data structure
 */
void *interpGetMin(void * const pointer);

/**
 * Interpreter data structure maximum function.
 * 
 * Interpreter function to return the maximum entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e void pointer to the maximum entry in the data structure
 */
void *interpGetMax(void * const pointer);

/**
 * Interpreter data structure first function.
 * 
 * Interpreter function to return the first entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e void pointer to the first entry in the data structure
 */
void *interpGetFirst(void * const pointer);

/**
 * Interpreter data structure last function.
 * 
 * Interpreter function to return the last entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e void pointer to the last entry in the data structure
 */
void *interpGetLast(void * const pointer);

/**
 * Interpreter data structure next function.
 * 
 * Interpreter function to return the next entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to an entry in the data structure
 * \return \e void pointer to the next entry in the data structure
 */
void *interpGetNext(void * const pointer, const void * const entry);

/**
 * Interpreter data structure previous function.
 * 
 * Interpreter function to return the previous entry in the data structure.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to an entry in the data structure
 * \return \e void pointer to the previous entry in the data structure
 */
void *interpGetPrevious(void * const pointer, const void * const entry);

/**
 * Interpreter data structure upper function.
 * 
 * Interpreter function to return the entry in the data structure that is 
 * greater than or equal to the \e entry argument.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to an entry in the data structure
 * \return \e void pointer to the greater than or equal to entry in the
 *         data structure
 */
void *interpGetUpper(void * const pointer, const void * const entry);

/**
 * Interpreter data structure lower function.
 * 
 * Interpreter function to return the entry in the data structure that is 
 * less than or equal to the \e entry argument.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to an entry in the data structure
 * \return \e void pointer to the less than or equal to entry in the
 *         data structure
 */
void *interpGetLower(void * const pointer, const void * const entry);

/**
 * Interpreter data structure find function.
 * 
 * Interpreter function to return the entry in the data structure that is 
 * equal to the \e entry argument.
 * \param pointer \e void pointer to the data structure
 * \param entry \e void pointer to an entry in the data structure
 * \return \e void pointer to the entry that is equal to \e entry
 */
void *interpFind(void * const pointer, void * const entry);

/**
 * Interpreter data structure check function.
 * 
 * Interpreter function to check the state of the data structure (ie: if
 * the entries in a tree-like data structure satisfy the comparison relation).
 * \param pointer \e void pointer to the data structure
 * \return \e bool indicating success
 */
bool interpCheck(void * const pointer);

/**
 * Interpreter data structure walk function type definition.
 */
typedef bool (*interpWalkFunc_t)(void * const entry, void * const user);

/**
 * Interpreter data structure walk function.
 * 
 * Interpreter function to walk the data structure running a \e walk() function
 * on each entry.
 * \param pointer \e void pointer to the data structure
 * \param walk \e interpWalkFunc_t pointer to a function operating on each entry
 * \return \e bool indicating success
 */
bool interpWalk(void * const pointer, const interpWalkFunc_t walk);

/**
 * Interpreter data structure sort function.
 * 
 * Interpreter function to sort the data structure.
 * \param pointer \e void pointer to the data structure
 * \return \e bool indicating success
 */
bool interpSort(void * const pointer);

/**
 * Interpreter data structure balance function.
 * 
 * Interpreter function to balance the data structure (ie: rearrange a tree-like
 * data structure for least depth).
 * \param pointer \e void pointer to the data structure
 * \return \e bool indicating success
 */
bool interpBalance(void * const pointer);

/**
 * Interpreter data structure copy function.
 * 
 * Interpreter function to copy the contents of a data structure.
 * \param dst \e void pointer to the destination data structure
 * \param src \e void pointer to the source data structure
 * \return \e void pointer to the destination data structure
 */
void *interpCopy(void * const dst, void * const src);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
