/*
 * binaryHeap_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "binaryHeap.h"

void *
interpCreate(void)
{
  return binaryHeapCreate(interpAlloc, 
                          interpDealloc,
                          interpDuplicateEntry,
                          interpDeleteEntry,
                          interpDebug, 
                          interpComp, 
                          NULL);
}

void *
interpFind(void * const binaryHeap, void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpInsert(void * const binaryHeap, void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpRemove(void * const binaryHeap, void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void 
interpClear(void * const binaryHeap)
{
  binaryHeapClear(binaryHeap);
}

void 
interpDestroy(void * const binaryHeap)
{
  binaryHeapDestroy(binaryHeap);
}

size_t
interpGetDepth(const void * const binaryHeap)
{
  return binaryHeapGetDepth(binaryHeap);
}

size_t
interpGetSize(const void * const binaryHeap)
{
  return binaryHeapGetSize(binaryHeap);
}

void *
interpGetMin(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetMax(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetFirst(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLast(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetNext(void * const binaryHeap, const void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetPrevious(void * const binaryHeap, const void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetUpper(void * const binaryHeap, const void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const binaryHeap, const void * const entry)
{
  (void)binaryHeap;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpCheck(void * const binaryHeap)
{
  return binaryHeapCheck(binaryHeap);
}

bool
interpWalk(void * const binaryHeap, const interpWalkFunc_t walk)
{
  return binaryHeapWalk(binaryHeap, walk);
}

bool
interpSort(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return false;
}

bool
interpBalance(void * const binaryHeap)
{
  (void)binaryHeap;
  interpError(__func__, __LINE__, "Not implemented!");
  return false;
}

void
interpPop(void * const binaryHeap)
{
  binaryHeapPop(binaryHeap);
  return;
}

void *
interpPush(void * const binaryHeap, void * const entry)
{
  return binaryHeapPush(binaryHeap, entry);
}

void *
interpPeek(void * const binaryHeap)
{
  return binaryHeapPeek(binaryHeap);
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}
