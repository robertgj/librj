/*
 * bsTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "bsTree.h"

void *
interpCreate(void)
{
  return bsTreeCreate(interpAlloc,
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
  return bsTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return bsTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return bsTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  bsTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  bsTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return bsTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return bsTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return bsTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return bsTreeGetMax(tree);
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
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetPrevious(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetUpper(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return bsTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return bsTreeCheck(tree);
}

bool
interpSort(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return false;
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
  return NULL;
}

void *
interpPush(void * const tree, void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}
