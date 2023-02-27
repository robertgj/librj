/** 
 * \file binaryHeap.h 
 *
 * Public interface for a binary heap type.
 */

#if !defined(BINARY_HEAP_H)
#define BINARY_HEAP_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdarg>
#include <cstdbool>
using std::size_t;
extern "C" {
#else
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#endif

#include "compare.h"

/**
 * \e binaryHeap_t structure. An opaque type for a binary heap.
 */
typedef struct binaryHeap_t binaryHeap_t;

/**
 * \e binaryHeap_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void *(*binaryHeapAllocFunc_t)(const size_t amount, void * const user);

/**
 * \e binaryHeap_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*binaryHeapDeallocFunc_t)(void * const pointer, void * const user);

/**
 * \e binaryHeap_t entry data memory duplicator callback function.
 *
 * Binary heap entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void * (*binaryHeapDuplicateEntryFunc_t)(void * const entry, 
                                                 void * const user);

/**
 * \e binaryHeap_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data.
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with \e walk.
 */
typedef bool (*binaryHeapDeleteEntryFunc_t)(void * const entry, 
                                            void * const user);

/**
 * Debugging message.
 *
 * Callback function to output a debugging message. Accepts a 
 * variable argument list. 
 *
 * \param function name
 * \param line number
 * \param user \e void pointer to user data to be echoed by callbacks
 * \param format string
 * \param ... variable argument list
 */
typedef void (*binaryHeapDebugFunc_t)(const char *function,  
                                      const unsigned int line,
                                      void * const user,
                                      const char *format, ...);

/**
 * Compare two entries.
 * 
 * Callback function to compare two entries when searching in the binary heap. 
 * The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e compare_e value
 */
typedef compare_e (*binaryHeapCompFunc_t)(const void * const a, 
                                          const void * const b, 
                                          void * const user);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e binaryHeapWalk().
 *
 * \param entry pointer to an entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*binaryHeapWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty binaryHeap.
 * 
 * Creates and initialises an empty \e binaryHeap_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for caller's entry data
 * \param debug message function callback
 * \param comp entry key comparison function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e binaryHeap_t. \e NULL indicates failure.
 */
binaryHeap_t *binaryHeapCreate
(const binaryHeapAllocFunc_t alloc, 
 const binaryHeapDeallocFunc_t dealloc, 
 const binaryHeapDuplicateEntryFunc_t duplicateEntry, 
 const binaryHeapDeleteEntryFunc_t deleteEntry, 
 const binaryHeapDebugFunc_t debug,
 const binaryHeapCompFunc_t comp,
 void * const user);

/**
 * Push an entry onto the binary heap.
 *
 * Allocates an entry and pushes it onto the binary heap. The address of the
 * entry data inserted is returned. 
 *
 * \param binaryHeap \e binaryHeap_t pointer 
 * \param entry \e void pointer to caller's entry data 
 * \return \e void_t pointer to the entry data installed or, if the 
 * entry already exists, the existing entry data pointer. \e NULL 
 * indicates failure.
 */
void *binaryHeapPush(binaryHeap_t * const binaryHeap, 
                     void * const entry);

/**
 * Copy the root entry in the binary heap.
 *
 * Copies the root entry in the binary heap.
 *
 * \param binaryHeap \e binaryHeap_t pointer 
 * \return pointer to \e entry. \e NULL indicates the heap is empty.
 */
void *binaryHeapPeek(binaryHeap_t * const binaryHeap);

/**
 * Pop an entry from the binary heap.
 *
 * Removes the root entry from the binary heap and then balances the heap. If 
 * the \e deleteEntry() callback has been defined then that function will be
 * called to deallocate the caller's entry memory and NULL will be returned.
 *
 * \param binaryHeap \e binaryHeap_t pointer
 * \return pointer to popped entry data. NULL if the entry memory has been
 * deallocated.
 */
void *binaryHeapPop(binaryHeap_t * const binaryHeap);

/**
 * Clear the binaryHeap.
 *
 * Clears all entries from the binary heap. The memory used by entry is 
 * assumed to have been allocated by the caller. If the \e deleteEntry()
 * callback has been defined then that function will be called to 
 * deallocate the caller's entry memory when the entries are removed.
 *
 * \param binaryHeap \e binaryHeap_t pointer
 */
void binaryHeapClear(binaryHeap_t * const binaryHeap);

/** 
 * Destroy the binary heap.
 *
 * Removes and deallocates all entries from the binary heap. If the binary heap 
 * \e deleteEntry() member exists then deallocates the caller's entry data.
 *
 * \param binaryHeap \e binaryHeap_t pointer
 */
void binaryHeapDestroy(binaryHeap_t * const binaryHeap);

/**
 * Get the number of entries in the binary heap.
 *
 * Returns the number of entries in the binary heap.
 *
 * \param binaryHeap \e binaryHeap_t pointer
 * \return Number of entries in the binary heap
 */
size_t binaryHeapGetSize(const binaryHeap_t * const binaryHeap);

/**
 * Get the depth of the binary heap.
 *
 * Returns the depth of the binary heap.
 *
 * \param binaryHeap \e binaryHeap_t pointer
 * \return Depth of the binary heap
 */
size_t binaryHeapGetDepth(const binaryHeap_t * const binaryHeap);

/** 
 * Operate on each binary heap entry.
 *
 * Traverses the binary heap calling a function for each entry.
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating success
 */
bool binaryHeapWalk(binaryHeap_t * const binaryHeap, 
                    const binaryHeapWalkFunc_t walk);

/** 
 * Check the binary heap.
 *
 * Traverses the binary heap checking that the heap is consistent.
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \return \e bool indicating success. 
 */
bool binaryHeapCheck(binaryHeap_t * const binaryHeap);

#ifdef __cplusplus
}
#endif

#endif
