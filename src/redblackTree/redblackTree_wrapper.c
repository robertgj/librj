/*
 * redblackTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "redblackTree.h"

void *
interpCreate(void)
{
  return redblackTreeCreate(interpAlloc,
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
  return redblackTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return redblackTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return redblackTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  redblackTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  redblackTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return redblackTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return redblackTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return redblackTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return redblackTreeGetMax(tree);
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
interpGetNext(void * const tree,  const void * const entry)
{
  return redblackTreeGetNext(tree, entry);
}

void *
interpGetPrevious(void * const tree,  const void * const entry)
{
  return redblackTreeGetPrevious(tree, entry);
}

void *
interpGetUpper(void * const tree,  const void * const entry)
{
  return redblackTreeGetUpper(tree, entry);
}

void *
interpGetLower(void * const tree,  const void * const entry)
{
  return redblackTreeGetLower(tree, entry);
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return redblackTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return redblackTreeCheck(tree);
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
