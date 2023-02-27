/*
 * stack_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "stack.h"

void *
interpCreate(void)
{
  return stackCreate(1,
                     interpAlloc, 
                     interpDealloc,
                     interpDuplicateEntry,
                     interpDeleteEntry,
                     interpDebug, 
                     NULL);
}

void *
interpFind(void * const stack, void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpInsert(void * const stack, void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpRemove(void * const stack, void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void 
interpClear(void * const stack)
{
  stackClear(stack);
}

void 
interpDestroy(void * const stack)
{
  stackDestroy(stack);
}

size_t
interpGetDepth(const void * const stack)
{ 
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

size_t
interpGetSize(const void * const stack)
{
  return stackGetSize(stack);
}

void *
interpGetMin(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetMax(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetFirst(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLast(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetNext(void * const stack, const void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetPrevious(void * const stack, const void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetUpper(void * const stack, const void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const stack, const void * const entry)
{
  (void)stack;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpCheck(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpWalk(void * const stack, const interpWalkFunc_t walk)
{
  return stackWalk(stack, walk);
}

bool
interpSort(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpBalance(void * const stack)
{
  (void)stack;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void 
interpPop(void * const stack)
{
  stackPop(stack);
  return;
}

void *
interpPeek(void * const stack)
{
  return stackPeek(stack);
}

void *
interpPush(void * const stack, void * const entry)
{
  return stackPush(stack, entry);
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}
