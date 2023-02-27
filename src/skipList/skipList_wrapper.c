/*
 * skipList_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "skipList.h"

void *
interpCreate(void)
{
  return skipListCreate(128,
                        interpAlloc,
                        interpDealloc,
                        interpDuplicateEntry,
                        interpDeleteEntry,
                        interpDebug,
                        interpComp,
                        NULL);
}

void *
interpFind(void * const tree, void * const entry)
{
  return skipListFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return skipListInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return skipListRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  skipListClear(tree);
}

void 
interpDestroy(void * const tree)
{
  skipListDestroy(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return skipListGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return skipListGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return skipListGetMax(tree);
}

void *
interpGetNext(void * const tree, const void * const entry)
{
  return skipListGetNext(tree, entry);
}

void *
interpGetPrevious(void * const tree, const void * const entry)
{
  return skipListGetPrevious(tree, entry);
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return skipListWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return skipListCheck(tree);
}

void *
interpGetFirst(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLast(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

size_t
interpGetDepth(const void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpGetUpper(void * const tree, const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const tree, const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpSort(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpBalance(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void
interpPop(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return;
}

void *
interpPeek(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpPush(void * const tree, void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}
