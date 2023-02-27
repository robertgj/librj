/** 
 * \file stack.h 
 *
 * Public interface for a stack type.
 */

#if !defined(STACK_H)
#define STACK_H


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

/**
 * \e stack_t structure. An opaque type for a stack. Only pointers
 * to user data are stacked. Memory management of stacked objects 
 * is the responsibility of the caller. 
 */
typedef struct stack_t stack_t;

/**
 * \e stack_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param amount of memory requested
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \e NULL indicates failure.
 */
typedef void* (*stackAllocFunc_t)(const size_t amount, void * const user);

/**
 * \e stack_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void (*stackDeallocFunc_t)(void * const pointer, void * const user);

/**
 * \e stack_t entry data memory duplicator callback function.
 *
 * Stack entry duplicator call-back for caller's entry data.
 *
 * \param entry pointer to memory to be duplicated
 * \param user \e void pointer to user data to be echoed by callbacks
 */
typedef void *(*stackDuplicateEntryFunc_t)(void * const entry, 
                                           void * const user);

/**
 * \e stack_t entry data memory de-allocator.
 *
 * Memory deallocation call-back for caller's entry data. If defined, 
 * this callback is used to deallocate entry data when appropriate.
 *
 * \param entry pointer to caller's memory to be deallocated
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool for compatibility with walk.
 */
typedef bool (*stackDeleteEntryFunc_t)(void * const entry, void * const user);

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
typedef void (*stackDebugFunc_t)(const char *function,  
                                 const unsigned int line,
                                 void * const user,
                                 const char *format, ...);

/**
 * Operate on entry.
 * 
 * Callback function run on an entry by \e stackWalk().
 *
 * \param entry \e entry defined by the caller
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \e bool indicating success.
 */
typedef bool (*stackWalkFunc_t)(void * const entry, void * const user);

/**
 * Create an empty stack.
 * 
 * Creates and initialises an empty \e stack_t instance
 *
 * \param space initial number of available stack elements
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param duplicateEntry entry duplication callback for caller's entry data
 * \param deleteEntry memory deallocator callback for callers entry data
 * \param debug message function callback
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return pointer to a \e stack_t. \e NULL indicates failure.
 */
stack_t *stackCreate(const size_t space,
                     const stackAllocFunc_t alloc, 
                     const stackDeallocFunc_t dealloc, 
                     const stackDuplicateEntryFunc_t duplicateEntry, 
                     const stackDeleteEntryFunc_t deleteEntry, 
                     const stackDebugFunc_t debug,
                     void * const user);

/**
 * Push an entry onto the stack.
 *
 * Pushes an entry onto the stack.
 *
 * \param stack \e stack_t pointer 
 * \param entry caller's \e entry data 
 * \return \e void pointer to the entry data pushed. 
 * \e NULL indicates failure.
 */
void *stackPush(stack_t * const stack, void * const entry);

/**
 * Pop an entry from the stack.
 *
 * Pops an entry from the stack. If the \e deleteEntry function has been
 * defined then the entry memory is freed and NULL is returned. Otherwise,
 * a pointer to the entry is returned.
 *
 * \param stack \e stack_t pointer
 * \return \e void pointer to the entry data popped
 */
void* stackPop(stack_t * const stack);

/**
 * Peek at the top entry on the stack.
 *
 * Peek at the top entry on the stack.
 *
 * \param stack \e stack_t pointer
 * \return entry \e entry data. \e NULL indicates failure.
 */
void *stackPeek(stack_t * const stack);

/**
 * Clear the stack.
 *
 * Clears all entries from the stack.
 *
 * \param stack \e stack_t pointer
 */
void stackClear(stack_t * const stack);

/** 
 * Destroy the stack.
 *
 * Destroys the stack.
 *
 * \param stack \e stack_t pointer
 */
void stackDestroy(stack_t * const stack);

/**
 * Get the number of entries available in the stack.
 *
 * Returns the number of entries available in the stack.
 *
 * \param stack \e stack_t pointer
 * \return Number of entries in the stack
 */
size_t stackGetSpace(const stack_t * const stack);

/**
 * Get the maximum number of entries in the stack.
 *
 * Returns the maximum number of entries in the stack.
 *
 * \param stack \e stack_t pointer
 * \return Maximum number of entries in the stack
 */
size_t stackGetSize(const stack_t * const stack);

/** 
 * Operate on each stack entry.
 *
 * Traverses the stack from head to tail calling a function for each entry.
 *
 * \param stack pointer to \e stack_t
 * \param walk pointer to a callback function to be called for each entry
 * \return \e bool indicating success
 */
bool stackWalk(stack_t * const stack, const stackWalkFunc_t walk);

#ifdef __cplusplus
}
#endif

#endif
