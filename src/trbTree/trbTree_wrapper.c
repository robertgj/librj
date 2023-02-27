/*
 * trbTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "trbTree.h"

void *
interpCreate(void)
{
  return trbTreeCreate(interpAlloc, 
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
  return trbTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return trbTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return trbTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  trbTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  trbTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return trbTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return trbTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return trbTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return trbTreeGetMax(tree);
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

void *
interpGetNext(void * const tree, const void * const entry)
{
  return trbTreeGetNext(tree, entry);
}

void *
interpGetPrevious(void * const tree, const void * const entry)
{
  return trbTreeGetPrevious(tree, entry);
}

void *
interpGetUpper(void * const tree, const void * const entry)
{
  return trbTreeGetUpper(tree, entry);
}

void *
interpGetLower(void * const tree, const void * const entry)
{
  return trbTreeGetLower(tree, entry);
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return trbTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return trbTreeCheck(tree);
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
