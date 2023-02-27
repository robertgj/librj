/** 
 * \file list.h 
 *
 * Public interface for a doubly linked list type.
 */

#if !defined(LIST_H)
#define LIST_H

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
 * \e list_t structure. An opaque type for a doubly linked list.
 */
typedef struct list_t list_t;

/**
 * \e list_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*listAllocFunc_t)(const size_t amount, void * const user);

/**
 * \e list_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*listDeallocFunc_t)(void * const pointer, void * const user);

/**
 * \e list_t entry data memory duplicator callback function.
 *
 * List entry duplicator call-back for caller's entry data.
 *
 * \param entry to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void * (*listDuplicateEntryFunc_t)(void * const entry, 
                                           void * const user);

/**
 * \e list_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*listDeleteEntryFunc_t)(void * const entry, void * const user);

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
typedef void (*listDebugFunc_t)(const char *function,  
                                const unsigned int line,
                                void * const user,
                                const char *format, 
                                ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries when searching in the list. 
 * The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*listCompFunc_t)(const void * const a, const void * const b,
                                    void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e listWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*listWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty list.
 * 
 * Creates and initialises an empty \e list_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for caller's entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e list_t. \e NULL indicates failure.
 */
list_t *listCreate(const listAllocFunc_t alloc, 
                   const listDeallocFunc_t dealloc, 
                   const listDuplicateEntryFunc_t duplicateEntry, 
                   const listDeleteEntryFunc_t deleteEntry, 
                   const listDebugFunc_t debug,
                   const listCompFunc_t comp,
                   void * const user);

/**
 * Find an entry in the list.
 *
 * Searches the list for an entry. Typically, the entry passed 
 * will be a pointer to a dummy entry declared automatic by the caller and
 * containing the requested entry. This entry might correspond to a simple
 * value or to the key of a key/value entry in a dictionary.
 *
 * \param list \e list_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the entry found in the list.
 * \e NULL indicates failure to find the entry.
 */
void *listFind(list_t * const list, const void * const entry);

/**
 * Insert an entry into the list.
 *
 * Allocates an entry and inserts it at the end of the list. Sets it
 * as the current entry. The address of the entry data inserted is
 * returned. If the list comparison function is defined then search
 * for an existing list entry equivalent to the argument entry. If the
 * entry already exists then the existing entry data pointer is
 * returned and that entry is not replaced. To replace an entry the
 * caller must first remove the entry from the list.
 *
 * \param list \e list_t pointer 
 * \param entry \e void pointer to caller's entry data 
 * \return \e void pointer to the entry data installed or, if the 
 * entry already exists, the existing entry data pointer. \e NULL 
 * indicates failure.
 */
void *listInsert(list_t * const list, void * const entry);

/**
 * Remove an entry from the list.
 *
 * Detaches an entry to the list and deallocates the entry. If 
 * the list \e deleteEntry() member exists then deallocates the caller's
 * entry data.
 *
 * \param list \e list_t pointer
 * \param entry \e void pointer to caller's entry data
 * \return \e void pointer to the existig entry data (subsequently free'd).
 * \e NULL indicates failure.
 */
void * listRemove(list_t * const list, void * const entry);

/**
 * Clear the list.
 *
 * Clears all entries from the list. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param list \e list_t pointer
 */
void listClear(list_t * const list);

/** 
 * Destroy the list.
 *
 * Detaches and deallocates all entries from the list. If the list 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param list \e list_t pointer
 */
void listDestroy(list_t * const list);

/**
 * Get the number of entries in the list.
 *
 * Returns the number of entries in the list.
 *
 * \param list \e list_t pointer
 * \return Number of entries in the list
 */
size_t listGetSize(const list_t * const list);

/**
 * Get the first entry data in the list.
 *
 * Returns a pointer to the first \e list_t instance entry data
 *
 * \param list \e list_t pointer
 * \return \e void pointer to the first entry data. \e NULL if the 
 * list has no entries
 */
void *listGetFirst(list_t * const list);

/**
 * Get the last entry data in the list.
 *
 * Returns a pointer to the last \e list_t instance entry data
 *
 * \param list \e list_t pointer
 * \return \e void pointer to the last entry data. \e NULL if the 
 * list has no entries
 */
void *listGetLast(list_t * const list);

/**
 * Get the next entry data in the list.
 *
 * Returns a pointer to the next \e list_t instance entry data
 *
 * \param list \e list_t pointer
 * \param entry \e void pointer to entry data
 * \return \e void pointer to the entry data after the entry. 
 * \e NULL if the current entry is the last or there are no entries.
 */
void *listGetNext(list_t * const list, const void * const entry);

/**
 * Get the previous entry data in the list.
 *
 * Returns a pointer to the previous \e list_t instance entry data
 *
 * \param list \e list_t pointer
 * \param entry \e void pointer to entry data
 * \return \e void pointer to the entry data before the entry. 
 * \e NULL if the entry is the first or  there are no entries.
 */
void *listGetPrevious(list_t * const list, const void * const entry);

/** 
 * Operate on each list entry.
 *
 * Traverses the list calling a function for each entry.
 *
 * \param list pointer to \e list_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating success
 */
bool listWalk(list_t * const list, const listWalkFunc_t walk);

/** 
 * Copy a list to another list.
 *
 * Copy a list to another list.
 *
 * \param dst pointer to destination \e list_t
 * \param src pointer to source \e list_t
 * \return \e list_t pointer \e dst. \e NULL indicates success
 */
list_t *listCopy(list_t * const dst, list_t * const src);

#ifdef __cplusplus
}
#endif

#endif
